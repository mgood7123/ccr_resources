 
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


static struct termios getch_old, getch_new;

/* Initialize new terminal i/o settings */
void getch_initTermios(int echo) 
{
  tcgetattr(0, &getch_old); /* grab old terminal i/o settings */
  getch_new = getch_old; /* make new settings same as old settings */
  getch_new.c_lflag &= ~ICANON; /* disable buffered i/o */
  getch_new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &getch_new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void getch_resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &getch_old);
}

int getch_rawmode = 0;
struct termios getch_orig_termios; /* In order to restore at exit.*/
int getch_atexit_registered = 0; /* Register atexit just 1 time. */

static void getch_disableRawMode(int fd) {
    /* Don't even check the return value as it's too late. */
    if (getch_rawmode && tcsetattr(fd,TCSAFLUSH,&getch_orig_termios) != -1)
        getch_rawmode = 0;
}

/* At exit we'll try to fix the terminal to the initial conditions. */
static void getch_linenoiseAtExit(void) {
    getch_disableRawMode(STDIN_FILENO);
}

/* Raw mode: 1960 magic shit. */
static int getch_enableRawMode(int fd, int echo) {
    struct termios raw;

    if (!isatty(STDIN_FILENO)) goto fatal;
    if (!getch_atexit_registered) {
        atexit(getch_linenoiseAtExit);
        getch_atexit_registered = 1;
    }
    if (tcgetattr(fd,&getch_orig_termios) == -1) goto fatal;

    raw = getch_orig_termios;  /* modify the original mode */
    raw.c_lflag &= ~( (echo ? ECHO : ~ECHO) | ICANON);
    /* control chars - set return condition: min number of bytes and timer.
     * We want read to return every single byte, without timeout. */
    raw.c_cc[VMIN] = 1; raw.c_cc[VTIME] = 0; /* 1 byte, no timer */

    /* put terminal in raw mode after flushing */
    if (tcsetattr(fd,TCSAFLUSH,&raw) < 0) goto fatal;
    getch_rawmode = 1;
    return 0;

fatal:
    errno = ENOTTY;
    return -1;
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
//   initTermios(echo);
  getch_enableRawMode(0, echo);
  ch = fgetc(stdin);
//   resetTermios();
  getch_disableRawMode(0);
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(1);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(0);
}
