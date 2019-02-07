// #include <gc.h>
#include "cpu_language.h"
void init_table_bank(char *** table_instructions, char *** table_types, char *** table_registers, char *** table_encoding, struct instruction ** instruction_bank) {
    *table_types = env__new();
    *table_registers = env__new();
    *table_encoding = env__new();
    *table_instructions = env__new();
    
    // instructions
	add_table_instructions(
        table_instructions, instruction_bank,
        "NOP", ET_BINARY, "0000 0000", // binary can contain an arbitary nmber of spaces or tabs
        NULL,
		NULL,

        "add3", ET_BINARY, "0000 0001",
        "a, b, c", // if this step is omitted all registers in the operations shall become IMPLICIT
        "a = (b * c) + c; a += (c*c)*a",

        "mov", ET_BINARY, "0000 0010",
        "register_r1, register_r2", // if this step is omitted all registers in the operations shall become IMPLICIT
        "register_r2 = register_r1",
		
        "JCN", ET_BINARY, "0000 0011",
		NULL,
		NULL
	);
    add_table(
        table_types,
        "type_register", 0x1,
        "type_number", 0x2,
        "type_hex", 0x3
	);
   	add_table(
        table_registers,
        "r1", 0x1,
        "r2", 0x2,
        "r3", 0x3,
        "pc", 0x4
	);
    *table_encoding = env__append_env(*table_encoding, *table_instructions);
    *table_encoding = env__append_env(*table_encoding, *table_types);
    *table_encoding = env__append_env(*table_encoding, *table_registers);
}

int main()
{
    char ** table_instructions;
    char ** table_types;
    char ** table_registers;
    char ** table_encoding;
    instruction_new_bank(test);
	
    init_table_bank(&table_instructions, &table_types, &table_registers, &table_encoding, &test);
	
    parse(table_encoding, "NOP");
    parse(table_encoding, "add3 %r1, %r1, %r1");
    parse(table_encoding, "mov 1, %r1");
    parse(table_encoding, "JCN");
	
    opcode_structure_print_all();
	
    puts("writing from opcode_structure to testcpuwrite");
    opcode_structure_write("testcpuwrite", "w");
    puts("reading from testcpuwrite into opcode_structure");
    opcode_structure_read("testcpuwrite");
	system("xxd -b -c7 ./testcpuwrite");
    opcode_structure_print_all();
    cpu_decode(table_encoding, table_types, table_registers, test, 0);
    cpu_decode(table_encoding, table_types, table_registers, test, 1);
    cpu_decode(table_encoding, table_types, table_registers, test, 2);
	
    opcode_structure_clear_all();
    denit_table_bank(&table_instructions, &table_types, &table_registers, &table_encoding, &test);
    return 0;
}
