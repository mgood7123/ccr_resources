#include "../builtins/libunicode.h"
#include "../builtins/ucd.h"

struct unicode {
	int bytes;
	int codepoint_bits;
} unicode;

char * detect(char * string) {
	ps(string);
	bstate * c = bopen(string);
	int i = 7;
	char * bits = NULL;
	bits = bread(c, 8, &i);
	if (bits[0] == '0') {
		unicode.bytes = 1;
		unicode.codepoint_bits = 7;
	}
	else
	if (bits[0] == '1') {
		if (bits[1] == '1') {
			if (bits[2] == '0') {
				unicode.bytes = 2;
				unicode.codepoint_bits = 11;
			}
			else
			if (bits[2] == '1') {
				if (bits[3] == '0') {
					unicode.bytes = 3;
					unicode.codepoint_bits = 16;
				}
				else
				if (bits[3] == '1') {
					if (bits[4] == '0') {
						unicode.bytes = 4;
						unicode.codepoint_bits = 21;
					}
				}
			}
		}
	}
	bclose(c);
	pi(unicode.bytes)
	pi(unicode.codepoint_bits)
	bstate * tmp = bopen(string);
	int tmpbits = 7;
	for (int i = 0; i < unicode.bytes; i++) {
		char * binary = NULL;
		binary = bread(tmp, 8, &tmpbits);
		ps(binary);
		free(binary);
		binary = NULL;
	}
	bclose(tmp);
	return string;
}

void main(void) {
// 	detect("a");
// 	detect("$");
// 	detect("¢");
// 	detect("ह");
// 	detect("€");
// 	detect("𐍈");
// 	int ii = 7, iii = 7;
// 	puts("opening binary state");
// 	bstate * c = bopen("€");
// 	pp(c);
// 	ps(c->data);
// 	ps(bread(c, 8, &iii));
// 	ps(bread(c, 8, &iii));
// 	ps(bread(c, 8, &iii));
// 	bclose(c);
// 	bstate * b = bopen("€");
// 	ps(bread(b, 1, &ii));
// 	ps(bread(b, 1, &ii));
// 	ps(bread(b, 1, &ii));
// 	ps(bread(b, 1, &ii));
// 	ps(bread(b, 1, &ii));
// 	ps(bread(b, 1, &ii));
// 	ps(bread(b, 1, &ii));
// 	ps(bread(b, 1, &ii));
// 	bclose(b);
// 	int i = 7;
// 	bstate * d___ = bopen("€");
// 	char * d__ = bread(d___, 8*3, &i);
// 	ps(d__);
// 	char * d = bwrite(d__);
// 	bclose(d__);
// 	ps(d);
// 	i = 7;
// 	pp(d);
// 	bstate * d____ = bopen(d);
// 	char * d_ = bread(d____, 8*3, &i);
// 	pp(d);
// 	ps(d_);
// 	free(d);
// 	free(d_);
// 	bclose(d____);
	
// 	ucd();
	codepoint_t c;
	for (c = 0; c <= 0x10FFFF; ++c) {
		printf("string as hex: 0x%x\n", c);
		printf("string as unicode: ");
		fput_utf8c(stdout, c);
		printf("\n");
		free(detect(fput_utf8c_ret(c)));
	}

}
