/*
PEA

Description: Computes the effective address and pushes it onto the stack. The effective address is a long address.
*/
ifregexEngineBinary("15-6,0100100001", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) {
	ifregexEngineBinary("5-3,111", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) {
		ifregexEngineBinary("2-0,001", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) {
			// addressing mode:    2.2.17 Absolute Long Addressing Mode
			PRINT_ADDRESS(WORD_TO_LONG(ROM.ROM.data16[++cpu->core[0].current.virtual_register.PC], ROM.ROM.data16[++cpu->core[0].current.virtual_register.PC]));
		}
	}

}
