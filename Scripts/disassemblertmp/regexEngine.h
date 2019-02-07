/*
this shall accept input in the notation of the reg_gen
syntax: from(-to),to_match,...
to do so we will use the existing code from reg_gen and modify it
*/
#ifndef shell__builtins
#include "../Shell/builtins/printfmacro.h"
#include "../Shell/builtins/regex_str.h"
#endif

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int regexEngine_max_bit_length = 0;
int regexEngine_bit_start = 0;
bool regexEngine_Debug = false;
int regexEngine_Total_Calls = 0;

// regexEngineBinary
#define REGEX_MAX_BIT_SIZE_SUPPORTED uint16_t /*uint64_t*/

#define ifregexEngine(x, y) if (!regexEngine(x,y))

#define inv(swap) swap = swap^1;

int regexEngine(char * fmt, char * string) {
	regexEngine_Total_Calls++;
	if (regexEngine_Debug) pi(regexEngine_max_bit_length)
	if (regexEngine_Debug) pi(regexEngine_bit_start)
	if (regexEngine_Debug) printf("string to parse = %s\n", fmt);
	int limit = regexEngine_max_bit_length*2;
	int limit_multiplierrange0 = 1;
	int limit_multiplierrange1 = 1;
	int limit_multiplierreg = 1;
	str_malloc(range0, limit*limit_multiplierrange0)
	range0.len = 0;
	range0.index = 0;
	str_malloc(range1, limit*limit_multiplierrange1)
	range1.len = 0;
	range1.index = 0;
	str_malloc(reg, limit*limit_multiplierreg)
	reg.len = 0;
	reg.index = 0;
	bool swap_next=0, range=0, range_next=0, is_range=1, is_regex=0, is_not = false, has_look = false;
	int offset = regexEngine_max_bit_length-regexEngine_bit_start;
	int regexEngine_bit_end = regexEngine_max_bit_length-offset, index=offset, add = 0, skip = 0, ii = 0, matches = -1, iitmp = 0, reglen = 0, number_of_matches = 0, number_to_pass = 0;
	while (fmt && *fmt) {
		
		// handle comma
		if(swap_next) {
			inv(swap_next)
			if (is_range) number_to_pass++;
			if (is_regex) inv(range)
			inv(is_regex)
			inv(is_range)
		}
		if (*fmt == ',' ) {
			inv(swap_next)
			inv(range_next)
		}
		
		// handle range
		if (range_next) {
			inv(range_next)
			inv(range)
		}
		if (*fmt == '-' && is_range) inv(range_next)
		
		// parse string
		if(is_range && !range_next && !swap_next) {
			if (!range){
				if (range0.index == limit*limit_multiplierrange0) {
					if (regexEngine_Debug) puts("reallocating");
					limit_multiplierrange0++;
					str_realloc(range0, limit*limit_multiplierrange0);
					if (regexEngine_Debug) puts("reallocating complete");
				}
				range0.string[range0.index] = *fmt;
				range0.index++;
			}else{
				if (range1.index == limit*limit_multiplierrange1) {
					if (regexEngine_Debug) puts("reallocating");
					limit_multiplierrange1++;
					str_realloc(range1, limit*limit_multiplierrange1);
					if (regexEngine_Debug) puts("reallocating complete");
				}
				range1.string[range1.index] = *fmt;
				range1.index++;
			}
		}
		if(is_regex && !swap_next) {
			// if regex contains a not, we go the easy way and replace it with a !
			if (skip) skip--;
			if (*(fmt) == 'n' && *(fmt+1) == 'o' && *(fmt+2) == 't') {
				skip += *(fmt+3)==' '?4:3;
			}
			if (skip) {
				if (skip == 1) {
					if (reg.index == limit*limit_multiplierreg) {
						if (regexEngine_Debug) puts("reallocating");
						limit_multiplierreg++;
						str_realloc(reg, limit*limit_multiplierreg);
						if (regexEngine_Debug) puts("reallocating complete");
					}
					reg.string[reg.index] = '!';
					reg.index++;
				}
			}
			else {
				if (reg.index == limit*limit_multiplierreg) {
					if (regexEngine_Debug) puts("reallocating");
					limit_multiplierreg++;
					str_realloc(reg, limit*limit_multiplierreg);
					if (regexEngine_Debug) puts("reallocating complete");
				}
				reg.string[reg.index] = *fmt=='x'||*fmt=='-'?'.':*fmt;
				reg.index++;
			} // if regex contains 'x' or '-' replace with '.'
		}
		*fmt++;
		
		
		if ((is_regex && swap_next) || (!*fmt)) {
			is_not = false;
			if (reg.string[0] == '!') is_not = true;
			int r0 = atoi(range0.string);
			int r1 = range1.string[0] == NULL?-1:atoi(range1.string);
			if (r1==-1) r1=r0;
			if (regexEngine_Debug) {
				ps(reg.string)
				ps(range0.string)
				pi(r0)
				ps(range1.string)
				pi(r1)
			}
			
			// to handle the range we must first know what the maximum range is
			
			// then we calculate the offset according to the following rule:
			// if the bit indexes start at zero instead of one, the same must also be applied to the string range
			
			// once we have this offset we correct the ranges by adding it to both ranges, we do this later
			
			if (strstr(reg.string, "!") && !(strstr(reg.string, "!") == reg.string)) {
				has_look = true;
				if (regexEngine_Debug) ps(reg.string)
				if (regexEngine_Debug) pc(reg.string[0])
				if (regexEngine_Debug) ps(strstr(reg.string, "!"))
				if (regexEngine_Debug) pi(index)
				reglen = strlen(strstr(reg.string, "!")-1);
			}
			
			// if index is 0, we insert a ^
			if (r0 != regexEngine_bit_start) { 
				add = (regexEngine_max_bit_length-(index))-r0;
				if (regexEngine_Debug) pi(add)
				if (add) index += add;
			}
			index--;
			if (regexEngine_Debug) pi(index)
			ii=0;
			iitmp=0;
			matches = -1;
			for (int i = r1; i <= r0; i++) {
				if (has_look == true) { // lookahead start
					reg.string += reglen;
					for (int iii = r1; iii <= r0; iii++) {
						if (matches == -1 || matches == 0) {
							if (regexEngine_Debug) {
								pc(string[index+iitmp])
								pc(reg.string[iitmp])
							}
							if (string[index+iitmp] == reg.string[iitmp] || reg.string[iitmp] == '.') matches = 0;
							else {
								matches = 1;
								break;
							}
						}
						if (regexEngine_Debug) pi(matches);
						iitmp++;
					}
					reg.string -= reglen;
					has_look = false;
					matches = reverseBool(matches);
				} // lookahead end
				if (matches == -1 || matches == 0) {
					if (regexEngine_Debug) {
						pi(index+ii)
						pc(string[index+ii])
						pc(reg.string[is_not==true?ii+1:ii])
						pi(string[index+ii] == reg.string[is_not==true?ii+1:ii])
						pi(string[index+ii] == reg.string[is_not==true?ii+1:ii] || reg.string[is_not==true?ii+1:ii] == '.')
						pi(0==0)
					}
					if (regexEngine_Debug) {
						pi(is_not==true?ii+1:ii)
						pc(reg.string[is_not==true?ii+1:ii])
					}
					if (string[index+ii] == reg.string[is_not==true?ii+1:ii] || reg.string[is_not==true?ii+1:ii] == '.') matches = 0;
					else {
						matches = 1;
						break;
					}
				}
				else break;
				if (regexEngine_Debug) pi(matches);
				ii++;
			}
			if (is_not == true) matches = reverseBool(matches);
			if (regexEngine_Debug) {
				pi(matches);
				puts("");
			}
			if (matches == 0) number_of_matches++;
			// reset the strings
			memset(range0.string, 0, limit*limit_multiplierrange0); 
			memset(range1.string, 0, limit*limit_multiplierrange1); 
			memset(reg.string, 0, limit*limit_multiplierreg); 
			range0.index = 0;
			range1.index = 0;
			reg.index = 0;
			
		}
	}
	str_free(range0)
	str_free(range1)
	str_free(reg)
	if (number_of_matches == number_to_pass) return 0;
	return -1;
}
































































