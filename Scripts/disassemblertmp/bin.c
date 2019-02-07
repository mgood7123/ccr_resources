// #include "disassembler.h"
#include "binary.h"
#include "regexEngine.h"

#define SIGN_EXTEND(to, from, value) ( ( ( (value) << (to - from) ) >> (to - from) ) )

unsigned int bit(int num) {
	struct binbuf binstr1 = binbuf_init(BINMODE_APPEND);
	int num1 = num;
	while(num) {
		read__binary(&binstr1, "1");
		num--;
	}
	while(num1<8) {
		read__binary(&binstr1, "0");
		num1++;
	}
	return binstr1.data->s[0];
}

uint64_t bit64(int num) {
	struct binbuf binstr1 = binbuf_init(BINMODE_APPEND);
	int num1 = num;
	while(num) {
		read__binary64(&binstr1, "1");
		num--;
	}
	while(num1<64) {
		read__binary64(&binstr1, "0");
		num1++;
	}
	return binstr1.data->s64[0];
}

int main(void) {
	struct binbuf binstr1 = binbuf_init(BINMODE_APPEND);
	binstr1.data->s64 = malloc(sizeof(uint64_t));
	binstr1.data->s64[0] = (uint64_t) INT16_MAX;
	read__string64(binstr1.data, 64, false);
	ps(bits64);
	pz(strlen(bits64));
	pui64(binstr1.data->s64[0])
	pui64(INT16_MAX)
	pui64(INT8_MAX)
	pui64(UINT64_MAX)

	struct binbuf binstr2 = binbuf_init(BINMODE_APPEND);
	read__binary64(&binstr2, bits64);
	read__string64(binstr2.data, 64, false);
	pui64(binstr2.data->s64[0])
	ps(bits64);
	pz(strlen(bits64));
	pui64(INT16_MAX)
	pui64(INT8_MAX)
	pui64(UINT64_MAX)
	
	pui64(bit(0))
	pui64(bit(1))
	pui64(bit(2))
	pui64(bit(3))
	pui64(bit(4))
	pui64(bit(5))
	pui64(bit(6))
	pui64(bit(7))
	pui64(bit(8))
	pui64(bit(9))
	pui64(bit(10))
	pui64(bit(11))
	pui64(bit(12))
	pui64(bit(13))
	pui64(bit(14))
	pui64(bit(15))
	pui64(bit(16))
	pui64(bit(17))
	pui64(bit(18))

	pui64(bit64(0))
	pui64(bit64(1))
	pui64(bit64(2))
	pui64(bit64(3))
	pui64(bit64(4))
	pui64(bit64(5))
	pui64(bit64(6))
	pui64(bit64(7))
	pui64(bit64(8))
	pui64(bit64(9))
	pui64(bit64(10))
	pui64(bit64(11))
	pui64(bit64(12))
	pui64(bit64(13))
	pui64(bit64(14))
	pui64(bit64(15))
	pui64(bit64(16))
	pui64(bit64(17))
	pui64(bit64(18))

	return 0;
}
