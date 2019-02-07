#include "../../varg.h"
// #include "cpu_language.h"
#include "../../../Shell/builtins/env.h"
#include "../../../Shell/builtins/printfmacro.h"
#include "../../../Shell/builtins/libstring.h"
#include "../../../disassemblertmp/regexEngine.h"
#define getBit regex_getBit
#include "../../../disassemblertmp/encoding.h"
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define GENERAL_PURPOSE_REGISTER_SIZE int32_t
#define PROGRAM_COUNTER_REGISTER_SIZE int32_t
#define CCR_COUNTER_REGISTER_SIZE 5


#define CPU_POWER_ON true
#define CPU_POWER_OFF false

#define null_int(x) x = 0
#define set_value(x, y) x = y

struct virtual_cpu_register_bank {
	
	// DATA REGISTERS
    GENERAL_PURPOSE_REGISTER_SIZE D0;
    GENERAL_PURPOSE_REGISTER_SIZE D1;
    GENERAL_PURPOSE_REGISTER_SIZE D2;
    GENERAL_PURPOSE_REGISTER_SIZE D3;
    GENERAL_PURPOSE_REGISTER_SIZE D4;
    GENERAL_PURPOSE_REGISTER_SIZE D5;
    GENERAL_PURPOSE_REGISTER_SIZE D6;
    GENERAL_PURPOSE_REGISTER_SIZE D7;
	
	// ADDRESS REGISTERS
    GENERAL_PURPOSE_REGISTER_SIZE A0;
    GENERAL_PURPOSE_REGISTER_SIZE A1;
    GENERAL_PURPOSE_REGISTER_SIZE A2;
    GENERAL_PURPOSE_REGISTER_SIZE A3;
    GENERAL_PURPOSE_REGISTER_SIZE A4;
    GENERAL_PURPOSE_REGISTER_SIZE A5;
    GENERAL_PURPOSE_REGISTER_SIZE A6;

	// USER STACK POINTER
	GENERAL_PURPOSE_REGISTER_SIZE A7;
	
	// PROGRAM COUNTER
	PROGRAM_COUNTER_REGISTER_SIZE PC;
	
	// CONDITION CODE REGISTER
	int CCR:CCR_COUNTER_REGISTER_SIZE;
	
	// CPU META INFO
    char * name;
};

struct virtual_cpu_sub_core {
    struct virtual_cpu_register_bank virtual_register;

	// CPU META INFO
    int op;
    int status;
    char * name;
};

struct virtual_cpu_core {
    struct virtual_cpu_sub_core previous;
    struct virtual_cpu_sub_core current;

	// CPU META INFO
	int status;
    int power;
    char * cpu_power_state[2];
    char * name;
    bool init;
};

struct virtual_cpu {
    int core_count;
    struct virtual_cpu_core *core;

	// CPU META INFO
	char * name;
} cpu_default;

void virtual_cpu_check(struct virtual_cpu *cpu);
void cpu_register_info(struct virtual_cpu *cpu, struct virtual_cpu_core *core, struct virtual_cpu_sub_core *sub_core, struct virtual_cpu_register_bank *virtual_register);
void cpu_sub_core_info(struct virtual_cpu *cpu, struct virtual_cpu_core *core, struct virtual_cpu_sub_core *sub_core);
void cpu_core_info(struct virtual_cpu *cpu, struct virtual_cpu_core *core);
void cpu_info(struct virtual_cpu *cpu);
void cpu_core_start(struct virtual_cpu_core *core);
void cpu_start(struct virtual_cpu *cpu);
void cpu_core_shutdown(struct virtual_cpu_core *core);
void cpu_shutdown(struct virtual_cpu *cpu);
void virtual_cpu_register_check(struct virtual_cpu_register_bank *reg, char * name);
void virtual_cpu_sub_core_check(struct virtual_cpu_sub_core * core, char * name);
void virtual_cpu_core_check(struct virtual_cpu_core *core, char * name);
// void cpu_execute(struct virtual_cpu *cpu);
void cpu_deinit(struct virtual_cpu *cpu);
void cpu_core_init(struct virtual_cpu *cpu, int max_cores);

#define CPU_REGISTER_PRINT(x)     printf("VIRTUAL CPU: INFO : %s : %s : %s : %s : %s: %d\n", cpu->name, core->name, sub_core->name, virtual_register->name, #x, virtual_register->x);