// binary regex

int regex_read__(char *file, REGEX_MAX_BIT_SIZE_SUPPORTED **p, size_t *q) {
    int fd;
    size_t len = 0;
    REGEX_MAX_BIT_SIZE_SUPPORTED *o;
    if (!(fd = open(file, O_RDONLY)))
    {
        fprintf(stderr, "open() failure\n");
        return (1);
    }
    len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, 0);
    if (!(o = malloc(len))) {
        fprintf(stderr, "failure to malloc()\n");
    }
    if ((read(fd, o, len)) == -1) {
        fprintf(stderr, "failure to read()\n");
    }
    int cl = close(fd);
    if (cl < 0) {
        fprintf(stderr, "cannot close \"%s\", returned %i\n", file, cl);
        return -1;
    }
    *p = o;
    *q = len;
    return len;
}


struct regex_strStream {
    unsigned int *s4;
    uint8_t *s8;
    uint16_t *s16;
    uint32_t *s32;
    uint64_t *s64;
    int bitpos4;
    int bitpos8;
    int bitpos16;
    int bitpos32;
    int bitpos64;
};

struct regex_strStream * regex_strStream_init(unsigned char * string) {
	struct regex_strStream * str = malloc(sizeof(*str));
	str->s4 = string;
	str->s8 = string;
	str->s16 = string;
	str->s32 = string;
	str->s64 = string;
	str->bitpos4 = 0;
	str->bitpos8 = 0;
	str->bitpos16 = 0;
	str->bitpos32 = 0;
	str->bitpos64 = 0;
	return str;
}


