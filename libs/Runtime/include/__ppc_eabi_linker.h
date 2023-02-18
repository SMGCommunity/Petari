#ifndef PPC_EABI_LINKER_H
#define PPC_EABI_LINKER_H

__declspec(section ".init") extern char	_stack_addr[];
__declspec(section ".init") extern char _stack_end[];

__declspec(section ".init") extern char _SDA_BASE_[];
__declspec(section ".init") extern char	_SDA2_BASE_[];

typedef struct __eti_init_info {
	void* eti_start;
	void* eti_end;
	void* code_start;
	unsigned long code_size;
} __eti_init_info;

__declspec(section ".init") extern __eti_init_info 	_eti_init_info[];

typedef struct __rom_copy_info {
	char* rom;
	char* addr;
	unsigned int size;
} __rom_copy_info;

__declspec(section ".init") extern __rom_copy_info 	_rom_copy_info[];

typedef struct __bss_init_info {
	char* addr;
	unsigned int size;
} __bss_init_info;

__declspec(section ".init") extern __bss_init_info 	_bss_init_info[];

#endif // PPC_EABI_LINKER_H