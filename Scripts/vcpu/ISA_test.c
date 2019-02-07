#pragma once
int CPU_Endian = 0;
#define E_LITTLE 0
#define E_BIG 1
#define RIGHT_TO_LEFT E_LITTLE
#define LEFT_TO_RIGHT E_BIG
#include <assert.h>
#include <stdio.h>
/*
FIN 1P
FIN: |0011|RP|0|
1P: 001
FIN 1P: 0011 0010
*/
char * FIN_1P = "\
0011\
001\
0";

/*
JCN DATA
JCN: |0001|CN  |ADDR|ADDR|
CN:
    |1|x|x|x|: invert all other jump conditions
    
    |0|x|x|1|: jump if test signal of 4004 is = 0
    |1|x|x|1|: jump if test signal of 4004 is = 1
    |0|x|1|x|: jump if carry bit = 1
    |1|x|1|x|: jump if carry bit = 0
    |0|1|x|x|: jump if accumulator = 0
    |1|1|x|x|: jump if accumulator != 0
DATA anything
*/

char * JCN_ALL = "\
0001\
1111\
1010\
0101\
";

int swap_(void) {
	unsigned int x = 1;
	return (int) (((char *)&x)[0]);
}

int swap = 0;

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#define BINMODE_WRITE 0
#define BINMODE_APPEND 1
// add two write modes:
// truncate to 0 and append from index 0
// append from current index without truncating
#define pi(x)      printf("%s = %d\n", #x, x);
#define ps(x)      printf("%s = %s\n", #x, x);
#define pc(x)      printf("%s = %c\n", #x, x);
#define pd(x)      printf("%s = %d\n", #x, (int)x);
int bitcount_ = 1;

struct strStream {
    unsigned char *s;
    int bitpos;
};

struct binbuf {
	struct strStream * data;
	size_t length;
	int BINMODE;
	int bitcount;
};

struct strStream * strStream_init(unsigned char * string) {
	struct strStream * str = malloc(sizeof(struct strStream));
	str->s = string;
	str->bitpos = 0;
	return str;
}

void strStream_deinit(struct strStream * * str) {
	(*str)->s = NULL;
	(*str)->bitpos = 0;
	free((*str));
}

struct binbuf binbuf_init(int mode) {
	struct binbuf bin = {0};
	bin.data = strStream_init(NULL);
	bin.BINMODE = mode;
	bin.bitcount = 0;
	return bin;
}

// reads a binary string writing it as literal to a buffer
int read__binary(struct binbuf * bin, char * s) {
    //printf( "INPUT:%s\n", s );
    int newlen = strlen(s);
    int totlen = bin->length + newlen;  // adds number of bits in input string
    
    // realloc new total (rounded up) bytes for new bittotal
    bin->data->s = realloc(bin->data->s, ((bin->length = totlen) + (CHAR_BIT-1)) /CHAR_BIT );
    memset( bin->data->s + ( ( bin->bitcount + 7 ) / 8 ), 0, ( ( bin->length - bin->bitcount + 7 ) / 8 ) );

	int i;
	for( i = 0; i < newlen; i++) {
		bin->data->s[((bin->bitcount + i)>>3)] |= ( s[i] == '1' ? 0x80 : 0 ) >> (7-((bin->bitcount + i  )&7));
	}
	
	// bitcount is really the same as bin->length, but delayed ....
	bin->bitcount += i;
	return 0;
}

// reads a string returning it as a binary string
unsigned char * read__string(struct strStream * strStream, int len) {
	unsigned char * bits = malloc(len+1);
	int i;
	for ( i = 0; i < len; i++) {
	    if( strStream->s[ (strStream->bitpos + i)>>3 ] 
		        & ( 1 << ( ( strStream->bitpos + i ) & 7 ) ) )
		    bits[i] = '1';
		else 
		    bits[i] = '0';
	}
	strStream->bitpos += i;
	bits[i] = 0;
	return bits;
}

