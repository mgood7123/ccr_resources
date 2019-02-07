/* Author: mgood7123 (Matthew James Good) http://github.com/mgood7123 */

#define LIBUNICODE
#pragma once

#include <limits.h>
#include <stdio.h>
#define ps(x)      printf("%s = %s\n", #x, x);
#define pi(x)      printf("%s = %d\n", #x, x);
#define pp(x)      printf("%s = %p\n", #x, x);
#include <assert.h>


int UgetBit(unsigned char byte, int position) // position in range 0-7
{
    return (byte >> position) & 0x1;
}

struct bstate {
	unsigned char * data;
	size_t index;
	size_t bitcount;
};

typedef struct bstate bstate;

bstate * bopen(unsigned char * data) {
	bstate * n = malloc(sizeof(bstate));
	n->data = data;
	n->index = 0;
	n->bitcount = 0;
	return n;
}

void bclose(bstate * n) {
	n->data = NULL;
	n->index = 0;
	n->bitcount = 0;
	free(n);
	n = NULL;
}

unsigned char * bread(bstate * n, int len, int * ii) {
	unsigned char * bits = malloc(len);
	int was_set = 0;
	
	for (int i = 0; i < len; i++)
	{
		bits[i]=!UgetBit(n->data[n->index], *ii)?'0':'1';
		(*ii)--;
		n->bitcount++;
		if (n->bitcount>7) {
			n->index++;
			n->bitcount = 0;
			was_set = 1;
			*ii = 7;
		}
	}
	bits[len] = 0;
	was_set = 0;
	return bits;
}

int dts(int x) {
	int n = 0;
	while(x /= 2) n++;
	return n;
}

// should this use a bstate?
unsigned char * bwrite(unsigned char * encoding) {
	// assume host byte size is CHAR_BIT
	if(strlen(encoding)%CHAR_BIT != 0) {
		printf("warning: bit length is not evenly divisible against a byte, the last byte may be corrupted\n");
	}
	size_t len = strlen(encoding)/CHAR_BIT;
	unsigned char * s = malloc(len);
	memset(s, 0, len);
	
	int i;
	for( i = 0; i < strlen(encoding); i++) {
		s[i>>dts(CHAR_BIT)] |= ( encoding[i] == '1' ? 1 : 0 ) << ((CHAR_BIT-1)-(i)&(CHAR_BIT-1));
	}
	s[len] = 0;
	return s;
}
