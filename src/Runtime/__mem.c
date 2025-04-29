#include <__mem.h>

void* memcpy(void *pDest, const void *pSrc, size_t len) {
    const char *p;
    char* q;
    int rev = pSrc < pDest;

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

// I really wish I could tell you what's going on here but I can't
void __fill_mem(void *pDest, int val, unsigned long n) {
	unsigned long v = (unsigned char) val;
	unsigned long i;

	((unsigned char*)pDest) = ((unsigned char*)pDest) - 1;

	if (n >= 32) {
		i = (~(unsigned long)pDest) & 3;

		if (i) {
			n -= i;

			do {
				*++(((unsigned char*)pDest)) = v;
			}
			while (--i);
		}

		if (v != 0) {
			v |= v << 24 | v << 16 | v <<  8;
		}

		((unsigned long*)pDest) = ((unsigned long*) (((unsigned char*)pDest) + 1)) - 1;
		i = n >> 5;

		if (i != 0) {
			do {
				*++(((unsigned long*)pDest)) = v;
				*++(((unsigned long*)pDest)) = v;
				*++(((unsigned long*)pDest)) = v;
				*++(((unsigned long*)pDest)) = v;
				*++(((unsigned long*)pDest)) = v;
				*++(((unsigned long*)pDest)) = v;
				*++(((unsigned long*)pDest)) = v;
				*++(((unsigned long*)pDest)) = v;
			}
			while (--i);
		}

		i = (n & 31) >> 2;

		if (i) {
			do {
				*++(((unsigned long*)pDest)) = v;
			}	
			while (--i);
		}

		((unsigned char*)pDest) = ((unsigned char*) (((unsigned long*)pDest) + 1)) - 1;
		n &= 3;
	}

	if (n) {
		do {
			*++(((unsigned char*)pDest)) = v;
		}
		while (--n);
	}
}

void* memset(void *pDest, int val, size_t num) {
    __fill_mem(pDest, val, num);
    return pDest;
}

#pragma section code_type ".text"

__declspec(weak) size_t (strlen)(const char* str);
size_t (strlen)(const char * str)
{
	size_t len = -1;
	unsigned char *p = (unsigned char*)str - 1;

	do {
		len++;
	}
	while (*++p);
		
	return len;
}

#pragma section code_type
