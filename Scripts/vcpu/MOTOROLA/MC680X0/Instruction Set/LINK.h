/*
LINK
Description:
Pushes the contents of the specified address register onto the stack. Then loads the updated stack pointer into the address register. Finally, adds the displacement value to the stack pointer. For word-size operation, the displacement is the sign-extended word following the operation word. For long size operation, the displacement is the long word following the operation word. The address register occupies one long word on the stack. The user should specify a negative displacement in order to allocate stack area.
*/
// LINK word
ifregexEngineBinary("15-3,0100111001010", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) {
		 ifregexEngineBinary("2-0,000", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_WORD(A0); }
	else ifregexEngineBinary("2-0,001", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_WORD(A1); }
	else ifregexEngineBinary("2-0,010", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_WORD(A2); }
	else ifregexEngineBinary("2-0,011", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_WORD(A3); }
	else ifregexEngineBinary("2-0,100", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_WORD(A4); }
	else ifregexEngineBinary("2-0,101", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_WORD(A5); }
	else ifregexEngineBinary("2-0,110", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_WORD(A6); }
	else ifregexEngineBinary("2-0,111", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_WORD(A7); }
}

// LINK long
ifregexEngineBinary("15-3,0100111001010", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) {
		 ifregexEngineBinary("2-0,000", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_LONG(A0); }
	else ifregexEngineBinary("2-0,001", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_LONG(A1); }
	else ifregexEngineBinary("2-0,010", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_LONG(A2); }
	else ifregexEngineBinary("2-0,011", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_LONG(A3); }
	else ifregexEngineBinary("2-0,100", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_LONG(A4); }
	else ifregexEngineBinary("2-0,101", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_LONG(A5); }
	else ifregexEngineBinary("2-0,110", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_LONG(A6); }
	else ifregexEngineBinary("2-0,111", ROM.ROM.data16[cpu->core[0].current.virtual_register.PC]) { LINK_OPERATION_LONG(A7); }
}
