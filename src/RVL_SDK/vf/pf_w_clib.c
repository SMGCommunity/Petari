#include "revolution/vf.h"

u32 VFipf_w_strlen(const u16* s) {
    const u16* t;

    t = s;
    while (*t != L'\0') {
        t++;
    }

    return ((s32)t - (s32)s) >> 1;
}

u16* VFipf_w_strcpy(u16* dst, const u16* src) {
    u16* d;  // Present in DWARF but unused here.

    goto jump;
    while (*src != L'\0') {
        src++;
        dst++;
    jump:
        *dst = *src;
    }
}

s32 VFipf_w_strncmp(const u16* s1, const u16* s2, u32 length) {
    const u16* p1;  // Present in DWARF but unused here.
    const u16* p2;  // Present in DWARF but unused here.

    while (length != 0) {
        if (*s1 == L'\0' || *s2 == L'\0' || *s1 != *s2) {
            return (int)(*s1 - *s2);
        }

        s1++;
        s2++;
        length--;
    }
    return 0;
}
