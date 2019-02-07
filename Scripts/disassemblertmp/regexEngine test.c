
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
	char * str = "11110010000000000000000000010001";
	ps(str)
	pi(strlen(str))
	pz(UINT32_MAX/512)
	uint64_t passes = 0, itterations = UINT32_MAX/512;
	pz(itterations);
	timefunc(
		dothis(
			ifregexEngine("31-28,1111 not 1011,27-25,00x,4,-",str) passes++;
			, itterations
		)
	);
/*
itterations = 8388607
cpu time: 17.025 seconds
passes = 8388607
*/
	pz(passes);
	pb(passes==itterations);
	pi(regexEngine("27-25,..1", str))
	pi(regexEngine("31-28,1..1", str))
	pi(regexEngine("31-28,1111 not 1011,27-25,00x,4,-", str))
	pi(regexEngine("4,-", str))
	pi(regexEngine("31-28,1..1", str))
	pi(regexEngine("31-28,1..1 not 1.34", str))
	pi(regexEngine("31-28,1..1 not 1.24", str))
	pi(regexEngine("31-28,1..1 not 1324", str))
	pi(regexEngine("31-28,1..1 not 1101", str))
	pi(regexEngine("31-0,11110010000000000000000000010001", str))
	pi(regexEngine("27-25,..1", str))
	pi(regexEngine("31-28,1..1", str))
	pi(regexEngine("31-28,1111 not 1011,27-25,00x,4,-", str))
	pi(regexEngine("4,-", str))
	pi(regexEngine("31-28,1..1", str))
	pi(regexEngine("31-28,1..1 not 1.34", str))
	pi(regexEngine("31-28,1..1 not 1.24", str))
	pi(regexEngine("31-28,1..1 not 1324", str))
	pi(regexEngine("31-28,1..1 not 1001", str))
	pi(regexEngine("31-0,11110010000000000000000000010001 not 10110010000000000000000000010000", str))
	return 0;
}