struct regex_binbuf {
	struct regex_strStream * data;
	size_t length4;
	size_t length8;
	size_t length16;
	size_t length32;
	size_t length64;
	int bitcount4;
	uint8_t bitcount8;
	uint16_t bitcount16;
	uint32_t bitcount32;
	uint64_t bitcount64;
};

struct regex_binbuf regex_binbuf_init(void) {
	struct regex_binbuf bin = {0};
	bin.data = regex_strStream_init(NULL);
	bin.bitcount4 = 0;
	bin.bitcount8 = 0;
	bin.bitcount16 = 0;
	bin.bitcount32 = 0;
	bin.bitcount64 = 0;
	return bin;
}

int regex_divtoshift(int x) {
	int n = 0;
	while(x /= 2) n++;
	return n;
}

int regex_getBit(REGEX_MAX_BIT_SIZE_SUPPORTED byte, int position) // position in range 0-7
{
    return (byte >> position) & 0x1;
}

int regex_bitcount = 0;

unsigned char * regex_readbin(REGEX_MAX_BIT_SIZE_SUPPORTED * encoding, int member_len, int member_count) {
	unsigned char * bits = malloc((member_len*member_count)+1);
	int bitmax = member_len-1;
	int ii = bitmax;
	int was_set = 0;
	
	for (int i = 0; i < member_len*member_count; i++)
	{
		bits[i]=regex_getBit(*encoding, ii)==0?'0':'1';
		if (regexEngine_Debug) {
			pi(ii)
			pi(regex_getBit(*encoding, ii))
			pi((*encoding >> ii) & 0x1)
			pi((0b0100111001010110 >> ii) & 0x1)
			pb(*encoding == 0b0100111001010110)
			pc(bits[i])
		}
		ii--;
		regex_bitcount++;
		if (regex_bitcount>bitmax) {
			encoding = (encoding)+0x1;
			regex_bitcount = 0;
			was_set = 1;
			ii = bitmax;
		}
	}
	bits[member_len*member_count] = 0;
	if (was_set == 0 && regex_bitcount<=bitmax) 
		for(int i = 0; i < member_len*member_count; i++)
			*encoding = *encoding << 0x1;
	was_set = 0;
	return bits;
}

#define ifregexEngineBinary(x, y) if (!regexEngineBinary(x,y))

isbit(int anum, int abit, int bnum, int bbit) { return ((anum >> abit) & 1) == ((bnum >> bbit) & 1); }

// int a = 0b011110, b = 0b111; if(isbit(a, 2, b, 0) && isbit(a, 3, b, 1) && isbit(a, 4, b, 2))

