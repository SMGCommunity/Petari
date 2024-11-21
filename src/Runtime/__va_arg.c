typedef struct {
	char gpr;
	char fpr;
	char reserved[2];
	char *input_arg_area;
	char *reg_save_area;
} va_list[1];

typedef enum {
    arg_ARGPOINTER,
    arg_WORD,
    arg_DOUBLEWORD,
    arg_REAL,
    arg_VECTOR
} _va_arg_type;

#define ALIGN(addr,amount)	(((unsigned int)(addr)+((amount)-1))&~((amount)-1))

void* __va_arg(va_list ap, _va_arg_type type) {
    char* addr;
    char* r = &(ap->gpr);
    int rr = ap->gpr;
    int max = 8;
    int size = 4;
    int inc = 1;
    int even = 0;
    int fpr_offset = 0;
    int size_reg = 4;

    if (type == 3) {
		r = &(ap->fpr);
		rr = ap->fpr;
		size = 8;
		fpr_offset = 32;
		size_reg = 8;
	}

	if (type == 2) {
		size = 8;
		max = max - 1;

		if (rr & 1) {
			even = 1;
        }

		inc = 2;
	}

    if (rr < max) {
		rr += even;
		addr = ap->reg_save_area + fpr_offset + (rr * size_reg);
		*r = rr + inc;
	} else {
		*r = 8;
		addr = ap->input_arg_area;
		addr = (char*)ALIGN(addr, size);
		ap->input_arg_area = addr + size;
	}
    if (type == 0)
        addr = *((char**)addr);

	return addr;
}