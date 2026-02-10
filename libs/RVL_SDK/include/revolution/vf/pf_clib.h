#ifndef PF_CLIB_H
#define PF_CLIB_H

#include "revolution/types.h"

s32 VFipf_toupper(s32 c);
void* VFipf_memcpy(void* dst, const void* src, u32 length);
void* VFipf_memset(void* dst, s32 c, u32 length);
s32 VFipf_memcmp(const void* s1, const void* s2, u32 length);
u32 VFipf_strlen(const s8* s);
u32 VFipf_strnlen(const s8* s, u32 maxlen);
s8* VFipf_strcpy(s8* dst, const s8* src);
s8* VFipf_strncpy(s8* dst, const s8* src, u32 length);
s8* VFipf_strchr(const s8* s, s32 c);
s8* VFipf_strcat(s8* dst, const s8* src);
s32 VFipf_strcmp(const s8* s1, const s8* s2);
s32 VFipf_strncmp(const s8* s1, const s8* s2, u32 length);

s8* VFipf_strcpy(s8* dst, const s8* src);

#endif  // PF_CLIB_H