/* Author: mgood7123 (Matthew James Good) http://github.com/mgood7123 */
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#define REGEX_STR
#pragma once

int getbitgroupcount(int bit) {
	return snprintf(stdout, 0, "%d", bit);
}

struct regex_string_structure
{
	char * string;
	int index;
	int len;
	int malloced;
};

struct regex_string
{
	char * string;
	struct regex_string_structure escaped;
	struct regex_string_structure json;
	struct regex_string_structure c_define;
	struct regex_string_structure no_spaces;
	struct regex_string_structure reverse;
	int index;
	int len;
	int malloced;
};

#define str_malloc_(y, z) \
	y.string = malloc(z); \
	memset(y.string, 0, z); \
	y.malloced = z; \
	y.len = 0; \
	y.index = 0;

#define str_mallocr(y, z) \
	str_malloc_(y, z); \
	str_malloc_(y.escaped, z); \
	str_malloc_(y.json, z); \
	str_malloc_(y.c_define, z); \
	str_malloc_(y.no_spaces, z); \
	str_malloc_(y.reverse, z); \
	
#define str_malloc(y, z) \
	struct regex_string y; \
	str_mallocr(y, z);

#define str_new(str) \
	str_malloc(str, 1) \
	
#define str_free_(y) \
		memset(y.string, 0, y.malloced); \
		free(y.string); \
		y.string = NULL; \
		y.malloced = 0; \
		y.len = 0; \
		y.index = 0; \

#define str_free(y) \
	{ \
		str_free_(y); \
		str_free_(y.escaped); \
		str_free_(y.json); \
		str_free_(y.c_define); \
		str_free_(y.no_spaces); \
		str_free_(y.reverse); \
	}
	
#define str_reset_(str) { \
	str_free_(str) \
	str_malloc_(str, 1) \
}

#define str_reset(str) { \
	str_reset_(str) \
	str_reset_(str.escaped) \
	str_reset_(str.json) \
	str_reset_(str.c_define) \
	str_reset_(str.no_spaces) \
	str_reset_(str.reverse) \
}

#define str_realloc(y, z) \
	y.string = realloc(y.string, z); \
	if (y.malloced < z) { \
		memset(y.string+y.malloced, 0, y.malloced-z); \
	} \
	y.malloced = z;
	
#define str_info_(str) \
		printf("%s.index = %d\n", #str, str.index); \
		printf("%s.len = %d\n", #str, str.len); \
		printf("%s.malloced = %d\n", #str, str.malloced); \
		printf("%s.string = %s\n", #str, str.string); \

#define str_info(str) { \
	str_info_(str) \
	str_info_(str.escaped) \
	str_info_(str.json) \
	str_info_(str.c_define) \
	str_info_(str.no_spaces) \
	str_info_(str.reverse) \
}

#define str_insert_char(str, ch) { \
	str_realloc(str, str.malloced+2); \
	str.string[str.index] = ch; \
	str.index++; \
	str.string[str.index] = 0; \
	str.len = strlen(str.string); \
}

#define str_insert_char_reverse(str, ch) { \
	str_realloc(str, str.malloced+2); \
	memmove(str.string+1, str.string, str.len); \
	str.string[0] = ch; \
	str.index++; \
	str.string[str.index] = 0; \
	str.len = strlen(str.string); \
}

#define str_undo(str) { \
	str.index--; \
	str.string[str.index] = 0; \
	str_realloc(str, str.malloced-1); \
	str.len = strlen(str.string); \
}

#define str_undo_reverse(str) { \
	str.index--; \
	memmove(str.string+1, str.string, str.len-1); \
	str.string[str.index] = 0; \
	str_realloc(str, str.malloced-1); \
	str.len = strlen(str.string); \
}

#define rng(s, e) ( uch >= s && uch <= e)

