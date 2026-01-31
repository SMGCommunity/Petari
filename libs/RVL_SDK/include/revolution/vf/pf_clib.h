#ifndef PF_CLIB_H
#define PF_CLIB_H

#include "revolution/types.h"

void* VFipf_memset(void* dst, long c, unsigned long length);

void* VFipf_memcpy(void* dst, void* src, u32 length);

#endif  // PF_CLIB_H
