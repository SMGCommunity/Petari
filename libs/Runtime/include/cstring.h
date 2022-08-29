#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

typedef u32 size_t;

void* memcpy(void *, const void *, u32);

int strcmp(const char *, const char *);

#ifdef __cplusplus
}
#endif