#define str_insert_string(str, string) { \
	const unsigned char * s = string; \
	const unsigned int size = strlen(string); \
	int c; \
	unsigned char uch; \
	str_insert_char(str.c_define, '#') \
	str_insert_char(str.c_define, 'd') \
	str_insert_char(str.c_define, 'e') \
	str_insert_char(str.c_define, 'f') \
	str_insert_char(str.c_define, 'i') \
	str_insert_char(str.c_define, 'n') \
	str_insert_char(str.c_define, 'e') \
	str_insert_char(str.c_define, ' ') \
	int i; \
	for (i = 0; i < size; ++i) { \
		c = s[i]; \
		uch = (unsigned char) s[i]; \
		str_insert_char(str, c); \
		str_insert_char_reverse(str.reverse, c); \
		if (c != ' ' || c != '\t') str_insert_char(str.no_spaces, c); \
		\
		/* escape sequences under gnu gcc are interperated by both cc1, and the terminal, assembling a string containing an invalid sequence will print as if it does not have the \ before it*/ \
		\
		if (isalnum(c) ) { \
			str_insert_char(str.c_define, c); \
		} \
		else str_insert_char(str.c_define, '_'); \
		if (c == '\a') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, 'a'); \
		} \
		else if (c == '\b') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, 'b'); \
			str_insert_char(str.json, '\\'); \
			str_insert_char(str.json, 'b'); \
		} \
		else if (c == '\f') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, 'f'); \
			str_insert_char(str.json, '\\'); \
			str_insert_char(str.json, 'f'); \
		} \
		else if (c == '\n') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, 'n'); \
			str_insert_char(str.json, '\\'); \
			str_insert_char(str.json, 'n'); \
		} \
		else if (c == '\r') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, 'r'); \
			str_insert_char(str.json, '\\'); \
			str_insert_char(str.json, 'r'); \
		} \
		else if (c == '\t') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, 't'); \
			str_insert_char(str.json, '\\'); \
			str_insert_char(str.json, 't'); \
		} \
		else if (c == '\v') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, 'v'); \
		} \
		else if (c == '\\') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.json, '\\'); \
			str_insert_char(str.json, '\\'); \
		} \
		else if (c == '\'') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, '\''); \
			str_insert_char(str.json, '\\'); \
			str_insert_char(str.json, '\''); \
		} \
		else if (c == '"') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, '"'); \
			str_insert_char(str.json, '\\'); \
			str_insert_char(str.json, '"'); \
		} \
		else if (c == '\?') { \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, '?'); \
		} \
		else if (c >= ' ' && c <= 0x7e) { \
			str_insert_char(str.escaped, c); \
			str_insert_char(str.json, c); \
		} \
		else if (c >= '0' && c <= '9' ) { \
			/* Print \ooo */ \
			str_insert_char(str.escaped, '\\'); \
			str_insert_char(str.escaped, '0' + (c >> 6)); \
			str_insert_char(str.escaped, '0' + ((c >> 3) & 0x7)); \
			str_insert_char(str.escaped, '0' + (c & 0x7)); \
		} else { \
			/* Print \[[o]o]o */ \
			str_insert_char(str.escaped, '\\'); \
			if ((c >> 3) != 0) { \
				if ((c >> 6) != 0) str_insert_char(str.escaped, '0' + (c >> 6)); \
				str_insert_char(str.escaped, '0' + ((c >> 3) & 0x7)); \
			} \
			str_insert_char(str.escaped, '0' + (c & 0x7)); \
		} \
	} \
	str_insert_char(str.c_define, '\n'); \
}

#define str_int2string(x,y) \
	char * x = malloc(getbitgroupcount(y)); \
	sprintf(x, "%d", y);
	
#define str_insert_int(str, integer) { \
	str_int2string(j, integer); \
	str_insert_string(str, j); \
	free(j); \
}

int reverseBool(int val) {
	if (val == 1 || val == 0) return val^1;
	else return val;
}

void str_output(struct regex_string * str, const char * file);

void str_output_append(struct regex_string * str, const char * file);