int regexEngineBinary(char * fmt, REGEX_MAX_BIT_SIZE_SUPPORTED string) {
	regexEngine_Total_Calls++;
	if (regexEngine_Debug) pi(regexEngine_max_bit_length)
	if (regexEngine_Debug) pi(regexEngine_bit_start)
	if (regexEngine_Debug) printf("string to parse = %s\n", fmt);
	int limit = regexEngine_max_bit_length*2;
	int limit_multiplierrange0 = 1;
	int limit_multiplierrange1 = 1;
	int limit_multiplierreg = 1;
	str_malloc(range0, limit*limit_multiplierrange0)
	range0.len = 0;
	range0.index = 0;
	str_malloc(range1, limit*limit_multiplierrange1)
	range1.len = 0;
	range1.index = 0;
	str_malloc(reg, limit*limit_multiplierreg)
	reg.len = 0;
	reg.index = 0;
	bool swap_next=0, range=0, range_next=0, is_range=1, is_regex=0, is_not = false, has_look = false;
	int offset = regexEngine_max_bit_length-regexEngine_bit_start;
	int regexEngine_bit_end = regexEngine_max_bit_length-offset, index=offset, add = 0, skip = 0, ii = 0, matches = -1, iitmp = 0, reglen = 0, number_of_matches = 0, number_to_pass = 0;
	while (fmt && *fmt) {
		
		// handle comma
		if(swap_next) {
			inv(swap_next)
			if (is_range) number_to_pass++;
			if (is_regex) inv(range)
			inv(is_regex)
			inv(is_range)
		}
		if (*fmt == ',' ) {
			inv(swap_next)
			inv(range_next)
		}
		
		// handle range
		if (range_next) {
			inv(range_next)
			inv(range)
		}
		if (*fmt == '-' && is_range) inv(range_next)
		
		// parse string
		if(is_range && !range_next && !swap_next) {
			if (!range){
				if (range0.index == limit*limit_multiplierrange0) {
					if (regexEngine_Debug) puts("reallocating");
					limit_multiplierrange0++;
					str_realloc(range0, limit*limit_multiplierrange0);
					if (regexEngine_Debug) puts("reallocating complete");
				}
				range0.string[range0.index] = *fmt;
				range0.index++;
			}else{
				if (range1.index == limit*limit_multiplierrange1) {
					if (regexEngine_Debug) puts("reallocating");
					limit_multiplierrange1++;
					str_realloc(range1, limit*limit_multiplierrange1);
					if (regexEngine_Debug) puts("reallocating complete");
				}
				range1.string[range1.index] = *fmt;
				range1.index++;
			}
		}
		if(is_regex && !swap_next) {
			// if regex contains a not, we go the easy way and replace it with a !
			if (skip) skip--;
			if (*(fmt) == 'n' && *(fmt+1) == 'o' && *(fmt+2) == 't') {
				skip += *(fmt+3)==' '?4:3;
			}
			if (skip) {
				if (skip == 1) {
					if (reg.index == limit*limit_multiplierreg) {
						if (regexEngine_Debug) puts("reallocating");
						limit_multiplierreg++;
						str_realloc(reg, limit*limit_multiplierreg);
						if (regexEngine_Debug) puts("reallocating complete");
					}
					reg.string[reg.index] = '!';
					reg.index++;
				}
			}
			else {
				if (reg.index == limit*limit_multiplierreg) {
					if (regexEngine_Debug) puts("reallocating");
					limit_multiplierreg++;
					str_realloc(reg, limit*limit_multiplierreg);
					if (regexEngine_Debug) puts("reallocating complete");
				}
				reg.string[reg.index] = *fmt=='x'||*fmt=='-'?'.':*fmt;
				reg.index++;
			} // if regex contains 'x' or '-' replace with '.'
		}
		*fmt++;
		
		
		if ((is_regex && swap_next) || (!*fmt)) {
			is_not = false;
			if (reg.string[0] == '!') is_not = true;
			int r0 = atoi(range0.string);
			int r1 = range1.string[0] == NULL?-1:atoi(range1.string);
			if (r1==-1) r1=r0;
			if (regexEngine_Debug) {
				ps(reg.string)
				ps(range0.string)
				pi(r0)
				ps(range1.string)
				pi(r1)
			}
			
			// to handle the range we must first know what the maximum range is
			
			// then we calculate the offset according to the following rule:
			// if the bit indexes start at zero instead of one, the same must also be applied to the string range
			
			// once we have this offset we correct the ranges by adding it to both ranges, we do this later
			
			if (strstr(reg.string, "!") && !(strstr(reg.string, "!") == reg.string)) {
				has_look = true;
				if (regexEngine_Debug) ps(reg.string)
				if (regexEngine_Debug) pc(reg.string[0])
				if (regexEngine_Debug) ps(strstr(reg.string, "!"))
				if (regexEngine_Debug) pi(index)
				reglen = strlen(strstr(reg.string, "!")-1);
			}
			
			// if index is 0, we insert a ^
			if (r0 != regexEngine_bit_start) { 
				add = (regexEngine_max_bit_length-(index))-r0;
				if (regexEngine_Debug) pi(add)
				if (add) index += add;
			}
			index--;
			if (regexEngine_Debug) pi(index)
			ii=0;
			iitmp=0;
			matches = -1;
			REGEX_MAX_BIT_SIZE_SUPPORTED raw;
			char * st = str_replace(reg.string, ".", "0");
			if (st) {
				if (regexEngine_Debug) ps(st);
				raw = strtol(st, 0, 2);
				free(st);
			} else {
				raw = strtol(reg.string, 0, 2);
			}
			int acount = r0;
			int acounttmp = r0;
			int bcount = r0-r1;
			int bcounttmp = r0-r1;
			for (int i = r1; i <= r0; i++) {
				if (has_look == true) { // lookahead start
					reg.string += reglen;
					unsigned int rawtmp;
					char * sttmp = str_replace(reg.string, ".", "0");
					if (sttmp) {
						if (regexEngine_Debug) ps(sttmp);
						rawtmp = strtol(sttmp, 0, 2);
						free(sttmp);
					} else {
						rawtmp = strtol(reg.string, 0, 2);
					}
					for (int iii = r1; iii <= r0; iii++) {
						if (matches == -1 || matches == 0) {
							if (regexEngine_Debug) {
								pi(acounttmp)
								pi(((string >> acounttmp) & 0x1))
								pi(bcounttmp)
								pi(((rawtmp >> bcounttmp) & 0x1))
								pb(isbit(string, acounttmp, rawtmp, bcounttmp))
							}
							if (reg.string[iitmp] == '.' || isbit(string, acounttmp, rawtmp, bcounttmp)) matches = 0;
							else {
								matches = 1;
								break;
							}
							acounttmp--;
							bcounttmp--;
						}
						if (regexEngine_Debug) pi(matches);
						iitmp++;
					}
					reg.string -= reglen;
					has_look = false;
					matches = reverseBool(matches);
				} // lookahead end
				if (regexEngine_Debug) pi(matches)
				if (matches == -1 || matches == 0) {
					if (regexEngine_Debug) {
						pi(acount)
						pi(((string >> acount) & 0x1))
						pi(bcount)
						pi(((raw >> bcount) & 0x1))
						pb(isbit(string, acount, raw, bcount))
					}
					if (regexEngine_Debug) {
						pi(is_not==true?ii+1:ii)
						pc(reg.string[is_not==true?ii+1:ii])
					}
					if (reg.string[is_not==true?ii+1:ii] == '.' || isbit(string, acount, raw, bcount)) matches = 0;
					else {
						matches = 1;
						break;
					}
					acount--;
					bcount--;
				}
				else break;
				if (regexEngine_Debug) pi(matches);
				ii++;
			}
			if (is_not == true) matches = reverseBool(matches);
			if (regexEngine_Debug) {
				pi(matches);
				puts("");
			}
			if (matches == 0) number_of_matches++;
			// reset the strings
			memset(range0.string, 0, limit*limit_multiplierrange0); 
			memset(range1.string, 0, limit*limit_multiplierrange1); 
			memset(reg.string, 0, limit*limit_multiplierreg); 
			range0.index = 0;
			range1.index = 0;
			reg.index = 0;
			
		}
	}
	str_free(range0)
	str_free(range1)
	str_free(reg)
	if (regexEngine_Debug) {
		pi(number_of_matches)
		pi(number_to_pass)
	}
	if (number_of_matches == number_to_pass) return 0;
	return -1;
}
