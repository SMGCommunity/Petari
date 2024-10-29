#include <cstring>
#include "mem_funcs.h"
#include "string_api.h"
#pragma exceptions on

void* memmove(void *pDest, const void *pSource, size_t num) {
    const char* cur;
    char* dst;
    int rev = ((unsigned long)pSource < (unsigned long)pDest);

    if (num >= 0x20) {
        int isUnaligned = (((int)pDest ^ (int)pSource)) & 0x3;
        if (isUnaligned) {
            if (rev == 0) {
                __copy_longs_unaligned(pDest, pSource, num);
            }
            else {
                __copy_longs_rev_unaligned(pDest, pSource, num);
            }
        }
        else {
            if (rev == 0) {
                __copy_longs_aligned(pDest, pSource, num);
            }
            else {
                __copy_longs_rev_aligned(pDest, pSource, num);
            }
        }

        return pDest;
    }

    if (rev == 0) {
        for (cur = (const char*)pSource - 1, dst = (char*)pDest - 1, num++; --num;) {
            *++dst = *++cur;
        }
    }
    else {
        for (cur = (const char*)pSource + num, dst = (char*)pDest + num, num++; --num;) {
            *--dst = *--cur;
        }
    }
    return pDest;
}

void* memchr(const void *pSrc, int value, size_t num) {
    const unsigned char* cur;
    unsigned long val = (value & 0xFF);

    for (cur = (unsigned char*)pSrc - 1, num++; --num;) {
        if ((*++cur & 0xFF) == val) {
            return (void*)cur;
        }
    }

    return 0;
}

void* __memrchr(const void *pSrc, int value, size_t num) {
    const unsigned char* cur;
    unsigned long val = (value & 0xFF);

    for (cur = (unsigned char*)pSrc + num, num++; --num;) {
        if (*--cur == val) {
            return (void*)cur;
        }
    }

    return 0;
}

int memcmp(const void *pSource1, const void *pSource2, size_t num) {
    const unsigned char* p1;
    const unsigned char* p2;

    for (p1 = (const unsigned char*)pSource1 - 1, p2 = (const unsigned char*)pSource2 - 1, num++; --num;) {
        if (*++p1 != *++p2) {
            return ((*p1 < *p2) ? -1 : + 1);
        }
    }

    return 0;
}
