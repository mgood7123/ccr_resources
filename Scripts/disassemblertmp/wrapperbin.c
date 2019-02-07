#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NCHARS (UCHAR_MAX+1)

struct node {
	int freq;
	int c;
	int left, right, parent;
	int value;
};

struct node array[NCHARS * 2 - 1]; /* huffman tree */

int root; /* root of the huffman tree */

int leaf[NCHARS]; /* characters actually used in the file */
int nleafs;

int compare(const void *aa, const void *bb)
{
	int a = *(const int *)(aa), b = *(const int *)(bb);
	if(array[a].freq < array[b].freq) return -1;
	if(array[a].freq > array[b].freq) return +1;
	if(array[a].c < array[b].c) return -1;
	if(array[a].c > array[b].c) return +1;
	return 0;
}

/* a very fast huffman tree construction */

void huffman(void)
{
	nleafs = 0;

	int c;
	for(c = 0; c < NCHARS; c++) {
		if(array[c].freq > 0) {
			leaf[nleafs++] = c;
		}
	}

	if(nleafs == 0) { root = 0; return; } /* special case for no characters */

	if(nleafs == 1) { root = leaf[0]; return; } /* special case for one character */

	qsort(leaf, nleafs, sizeof(*leaf), compare);

	/* always begin by adding two leaves */

	root = NCHARS; /* root of the huffman tree */

	array[root].freq = array[leaf[0]].freq + array[leaf[1]].freq;
	array[root].left = leaf[0];
	array[root].right = leaf[1];
	array[leaf[0]].parent = root;
	array[leaf[0]].value = 0;
	array[leaf[1]].parent = root;
	array[leaf[1]].value = 1;
	root++;

	int a = 2; /* next smallest leaf */
	int b = NCHARS; /* next smallest node */

	while(root < NCHARS + nleafs - 1) {
		if(a < nleafs && array[leaf[a]].freq <= array[b].freq) {
			array[root].freq = array[leaf[a]].freq;
			array[root].left = leaf[a];
			array[leaf[a]].parent = root;
			array[leaf[a]].value = 0;
			a++;
		} else {
			array[root].freq = array[b].freq;
			array[root].left = b;
			array[b].parent = root;
			array[b].value = 0;
			b++;
		}
		if(a < nleafs && array[leaf[a]].freq <= array[b].freq) {
			array[root].freq += array[leaf[a]].freq;
			array[root].right = leaf[a];
			array[leaf[a]].parent = root;
			array[leaf[a]].value = 1;
			a++;
		} else {
			array[root].freq += array[b].freq;
			array[root].right = b;
			array[b].parent = root;
			array[b].value = 1;
			b++;
		}
		root++;
	}
	root--;
}

void writeword(FILE *wfp, int word)
{
	putc(word >> 24 & 0xff, wfp);
	putc(word >> 16 & 0xff, wfp);
	putc(word >> 8 & 0xff, wfp);
	putc(word >> 0 & 0xff, wfp);
}
void writebyte(FILE *wfp, int byte)
{
	putc(byte & 0xff, wfp);
}

int wbuffer = 0, wbit = 7;

void writebit(FILE *wfp, int bit)
{
	wbuffer |= (bit & 1) << wbit;
	wbit--;
	if(wbit < 0) {
		writebyte(wfp, wbuffer);
		wbuffer = 0;
		wbit = 7;
	}
}
void flushbit(FILE *wfp)
{
	if(wbit < 7) {
		writebyte(wfp, wbuffer);
	}
}

void compress(FILE *rfp, FILE *wfp)
{
	int a;
	for(a = 0; a < NCHARS; a++) {
		array[a].freq = 0;
		array[a].c = a;
	}

	int b;
	while(b = getc(rfp), b != EOF) { /* count the characters in the file */
		array[b].freq++;
	}

	huffman();

	writeword(wfp, nleafs);
	int c;
	for(c = 0; c < NCHARS; c++) {
		if(array[c].freq > 0) {
			writebyte(wfp, array[c].c);
			writeword(wfp, array[c].freq);
		}
	}

	rewind(rfp);

	int d;
	while(d = getc(rfp), d != EOF) {
		int e = 1;
		int f;
		for(f = d; f != root; f = array[f].parent) {
			e = (e << 1) + array[f].value;
		}
		while(e > 1) {
			writebit(wfp, (e & 1)); /* huffman code must be reversed */
			e >>= 1;
		}
	}
	flushbit(wfp);
}

int readword(FILE *rfp)
{
	int a = getc(rfp) & 0xff;
	int b = getc(rfp) & 0xff;
	int c = getc(rfp) & 0xff;
	int d = getc(rfp) & 0xff;
	return(a << 24 | b << 16 | c << 8 | d << 0);
}
int readbyte(FILE *rfp)
{
	int a = getc(rfp) & 0xff;
	return(a);
}

int rbuffer = 0, rbit = 0;

int readbit(FILE *rfp)
{
	rbit--;
	if(rbit < 0) {
		rbuffer = readbyte(rfp);
		rbit = 7;
	}
	return(rbuffer >> rbit & 1);
}

int total; /* uncompressed file length */

void uncompress(FILE *rfp, FILE *wfp)
{
	int a;
	for(a = 0; a < NCHARS; a++) {
		array[a].freq = 0;
		array[a].c = a;
	}

	total = 0;

	int b = readword(rfp);
	int c;
	for(c = 0; c < b; c++) {
		int d = readbyte(rfp);
		int e = readword(rfp);
		array[d].freq = e;
		total += e;
	}

	huffman();

	while(total > 0) {
		int f;
		for(f = root; f >= NCHARS; ) {
			int g = readbit(rfp);
			if(g == 0) {
				f = array[f].left;
			} else {
				f = array[f].right;
			}
		}
		if(putc(array[f].c, wfp) == EOF) { perror("write"); return; }

		total--;
	}
}

/* primitive command line */

int main(int argc, char **argv)
{
	if(argc < 3) {
		fprintf(stderr, "usage: huffman [e|d] infile > outfile\n");
		return(1);
	}

	int mode;
	if(strcmp(argv[1], "e") == 0) {
		mode = 0;
	} else if(strcmp(argv[1], "d") == 0) {
		mode = 1;
	} else {
		fprintf(stderr, "must [e]ncode or [d]ecode\n");
		return(1);
	}

	FILE *fp;
	if(fp = fopen(argv[2], "rb"), fp == 0) { perror("open"); return(1); }

	if(mode == 0) compress(fp, stdout);
	if(mode == 1) uncompress(fp, stdout);

	if(fclose(fp) != 0) { perror("close"); return(1); }

	return(0);
}

#if 0
void debugfreq(void)
{
	int a;
	for(a = 0; a < nleafs; a++)
		fprintf(stderr, "'%c' freq %d\n", array[leaf[a]].c, array[leaf[a]].freq);
}
int debugtree(int a)
{
	if(a < NCHARS) {
		fprintf(stderr, "%c", array[a].c);
	} else {
		fprintf(stderr, "(");
		debugtree(array[a].left);
		fprintf(stderr, ",");
		debugtree(array[a].right);
		fprintf(stderr, ")");
	}
}
#endif
