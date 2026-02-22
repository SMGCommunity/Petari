#ifndef NWC24STDAPI_H
#define NWC24STDAPI_H

#include "revolution.h"
#include <cstdio>

#ifdef __cplusplus
extern "C" {
#endif

void* Mail_memset(void* dst, int ch, size_t n);

size_t STD_strnlen(const char* str, size_t n);

#ifdef __cplusplus
}
#endif

#endif  // NWC24STDAPI_H
