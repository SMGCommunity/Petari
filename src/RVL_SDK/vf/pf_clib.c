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
    const s8* s;

    ld = dst;
    ls = src;
    if (((u32)dst & 3) == 0 && ((u32)src & 3) == 0) {
        while (length > 3) {
            *ld++ = *ls++;
            length -= 4;
        }
    }
    d = (s8*)ld;
    for (s = (const s8*)ls; length-- != 0; s = (s + 1)) {
        *d++ = *s;
    }
    return dst;
}

void* VFipf_memset(void* dst, s32 c, u32 length) {
    s8* d;
    u32* ld;
    u32 lc;
    s8* v1;

    d = dst;
    while (((u32)d & 3) != 0 && length) {
        *d = c;
        d++;
        length--;
    }
    ld = (u32*)d;
    lc = (c | (c << 8)) | ((c << 24) | (c << 16));
    while (length > 3) {
        *ld++ = lc;
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
    s8* d = dst;

    while ((*d = *src) != 0) {
        src++;
        d++;
    }

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
