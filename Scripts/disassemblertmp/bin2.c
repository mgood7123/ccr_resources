// #include "disassembler.h"
#include "binary.h"
#include "regexEngine.h"
#include "../Shell/builtins/printfmacro.h"
#include "../Shell/builtins/libstring.h"
#include "../Shell/builtins/regex_str.h"
/*
read__binary(&binstr, "xx0x0 not 0x010");
*/
void and(char * bin) {
	struct binbuf binstr1 = binbuf_init(BINMODE_APPEND);
	read__binary(&binstr1, bin);
	ps(bin)
	pi(regexEngineb(5, 5, "4-0,xx0x0 not 0x010", binstr1.data->s))
}

void main(void) {
	and("00000");
	and("00001");
	and("00010");
	and("00011");
	and("00100");
	and("00101");
	and("00110");
	and("00111");
	and("01000");
	and("01001");
	and("01010");
	and("01011");
	and("01100");
	and("01101");
	and("01110");
	and("01111");
	and("10000");
	and("10001");
	and("10010");
	and("10011");
	and("10100");
	and("10101");
	and("10110");
	and("10111");
	and("11000");
	and("11001");
	and("11010");
	and("11011");
	and("11100");
	and("11101");
	and("11110");
	and("11111");
// 	struct binbuf binstr1 = binbuf_init(BINMODE_APPEND);
// 	read__binary(&binstr1, "10110010000000000000000000010000");
// 	char * str = read__string(binstr1.data, 32, false);
// 	ps(str)
}
