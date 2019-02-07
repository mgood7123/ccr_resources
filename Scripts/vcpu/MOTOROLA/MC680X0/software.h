		regexEngine_max_bit_length = 16;
		regexEngine_bit_start = 15;
		ps(regex_readbin(&ROM.ROM.data16[cpu->core[0].current.virtual_register.PC], 16, 1));
		
		
		
		// disassembler
		
		/*
		 * stack pointer:
		 * 
		 * look at 2.6.1 System Stack, which refers to the following two sections in section 1
		 * 
		 * 1.3.1 Address Register 7 (A7) in 1.3 SUPERVISOR PROGRAMMING MODEL
		 * 1.1.2 Address Registers (A7 – A0) in 1.1 INTEGER UNIT USER PROGRAMMING MODEL
		 * 
		 * stack is suspected to be held in RAM
		 */
#include "Instruction Set/LINK.h"
#include "Instruction Set/PEA.h"
		puts("");