#define str_output(str, file) { \
	FILE * stream = stdout; \
	int isfile = 0; \
	if (strcmp("stdin" file) == 0) stream = stdin; \
	else if(strcmp("stdout" file) == 0) stream = stdout; \
	else if(strcmp("stderr" file) == 0) stream = stderr; \
	else isfile = 1; \
	if (isfile) stream = fopen(file, "w"); \
	fwrite(str.string,str.len, 1, stream); \
	if (isfile) fclose(stream); \
}

#define str_output_append(str, file) { \
	FILE * stream = stdout; \
	int isfile = 0; \
	if (strcmp("stdin" file) == 0) stream = stdin; \
	else if(strcmp("stdout" file) == 0) stream = stdout; \
	else if(strcmp("stderr" file) == 0) stream = stderr; \
	else isfile = 1; \
	if (isfile) stream = fopen(file, "a"); \
	fwrite(str.string,str.len, 1, stream); \
	if (isfile) fclose(stream); \
}

int bcmpcq__RE(void const *vp, size_t n, void const *vp2, size_t n2)
{
    int string_match = 0;
    unsigned char const *p = vp;
    unsigned char const *p2 = vp2;
    int matches=0;
    int contains_matches=0;
    for (size_t i=0; i<n; i++) {
        if (p[i] == p2[i] || (p[i] == 'x' || p2[i] == 'x')) {
            matches++;
        } else {
            if (matches) contains_matches = 1;
            break;
        }
    }
    if (matches == 0) {
        return -1;
    } else {
        int ret = 0;
        if (contains_matches) ret = 1;
        return ret;
    }
}

int bytecmpcRE(void const * p, void const * pp) { return bcmpcq__RE(p, strlen(p), pp, strlen(pp)); }


void dis(char * binary) {
	CPU_Endian=E_BIG;
	str_new(binary_);
	str_insert_string(binary_, binary);
	struct binbuf binstr = binbuf_init(BINMODE_APPEND);
	swap = swap_();
	pi(swap)
	printf("Endian: %s\n", swap==1?"Big":"Little");
	pi(CPU_Endian)
	printf("CPU_Endian: %s\n", CPU_Endian==1?"Big":"Little");
	
	if (swap == E_LITTLE && CPU_Endian == E_LITTLE) read__binary(&binstr, binary_.string);
	if (swap == E_LITTLE && CPU_Endian == E_BIG) read__binary(&binstr, binary_.reverse.string);
	if (swap == E_BIG && CPU_Endian == E_LITTLE) read__binary(&binstr, binary_.reverse.string);
	if (swap == E_BIG && CPU_Endian == E_BIG) read__binary(&binstr, binary_.string);
	char * instr = read__string(binstr.data, 4);
	if (!bytecmpcRE(instr, "001x")) {
		puts("FIN");
		if (!bytecmpcRE(read__string(binstr.data, 3), "00x")) {
			puts("register pair 1: register 2 and register 3");
			if (!bytecmpcRE(read__string(binstr.data, 1), "0")) {
				puts("VALID INSTRUCTION");
			}
		}
	}
	else if (!bytecmpcRE(instr, "000x")) {
		puts("JCN");
		int inverse_meaning = 0;
		if (!bytecmpcRE(read__string(binstr.data, 1), "1")) {
			inverse_meaning = 1;
		}
		if (!bytecmpcRE(read__string(binstr.data, 1), "1")) {
			printf("jump if accumulator %s 0\n", inverse_meaning==0?"=":"!=");
		}
		if (!bytecmpcRE(read__string(binstr.data, 1), "1")) {
			printf("jump if carry bit = %s\n", inverse_meaning==0?"1":"0");
		}
		if (!bytecmpcRE(read__string(binstr.data, 1), "1")) {
			printf("jump if test signal of 4004 is = %s\n", inverse_meaning==0?"0":"1");
		}
		char * DATA = read__string(binstr.data, 8);
		ps(DATA)
	}
}

void main() {
	dis(FIN_1P);
	dis(JCN_ALL);
}