void cpu_register_info(struct virtual_cpu *cpu, struct virtual_cpu_core *core, struct virtual_cpu_sub_core *sub_core, struct virtual_cpu_register_bank *virtual_register) {
    printf("VIRTUAL CPU: INFO : %s : %s : %s : register bank name: %s\n", cpu->name, core->name, sub_core->name, virtual_register->name);
    printf("VIRTUAL CPU: INFO : %s : %s : %s : size of %s: %zu\n", cpu->name, core->name, sub_core->name, virtual_register->name, sizeof(*virtual_register));
	CPU_REGISTER_PRINT(D0);
	CPU_REGISTER_PRINT(D1);
	CPU_REGISTER_PRINT(D2);
	CPU_REGISTER_PRINT(D3);
	CPU_REGISTER_PRINT(D4);
	CPU_REGISTER_PRINT(D5);
	CPU_REGISTER_PRINT(D6);
	CPU_REGISTER_PRINT(D7);

	CPU_REGISTER_PRINT(A0);
	CPU_REGISTER_PRINT(A1);
	CPU_REGISTER_PRINT(A2);
	CPU_REGISTER_PRINT(A3);
	CPU_REGISTER_PRINT(A4);
	CPU_REGISTER_PRINT(A5);
	CPU_REGISTER_PRINT(A6);
	
	CPU_REGISTER_PRINT(A7);
	
	CPU_REGISTER_PRINT(PC);
	
	CPU_REGISTER_PRINT(CCR);
}

void cpu_sub_core_info(struct virtual_cpu *cpu, struct virtual_cpu_core *core, struct virtual_cpu_sub_core *sub_core) {
    printf("VIRTUAL CPU: INFO : %s : %s : sub core name: %s\n", cpu->name, core->name, sub_core->name);
    printf("VIRTUAL CPU: INFO : %s : %s : size of %s: %zu\n", cpu->name, core->name, sub_core->name, sizeof(*sub_core));
    cpu_register_info(cpu, core, sub_core, &sub_core->virtual_register);
}

void cpu_core_info(struct virtual_cpu *cpu, struct virtual_cpu_core *core) {
    printf("VIRTUAL CPU: INFO : %s : core name:  %s\n", cpu->name, core->name);
    printf("VIRTUAL CPU: INFO : %s : size of %s: %zu\n", cpu->name, core->name, 
           sizeof(*core)-
           sizeof(core->cpu_power_state)+
           (
               sizeof(core->cpu_power_state)*env__size(core->cpu_power_state)
           )
    );
    printf("VIRTUAL CPU: INFO : %s : %s : cpu power status: %s\n", cpu->name, core->name, core->cpu_power_state[core->power]);
    printf("VIRTUAL CPU: INFO : %s : %s : status:           %d\n", cpu->name, core->name, core->status);
    cpu_sub_core_info(cpu, core, &core->previous);
    cpu_sub_core_info(cpu, core, &core->current);
}

void cpu_info(struct virtual_cpu *cpu) {
    virtual_cpu_check(cpu);
    printf("VIRTUAL CPU: INFO : cpu name:         %s\n", cpu->name);
    printf("VIRTUAL CPU: INFO : cpu cores:        %d\n", cpu->core_count);
    printf
    (
        "VIRTUAL CPU: INFO : size of %s: %zu\n", cpu->name,
        (
            sizeof(*cpu)-
            (
                sizeof(cpu->core)
            )+
            (
                (
                    sizeof(cpu->core[0])-
                    sizeof(cpu->core[0].cpu_power_state)+
                    (
                        sizeof(cpu->core[0].cpu_power_state)*env__size(cpu->core[0].cpu_power_state)
                    )
                ) * cpu->core_count
            )
        )
    );
    for (int i = 0; i < 1; i++) cpu_core_info(cpu, &cpu->core[i]);
}

void cpu_core_start(struct virtual_cpu_core *core) {
    printf("VIRTUAL CPU: starting virtual cpu core:      %s\n", core->name);
    core->power = CPU_POWER_ON;
}

void cpu_start(struct virtual_cpu *cpu) {
    puts("VIRTUAL CPU: performing startup check");
    virtual_cpu_check(cpu);
    puts("VIRTUAL CPU: startup check complete");
    printf("VIRTUAL CPU: starting virtual cpu:      %s\n", cpu->name);
    cpu_core_start(&cpu->core[0]);
}

