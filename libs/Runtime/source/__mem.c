#include <__mem.h>

void* memcpy(void *pDest, const void *pSrc, size_t len) {
    const char *p;
    char* q;
    int rev = pDest < pSrc;

    if (!rev) {
        for (p = (const char*)pSrc - 1, q = (char*)pDest - 1, len++; --len;) {
            *++q = *++p;
        }
    }
    else {
        for (p = (const char*)pSrc + len, q = (char*)pDest + len, len++; --len;) {
            *--q = *--p;
        }
    }

    return pDest;
}

void* memset(void *pDest, int val, size_t num) {
    __fill_mem(pDest, val, num);
    return pDest;
}

#pragma section code_type ".text"

size_t (strlen)(const char* pStr)
{
	size_t len = -1;
	unsigned char* p = (unsigned char*)(pStr - 1);

	do {
		len++;
    }
	while (*++p);
		
	return len;
}