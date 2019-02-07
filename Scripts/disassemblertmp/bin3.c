#include "binary.h"
#ifndef MCASSEMBLER

int getBit(unsigned char byte, int position) // position in range 0-7
{
    return (byte >> position) & 0x1;
}

int bitcount = 0;

unsigned char * readbin(unsigned char * encoding, int member_len, int member_count) {
	unsigned char * bits = malloc(member_len*member_count);
	int bitmax = member_len-1;
	int ii = bitmax;
	int was_set = 0;
	
	for (int i = 0; i < member_len*member_count; i++)
	{
		bits[i]=!getBit(*encoding, ii)?'0':'1';
		ii--;
		bitcount++;
		if (bitcount>bitmax) {
			encoding = (encoding)+0x1;
			bitcount = 0;
			was_set = 1;
			ii = bitmax;
		}
	}
	if (was_set == 0 && bitcount<=bitmax) 
		for(int i = 0; i < member_len*member_count; i++)
			*encoding = *encoding << 0x1;
	was_set = 0;
	return bits;
}
#else
	#include "disassembler.h"
#endif
#include "regexEngine.h"
#include "../Shell/builtins/regex_str.h"
#include "../Shell/builtins/printfmacro.h"
bool yyfrom_FILE=false;
int    yy_FILE_string_pos = 0;
char * yy_FILE_string = NULL;
FILE * yy_FILE_file = 0;
struct strStream * STREAM = NULL;
char yy_CURRENT_CHARACTER = '\0';
char yy_CURRENT_CHARACTER_AS_STRING[2];
char yy_CURRENT_CHARACTER_AS_STRING_ORIGINAL[2];
int yy_bits_read = 0;
bool yyallocated = false;
char get_char(void)
{
	if (yy_bits_read == CHAR_BIT) {
		yy_bits_read = 0;
		yyallocated = false;
	}
	if (!yyallocated) {
		if (yyfrom_FILE == true) {
			if (yy_FILE_file == NULL) yy_FILE_file = stdin;
			puts("Please enter some text:");
			yy_CURRENT_CHARACTER = getc(yy_FILE_file);
			puts("You have entered some text");
		} else {
			yy_CURRENT_CHARACTER = yy_FILE_string[yy_FILE_string_pos++];
		}
		yy_CURRENT_CHARACTER_AS_STRING[0] = yy_CURRENT_CHARACTER;
		yy_CURRENT_CHARACTER_AS_STRING[1] = '\0';
		yy_CURRENT_CHARACTER_AS_STRING_ORIGINAL[0] = yy_CURRENT_CHARACTER;
		yy_CURRENT_CHARACTER_AS_STRING_ORIGINAL[1] = '\0';
		yyallocated = true;
		ps(yy_CURRENT_CHARACTER_AS_STRING);
	}
	yy_bits_read++;
	return readbin(yy_CURRENT_CHARACTER_AS_STRING, 1, 1)[0];
}

char unget_char() {
// 	pi(yy_FILE_string_pos)
// 	pi(yy_bits_read)
	yy_FILE_string_pos--;
	yy_bits_read--;
	yy_CURRENT_CHARACTER_AS_STRING[0] = yy_CURRENT_CHARACTER_AS_STRING_ORIGINAL[0];
	yy_CURRENT_CHARACTER_AS_STRING[1] = yy_CURRENT_CHARACTER_AS_STRING_ORIGINAL[1];
	for(int i = 0; i < yy_bits_read; i++)
		*yy_CURRENT_CHARACTER_AS_STRING = *yy_CURRENT_CHARACTER_AS_STRING << 0x1;
	return 0;
}

int getcharf(void) {
	return get_char();
}
int ungetcf(void) {
	return unget_char();
}

struct b {
	int16_t word_len_16;
} * data;

 
int main(void) {
	char * string = NULL;
	size_t size = 0;
	read__("../vcpu/main.c.bin", &string, &size);
	data = string;
	regexEngine_Debug = true;
	uint16_t zero = 0b0100111001010110;
// 	pi((0b0100111001010110 >> 15) & 0x1)
// 	pi((0b0100111001010110 >> 14) & 0x1)
// 	ps(regex_readbin(&zero, 16, 1));
	regexEngine_max_bit_length = 16;
	regexEngine_bit_start = 15;
	pi(regexEngineBinary("15-0,0100111001010110", zero));
// 	ifregexEngineBinary("15-0, 0100111001010110", data[0].word_len_16) puts("true");
// 	else puts("false");
// 	ps(readbin(&data[0].word_len_16, 16));
// 	ps(readbin(&data[1].word_len_16, 16));
// 	int16_t a = 0b0100111001010110;
// 	pi(0b0100111001010110==0b0100111001010110)
// 	pi(a==0b0100111001010110)
// 	pi(a==a)
// 	pi(((0b0100111001010110>>15)&0x1))
// 	pi(((0b0100111001010110>>14)&0x1))
// 	pi(((0b0100111001010110>>13)&0x1))
	/*
	puts("setting *string");
	yyfrom_FILE = true;
	yy_FILE_string = "a";
	char ch = 0;
	pc(getcharf())
	pc(getcharf())
	pc(getcharf())
	pc(getcharf())
	pc(getcharf())
	pc(getcharf())
	pc(getcharf())
	pc(getcharf())
	pc(ungetcf())
	pc(ungetcf())
	pc(getcharf())
	pc(getcharf())
	pc(getcharf())
	*/
// 	pi(getBit('a', 7))
// 	read__string_undo(STREAM);
// 	struct binbuf BINARYSTREAM = binbuf_init(BINMODE_APPEND);
// 	read__binary64(&binstr2, bits64);
// 	binstr1.data->s64 = malloc(sizeof(uint64_t));
// 	binstr1.data->s64[0] = (uint64_t) INT16_MAX;
// 	pz(strlen(bits64));
// 	pui64(binstr1.data->s64[0])
// 	pui64(INT16_MAX)
// 	pui64(INT8_MAX)
// 	pui64(UINT64_MAX)
// 
// 	struct binbuf binstr2 = binbuf_init(BINMODE_APPEND);
// 	read__string64(binstr2.data, 64, false, ENDIAN_LITTLE);
// 	pui64(binstr2.data->s64[0])
// 	ps(bits64);
// 	pz(strlen(bits64));
// 	pui64(INT16_MAX)
// 	pui64(INT8_MAX)
// 	pui64(UINT64_MAX)
	
	return 0;
}
