struct ROM {
	uint32_t SIZE;
	struct ROM_Multi {
		uint8_t * data8;
		uint16_t * data16;
		uint32_t * data32;
	} ROM;
} ROM;

#include "macro.h"

void INIT_ROM(uint32_t size) {
	ROM.SIZE = 0;
	ROM.ROM.data8 = NULL;
	ROM.ROM.data16 = NULL;
	ROM.ROM.data32 = NULL;
	
	if (size == 0) {
		printf("CPU: ERROR: CANNOT ALLOCATE ROM OF SIZE %zu\n", size);
		return;
	}
	ROM.SIZE = size;
	return;
}

int read_and_convert(char * file, int bit_size) {
	char * memory = NULL;
	ssize_t size = 0;
	if (read__(file, &memory, &size) == -1) return -1;
	
	assert((size % CHAR_BIT) == 0);
	
	// swap bytes
	char * memory_swapped = malloc(size);
	if (bit_size == 8) {
		for (int i = 0; i < size; i++) memory_swapped[i] = memory[i];
		free(memory);
		ROM.ROM.data8 = memory_swapped;
		return 0;
	} else if (bit_size == 16) {
		for (int i = 0; i < size; i++) {
			int one = i;
			int two = i+1;
			memory_swapped[two] = memory[i++];
			memory_swapped[one] = memory[i];
		}
		free(memory);
		ROM.ROM.data16 = memory_swapped;
		return 0;
	} else if (bit_size == 32) {
		for (int i = 0; i < size; i++) {
			int one = i;
			int two = i+1;
			int three = i+2;
			int four = i+3;
			memory_swapped[four] = memory[i++];
			memory_swapped[three] = memory[i++];
			memory_swapped[two] = memory[i++];
			memory_swapped[one] = memory[i];
		}
		free(memory);
		ROM.ROM.data32 = memory_swapped;
		return 0;
	}
	return -1;
}

struct RAM {
	uint32_t SIZE;
	char * RAM;
} RAM;

void INIT_RAM(uint32_t size) {
	RAM.SIZE = 0;
	RAM.RAM = NULL;
	if (size == 0) {
		printf("CPU: ERROR: CANNOT ALLOCATE RAM OF SIZE %zu\n", size);
		return;
	}
	RAM.RAM = malloc(size);
	if (RAM.RAM == NULL) {
		printf("CPU: ERROR: CANNOT ALLOCATE RAM OF SIZE %zu\n", size);
		return;
	}
	RAM.SIZE = size;
	return;
}
