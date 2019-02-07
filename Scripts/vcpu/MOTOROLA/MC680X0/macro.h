#define BYTE 1
#define KILOBYTE (BYTE*1024)
#define MEGABYTE (KILOBYTE*1024)
#define GIGABYTE (MEGABYTE*1024)

#define SIGN_EXTEND(to, from, value) ( ( ( (value) << (to - from) ) >> (to - from) ) )	
#define ZERO_EXTEND(to, from, value) ( ( (unsigned)( (value) << (to - from) ) >> (to - from) ) )

#define CPU_PRINTF(...) printf("CPU: " __VA_ARGS__)

#define WORD_TO_LONG(upper, lower) (int32_t) upper << 16 | lower
#define LONG_UPPER_TO_WORD(L) (int16_t) L
#define LONG_LOWER_TO_WORD(L) (int16_t) (L >> 16)

#define PRINT_ADDRESS(x) CPU_PRINTF("Address of value %s: 0x%08x\n", #x, x);

#define LINK_OPERATION_WORD(REGISTER) CPU_PRINTF("Pushing value %d from Address Register %s to RAM address 0x%x\n", cpu->core[0].current.virtual_register.REGISTER, #REGISTER, cpu->core[0].current.virtual_register.A7); RAM.RAM[cpu->core[0].current.virtual_register.A7++] = cpu->core[0].current.virtual_register.REGISTER; CPU_PRINTF("Loading the updated stack pointer (Register A7) into Register %s\n", #REGISTER); cpu->core[0].current.virtual_register.REGISTER = cpu->core[0].current.virtual_register.A7; int32_t LONG = SIGN_EXTEND(32, 16, ROM.ROM.data16[++cpu->core[0].current.virtual_register.PC]); CPU_PRINTF("Adding displacement value %li to stack pointer (Register A7)\n", LONG); cpu->core[0].current.virtual_register.A7 += LONG;

#define LINK_OPERATION_LONG(REGISTER) CPU_PRINTF("Pushing value %d from Address Register %s to RAM address 0x%x\n", cpu->core[0].current.virtual_register.REGISTER, #REGISTER, cpu->core[0].current.virtual_register.A7); RAM.RAM[cpu->core[0].current.virtual_register.A7++] = cpu->core[0].current.virtual_register.REGISTER; CPU_PRINTF("Loading the updated stack pointer (Register A7) into Register %s\n", #REGISTER); cpu->core[0].current.virtual_register.REGISTER = cpu->core[0].current.virtual_register.A7; int32_t LONG = WORD_TO_LONG(ROM.ROM.data16[++cpu->core[0].current.virtual_register.PC], ROM.ROM.data16[++cpu->core[0].current.virtual_register.PC]); CPU_PRINTF("Adding displacement value %li to stack pointer (Register A7)\n", LONG); cpu->core[0].current.virtual_register.A7 += LONG;
