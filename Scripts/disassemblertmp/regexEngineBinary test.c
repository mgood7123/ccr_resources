
#include "regexEngine.h"
#include "../Shell/builtins/clock.h"
#define dothis(what, times) {\
	for(int i = 0;i<times;i++) { what }; \
}

int main(void)
{
	regexEngine_max_bit_length = 32;
	regexEngine_bit_start = 31;
	regexEngine_Debug = false;
	int32_t str = 0b11110010000000000000000000010001;
	pz(UINT32_MAX/512)
	uint64_t passes, itterations = UINT32_MAX/512;
	pz(itterations);
	passes = 0;
	timefunc(
		dothis(
			, itterations
		)
	);
	passes = 0;
	timefunc(
		dothis(
			ifregexEngineBinary("31-28,1111 not 1011,27-25,00x,4,-",str) passes++;
			, itterations
		)
	);
	pz(passes);
	pb(passes==itterations);
/*
itterations = 8388607
cpu time: 19.464 seconds
passes = 8388607
*/
	pi(regexEngineBinary("27-25,..1", str))
	pi(regexEngineBinary("31-28,1..1", str))
	pi(regexEngineBinary("31-28,1111 not 1011,27-25,00x,4,-", str))
	pi(regexEngineBinary("4,-", str))
	pi(regexEngineBinary("31-28,1..1", str))
	pi(regexEngineBinary("31-28,1..1 not 1.34", str))
	pi(regexEngineBinary("31-28,1..1 not 1.24", str))
	pi(regexEngineBinary("31-28,1..1 not 1324", str))
	pi(regexEngineBinary("31-28,1..1 not 1101", str))
	pi(regexEngineBinary("31-0,11110010000000000000000000010001", str))
	pi(regexEngineBinary("27-25,..1", str))
	pi(regexEngineBinary("31-28,1..1", str))
	pi(regexEngineBinary("31-28,1111 not 1011,27-25,00x,4,-", str))
	pi(regexEngineBinary("4,-", str))
	pi(regexEngineBinary("31-28,1..1", str))
	pi(regexEngineBinary("31-28,1..1 not 1.34", str))
	pi(regexEngineBinary("31-28,1..1 not 1.24", str))
	pi(regexEngineBinary("31-28,1..1 not 1324", str))
	pi(regexEngineBinary("31-28,1..1 not 1001", str))
	pi(regexEngineBinary("31-0,11110010000000000000000000010001 not 11110010000000000000000000010000", str))
	return 0;
}

