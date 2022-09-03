#ifndef PPC_EABI_LINKER_H
#define PPC_EABI_LINKER_H

typedef struct __eti_init_info {
	void * 			eti_start;
	void *			eti_end;
	void * 			code_start;
	unsigned long	code_size;
} __eti_init_info;

__declspec(section ".init") extern __eti_init_info 	_eti_init_info[];

#endif // PPC_EABI_LINKER_H