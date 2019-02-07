int offset = 0;
int bitcount = 0;

unsigned char * readbin(unsigned char * encoding, int len) {
	unsigned char * bits = malloc(32);
	int ii = 7;
	int was_set = 0;
	
	for (int i = 0; i < len; i++)
	{
		bits[i]=!getBit(*encoding, ii)?'0':'1';
		ii--;
		bitcount++;
		if (bitcount>7) {
			encoding = (encoding)+0x1;
			bitcount = 0;
			was_set = 1;
			ii = 7;
		}
	}
	if (was_set == 0 && bitcount<=7) 
		for(int i = 0; i < len; i++)
			*encoding = *encoding << 0x1;
	was_set = 0;
	return bits;
}

unsigned char * readbinbit(unsigned char * encoding, int len) {
	unsigned char * bits = malloc(32);
	int ii = 7;
	int was_set = 0;
	
	for (int i = 0; i < len; i++)
	{
		bits[i]=!getBit(*encoding, ii)?'0':'1';
		ii--;
		bitcount++;
		if (bitcount>7) {
			encoding = (encoding)+0x1;
			bitcount = 0;
			was_set = 1;
			ii = 7;
		}
	}
	if (was_set == 0 && bitcount<=7) for(int i = 0; i < len; i++) *encoding = *encoding << 0x1;
	was_set = 0;
	return bits;
}


unsigned char * readbin_(unsigned char * encoding, int len) {
	unsigned char * bits = malloc(len);
	int ii = 7;
	int was_set = 0;
	
	for (int i = 0; i < len; i++)
	{
		bits[i]=!getBit(*encoding, ii)?'0':'1';
		ii--;
		bitcount++;
		if (bitcount>7) {
			encoding = (encoding)+0x1;
			bitcount = 0;
			was_set = 1;
			ii = 7;
		}
	}
	bits[len] = 0;
	was_set = 0;
	return bits;
}

unsigned char * readbin_l(unsigned char * encoding, int len) {
	unsigned char * bits = malloc(len);
	int ii = 0;
	int was_set = 0;
	
	for (int i = 0; i < len; i++)
	{
		bits[i]=!getBit(*encoding, ii)?'0':'1';
		ii++;
		bitcount++;
		if (bitcount>7) {
			encoding = (encoding)+0x1;
			bitcount = 0;
			was_set = 1;
			ii = 0;
		}
	}
	bits[len] = 0;
	was_set = 0;
	return bits;
}

#define printBin32(num) { \
	for (int i = 31; i != -1; i--) { \
		ifc printf_r("%d", (num >> i) & 0x1); \
		else printf("%d", (num >> i) & 0x1); \
	} \
	printf(" = %s", #num); \
}

#define printBinLen(num, len) { \
	for (int i = len-1; i != -1; i--) { \
		ifc printf_r("%d", (num >> i) & 0x1); \
		else printf("%d", (num >> i) & 0x1); \
	} \
	printf(" = %s", #num); \
}

int BITC = 0;
int BITL = 0;
int BITI = 0;
char * BinLen(unsigned char ** num, int len) {
	unsigned char * n = *num;
	unsigned char * bits = malloc(len+1);
	for (int i = len-1; i != -1; i--) {
		pi(getBit((*num), 7));
		pi(getBit((*num), 7));
	}
	return bits;
}

char * BinLen_(unsigned char ** encoding, int len) {
	unsigned char * bits = malloc(32);
	int ii = 7;
	int was_set = 0;
	
	for (int i = 0; i < len; i++)
	{
		bits[i]=(!getBit(*encoding, ii))?'0':'1';
		ii--;
		bitcount++;
		if (bitcount>7) {
			(*encoding)++;
			bitcount = 0;
			was_set = 1;
			ii = 7;
		}
	}
// 	if (was_set == 0 && bitcount<=7) for(int i = 0; i < len; i++) *encoding = *encoding << 0x1;
	was_set = 0;
	return bits;
}

#define printarray32(num) { \
	for (int i = 0; i != 32; i++) { \
		ifc printf_r("%c", num[i]); \
		else printf("%c", num[i]); \
	} \
	printf(" = %s", #num); \
}
