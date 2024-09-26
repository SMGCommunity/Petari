#pragma once

#include <cstring>
#include <__ppc_eabi_linker.h>
#include <size_t.h>

typedef void* ctor_dtor_ptr;

extern "C" {

void* __construct_new_array(void *, ctor_dtor_ptr, ctor_dtor_ptr, size_t, size_t);
void __construct_array(void *, ctor_dtor_ptr, ctor_dtor_ptr, size_t, size_t);
void __destroy_arr(void *, ctor_dtor_ptr, size_t, size_t);
void __destroy_new_array(void *, ctor_dtor_ptr);

extern int __register_fragment(struct __eti_init_info *, char *);
extern void	__unregister_fragment(int);

};