void cpu_core_shutdown(struct virtual_cpu_core *core) {
    printf("VIRTUAL CPU: shutting down virtual cpu core: %s\n", core->name);
    core->power = CPU_POWER_OFF;
}

void cpu_shutdown(struct virtual_cpu *cpu) {
    puts("VIRTUAL CPU: performing shutdown check");
    virtual_cpu_check(cpu);
    puts("VIRTUAL CPU: shutdown check complete");
    printf("VIRTUAL CPU: shutting down virtual cpu:      %s\n", cpu->name);
    cpu_core_shutdown(&cpu->core[0]);
    cpu_deinit(cpu);
}

void virtual_cpu_register_check(struct virtual_cpu_register_bank *reg, char * name) {
    null_int(reg->D0);
    null_int(reg->D1);
    null_int(reg->D2);
    null_int(reg->D3);
    null_int(reg->D4);
    null_int(reg->D5);
    null_int(reg->D6);
    null_int(reg->D7);
	
    null_int(reg->A0);
    null_int(reg->A1);
    null_int(reg->A2);
    null_int(reg->A3);
    null_int(reg->A4);
    null_int(reg->A5);
    null_int(reg->A6);

	null_int(reg->A7);

	null_int(reg->PC);

	null_int(reg->CCR);
	
	set_value(reg->name, name);
}

void virtual_cpu_sub_core_check(struct virtual_cpu_sub_core * core, char * name) {
    virtual_cpu_register_check(&core->virtual_register, "register");
    null_int(core->op);
    null_int(core->status);
    set_value(core->name, name);
}

// a cpu part shall be considered uninitialized if has a value of zero [ 0 ]
void virtual_cpu_core_check(struct virtual_cpu_core *core, char * name) {
    if (core->init == true) return;
    virtual_cpu_sub_core_check(&core->previous, "PREVIOUS");
    virtual_cpu_sub_core_check(&core->current, "CURRENT");
    null_int(core->status);
    null_int(core->power);
    set_value(core->cpu_power_state[0], "OFF");
    set_value(core->cpu_power_state[1], "ON");
    set_value(core->name, name);
    core->init = true;
}

void cpu_core_init(struct virtual_cpu *cpu, int max_cores) {
    if (cpu->core) return;
    cpu->core = malloc(max_cores*sizeof(struct virtual_cpu_core));
    for (int i = 0; i < max_cores; i++) {
        char * name = malloc(100);
        snprintf(name, 100, "Core %d", i);
        virtual_cpu_core_check(&cpu->core[i], name);
    }
    cpu->core_count = max_cores;
}

void cpu_deinit(struct virtual_cpu *cpu) {
    if (!cpu->core) return;
    for (int i = 0; i < cpu->core_count; i++) {
        free(cpu->core[i].name);
    }
    free(cpu->core);
    cpu->core_count = 0;
}

void virtual_cpu_check(struct virtual_cpu *cpu) {
    cpu_core_init(cpu, 32);
    set_value(cpu->name, "MOTOROLA MC680X0");
}




#include "hardware.h"

// cpu shall backup current to previous in order to have a restore point, then take changes from que and commit them to current
void cpu_execute(struct virtual_cpu *cpu, int program_len) {
	if (!cpu->core) {
		puts("error: cpu uninitialized");
		return;
	}
	int instructions = 0;
	for (cpu->core[0].current.virtual_register.PC = 0;  cpu->core[0].current.virtual_register.PC < program_len;  cpu->core[0].current.virtual_register.PC++) {
		// instruction processing loop
		/*
		 * $ sudo debchroot
		 * $ compile_and_extract main.c
		 */
		#include "software.h"
//         cpu->core[0].current.virtual_register.PC++;
        instructions++;
    }
    if (instructions == 0) puts("i have no instruction to execute");
    else puts("i have no instructions left to execute");
}


 
int main(void) {
	system("cat main.c.asm");
	system("xxd -b -c 2 -g 4 main.c.bin");
	INIT_ROM(50*MEGABYTE);
	INIT_RAM(50*MEGABYTE);
	if (read_and_convert("main.c.bin", 16) == -1) return -1;
//     cpu_info(&cpu_default);
    cpu_start(&cpu_default);
	puts("");
    if (RAM.RAM) cpu_execute(&cpu_default, 12);
    cpu_info(&cpu_default);
    cpu_shutdown(&cpu_default);
	free(ROM.ROM.data16);
    return 0;
}
