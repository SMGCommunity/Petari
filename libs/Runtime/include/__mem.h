#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int size_t;
typedef unsigned int u32;

#pragma section code_type ".init"

void* memcpy(void *, const void *, size_t);
void __fill_mem(void *, int, unsigned long);
void* memset(void *, int, size_t);

#ifdef __cplusplus
}
#endif