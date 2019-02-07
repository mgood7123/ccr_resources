#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#define BINMODE_WRITE 0
#define BINMODE_APPEND 1
// add two write modes:
// truncate to 0 and append from index 0
// append from current index without truncating
#define ENDIAN_LITTLE 1
#define ENDIAN_BIG 0
#define pui64(x) printf("%s = %" PRIu64 "\n", #x, x);
#define pi(x)      printf("%s = %d\n", #x, x);
#define ps(x)      printf("%s = %s\n", #x, x);
#define pc(x)      printf("%s = %c\n", #x, x);
#define psize_t(x) printf("%s = %zu\n", #x, x);
#define pz(x)      psize_t(x)
int bitcount_ = 1;

struct strStream {
    unsigned char *s;
    int bitpos;
    uint64_t *s64;
    int bitpos64;
};

struct binbuf {
	struct strStream * data;
	size_t length;
	size_t length64;
	int BINMODE;
	int bitcount;
	int bitcount64;
};

struct strStream * strStream_init(unsigned char * string) {
	struct strStream * str = malloc(sizeof(*str));
	str->s = string;
	str->s64 = NULL;
	str->bitpos = 0;
	str->bitpos64 = 0;
	return str;
}

void strStream_deinit(struct strStream * * str) {
	(*str)->s = NULL;
	(*str)->s64 = NULL;
	(*str)->bitpos = 0;
	(*str)->bitpos64 = 0;
	free((*str));
	*str = NULL;
}

struct binbuf binbuf_init(int mode) {
	struct binbuf bin = {0};
	bin.data = strStream_init(NULL);
	bin.BINMODE = mode;
	bin.bitcount = 0;
	bin.bitcount64 = 0;
	return bin;
}

int divtoshift(int x) {
	int n = 0;
	while(x /= 2) n++;
	return n;
}

// reads a binary string writing it as literal to a buffer

int read__binary(struct binbuf * bin, char * s) {
    //printf( "INPUT:%s\n", s );
    int newlen = strlen(s);
    int totlen = bin->length + newlen;  // adds number of bits in input string
    
    // realloc new total (rounded up) bytes for new bittotal
    bin->data->s = realloc(bin->data->s, ((bin->length = totlen) + (CHAR_BIT-1)) /CHAR_BIT );
    memset( bin->data->s + ( ( bin->bitcount + (CHAR_BIT-1)) /CHAR_BIT ), 0, ( ( bin->length - bin->bitcount + (CHAR_BIT-1)) /CHAR_BIT ) );

	int i;
	for( i = 0; i < newlen; i++) {
		bin->data->s[((bin->bitcount + i)>>divtoshift(CHAR_BIT))] |= ( s[i] == '1' ? (UINT8_MAX) : 0 ) >> ((CHAR_BIT-1)-(bin->bitcount + i)&(CHAR_BIT-1));
	}
	
	// bitcount is really the same as bin->length, but delayed ....
	bin->bitcount += i;
	return 0;
}

int read__binary64(struct binbuf * bin, char * s) {
    //printf( "INPUT:%s\n", s );
    int newlen = strlen(s);
    int totlen = bin->length64 + newlen;  // adds number of bits in input string
    
    // realloc new total (rounded up) bytes for new bittotal
    bin->data->s64 = realloc(bin->data->s64, ((bin->length64 = totlen) + (64-1)) /64 );
    memset( bin->data->s64 + ( ( bin->bitcount64 + (64-1) ) / 64 ), 0, ( ( bin->length64 - bin->bitcount64 + (64-1) ) / 64 ) );

	int i;
	for( i = 0; i < newlen; i++) {
		bin->data->s64[((bin->bitcount64 + i)>>divtoshift(64))] |= ( s[i] == '1' ? (UINT64_MAX) : 0 ) >> ((64-1)-(bin->bitcount64 + i)&(64-1));
	}
	
	// bitcount is really the same as bin->length, but delayed ....
	bin->bitcount64 += i;
	return 0;
}

unsigned char * bits = NULL;

int read__string_undo(struct strStream * strStream) {
	if (bits) strStream->bitpos -= strlen(bits);
	return 0;
}

// reads a string returning it as a binary string
unsigned char * read__string(struct strStream * strStream, int len, bool undo, bool endian) {
	if (undo == true) read__string_undo(strStream);
	if (bits) {
		memset(bits,0,strlen(bits));
		free(bits);
	}
	bits = malloc(len+1);
	int i, idx = 0;
	if (endian == ENDIAN_BIG) {
		for ( i = 0; i < len; i++) {
			if( strStream->s[ (strStream->bitpos + i)>>divtoshift(CHAR_BIT) ] 
					& ( 1 << ( ( strStream->bitpos + i ) & (CHAR_BIT-1) ) ) )
				bits[idx] = '1';
			else 
				bits[idx] = '0';
			idx++;
		}
	} else if (endian == ENDIAN_LITTLE) { // reads incorrectly
		for ( i = len-1; i >= 0; i--) {
			if( strStream->s[ (strStream->bitpos + i)>>divtoshift(CHAR_BIT) ] 
					& ( 1 << ( ( strStream->bitpos + i ) & (CHAR_BIT-1) ) ) )
				bits[idx] = '1';
			else
				bits[idx] = '0';
			idx++;
		}
	}
	strStream->bitpos += idx;
	bits[idx] = 0;
	return bits;
}

unsigned char * bits64 = NULL;

int read__string_undo64(struct strStream * strStream) {
	if (bits64) strStream->bitpos64 -= strlen(bits64);
	return 0;
}


unsigned char * read__string64(struct strStream * strStream, int len, bool undo, bool endian) {
	if (undo == true) read__string_undo64(strStream);
	if (bits64) {
		memset(bits64,0,strlen(bits64));
		free(bits64);
	}
	bits64 = malloc(len+1);
	int i, idx = 0;
	if (endian == ENDIAN_BIG) {
		for ( i = 0; i < len; i++) {
			if( strStream->s64[ (strStream->bitpos64 + i)>>divtoshift(64) ] 
					& ( (uint64_t)1 << ( ( strStream->bitpos64 + i ) & (64-1)) ) )
				bits64[idx] = '1';
			else 
				bits64[idx] = '0';
			idx++;
		}
	} else if (endian == ENDIAN_LITTLE) {
		for ( i = len; i > 0; i--) {
			if( strStream->s64[ (strStream->bitpos64 + i)>>divtoshift(64) ] 
					& ( (uint64_t)1 << ( ( strStream->bitpos64 + i ) & (64-1)) ) )
				bits64[idx] = '1';
			else 
				bits64[idx] = '0';
			idx++;
		}
	}
	strStream->bitpos64 += len;
	bits64[idx] = 0;
	return bits64;
}
