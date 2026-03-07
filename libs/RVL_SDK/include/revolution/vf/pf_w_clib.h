#ifndef PF_W_CLIB_H
#define PF_W_CLIB_H

#include "revolution/types.h"

u32 VFipf_w_strlen(const u16* s);
u16* VFipf_w_strcpy(u16* dst, const u16* src);
u16* VFipf_w_strncpy(u16* dst, const u16* src, u32 length);
s32 VFipf_w_strcmp(const u16* s1, const u16* s2);
s32 VFipf_w_strncmp(const u16* s1, const u16* s2, u32 length);

#endif
