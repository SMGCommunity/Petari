#include "revolution/vf/pf_clib.h"

s32 VFipf_toupper(s32 c) {
    if (c >= 97 && c <= 122) {
        c -= 32;
    }
    return c;
}

void* VFipf_memcpy(void* dst, const void* src, u32 length) {
    u32* ld;
    const u32* ls;
    s8* d;
    const u32* s;

    ld = dst;
    ls = src;
    if ((*((u8*)dst) & 3) == 0 && (*((u8*)src) & 3) == 0) {
        while (length > 3) {
            *ld++ = *ls++;
            length -= 4;
        }
    }
    d = (s8*)ld;
    for (s = ls; length-- != 0; s = (s + 1)) {
        *d++ = *s;
    }
    return dst;
}

void* VFipf_memset(void* dst, s32 c, u32 length) {
    s8* d;    // r31
    u32* ld;  // r30
    s8* v1;   // r31

    for (d = dst; (*((u8*)d) & 3) != 0 && length; ++d) {
        *d = c;
        --length;
    }
    ld = (u32*)d;
    while (length > 3) {
        *ld++ = c | (c << 8) | (c << 24) | (c << 16);
        length -= 4;
    }
    for (v1 = (s8*)ld; length-- != 0; ++v1)
        *v1 = c;
    return dst;
}

u32 VFipf_strlen(const s8* s) {
    const s8* t;

    for (t = s; *t; ++t)
        ;
    return t - s;
}

s8* VFipf_strcpy(s8* dst, const s8* src) {
    s8* i = dst;
    while ((*i++ = *src++))
        ;
    return dst;
}

s32 VFipf_strcmp(const s8* s1, const s8* s2) {
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;

    while (*p1 && *p2 && *p1 == *p2) {
        ++p1;
        ++p2;
    }
    return *p1 - *p2;
}

s32 VFipf_strncmp(const s8* s1, const s8* s2, u32 length) {
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;

    while (length-- != 0) {
        if (!*p1 || !*p2 || *p1 != *p2)
            return *p1 - *p2;
        ++p1;
        ++p2;
    }
    return 0;
}
