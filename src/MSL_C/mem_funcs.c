#include "mem_funcs.h"

#define srcCharPtr	((unsigned char*)pSrc)
#define destCharPtr	((unsigned char*)pDest)
#define srcLongPtr	((unsigned long*)pSrc)
#define destLongPtr	((unsigned long*)pDest)

void __copy_longs_aligned(void *pDest, const void *pSrc, unsigned long len) {
    unsigned long i = (-(unsigned long)pDest) & 3;
    srcCharPtr = ((unsigned char *)pSrc) - 1;
    destCharPtr = ((unsigned char *)pDest) - 1;
    
    if (i != 0) {
        len -= i;

        do {
            *++(destCharPtr) = *++(srcCharPtr);
        } while (--i);
    }

    srcLongPtr = ((unsigned long *)(srcCharPtr + 1)) - 1;
    destLongPtr = ((unsigned long *)(destCharPtr + 1)) - 1;

    i = len >> 5;

    if (i != 0) {
        do {
            *++(destLongPtr) = *++(srcLongPtr);
            *++(destLongPtr) = *++(srcLongPtr);
            *++(destLongPtr) = *++(srcLongPtr);
            *++(destLongPtr) = *++(srcLongPtr);
            *++(destLongPtr) = *++(srcLongPtr);
            *++(destLongPtr) = *++(srcLongPtr);
            *++(destLongPtr) = *++(srcLongPtr);
            *++(destLongPtr) = *++(srcLongPtr);
        } while (--i);
    }

    i = (len & 31) >> 2;

    if (i != 0) {
        do {
            *++(destLongPtr) = *++(srcLongPtr);
        } while (--i);
    }

    srcCharPtr = ((unsigned char *) (srcLongPtr + 1)) - 1;
	destCharPtr = ((unsigned char *) (destLongPtr + 1)) - 1;

    len &= 3;
	
	if (len != 0) {
		do
			*++(destCharPtr) = *++(srcCharPtr);
		while (--len);
    }
}

void __copy_longs_rev_aligned(void *pDest, const void *pSrc, unsigned long len) {
    unsigned long i;
    srcCharPtr = ((unsigned char *)pSrc) + len;
    destCharPtr = ((unsigned char *)pDest) + len;
    i = ((unsigned long) destCharPtr) & 3;

    if (i != 0) {
        len -= i;

        do {
            *--destCharPtr = *--srcCharPtr;
        } while(--i);
    }

    i = len >> 5;

    if (i != 0) {
        do {
            *--destLongPtr = *--srcLongPtr;
            *--destLongPtr = *--srcLongPtr;
            *--destLongPtr = *--srcLongPtr;
            *--destLongPtr = *--srcLongPtr;
            *--destLongPtr = *--srcLongPtr;
            *--destLongPtr = *--srcLongPtr;
            *--destLongPtr = *--srcLongPtr;
            *--destLongPtr = *--srcLongPtr;
        } while(--i);
    }

    i = (len & 31) >> 2;

    if (i != 0) {
        do {
            *--destLongPtr = *--srcLongPtr;
        } while(--i);
    }

    len &= 3;

    if (len != 0) {
        do {
            *--destCharPtr = *--srcCharPtr;
        } while(--len);
    }
}

void __copy_longs_unaligned(void *pDest, const void *pSrc, unsigned long len) {
    unsigned long i, v1, v2;
    unsigned int src, ls, rs;

    i = (-(unsigned long)pDest) & 3;
    srcCharPtr = ((unsigned char*)pSrc) - 1;
    destCharPtr = ((unsigned char*)pDest) - 1;

    if (i != 0) {
        len -= i;

        do {
            *++destCharPtr = *++srcCharPtr;
        } while(--i);
    }

    src = ((unsigned int)(srcCharPtr + 1)) & 3;
    ls = src << 3;
    rs = 32 - ls;

    srcCharPtr -= src;

    srcLongPtr = ((unsigned long*)(srcCharPtr + 1)) - 1;
    destLongPtr = ((unsigned long*)(destCharPtr + 1)) - 1;

    i = len >> 3;
    v1 = *++srcLongPtr;

    do {
        v2 = *++srcLongPtr;
        *++destLongPtr = (v1 << ls) | (v2 >> rs);
        v1 = *++srcLongPtr;
        *++destLongPtr = (v2 << ls) | (v1 >> rs);
    } while(--i);

    if (len & 4) {
        v2 = *++srcLongPtr;
        *++destLongPtr = (v1 << ls) | (v2 >> rs);
    }

    srcCharPtr = ((unsigned char *)(srcLongPtr + 1)) - 1;
    destCharPtr = ((unsigned char *)(destLongPtr + 1)) - 1;

    len &= 3;

    if (len != 0) {
        srcCharPtr -= 4 - src;
        do {
            *++destCharPtr = *++srcCharPtr;
        } while(--len);
    }
}

void __copy_longs_rev_unaligned(void *pDest, const void *pSrc, unsigned long len) {
    unsigned long i, v1, v2;
    unsigned int src, ls, rs;

    srcCharPtr = ((unsigned char*)pSrc) + len;
    destCharPtr = ((unsigned char*)pDest) + len;
    i = ((unsigned long)pDest) & 3;

    if (i != 0) {
        len -= i;

        do {
            *--destCharPtr = *--srcCharPtr;
        } while(--i);
    }

    src = ((unsigned int)(srcCharPtr)) & 3;
    ls = src << 3;
    rs = 32 - ls;

    srcCharPtr += 4 - src;
    i = len >> 3;
    v1 = *--srcLongPtr;

    do {
        v2 = *--srcLongPtr;
        *--destLongPtr = (v2 << ls) | (v1 >> rs);
        v1 = *--srcLongPtr;
        *--destLongPtr = (v1 << ls) | (v2 >> rs);
    } while(--i);

    if (len & 4) {
        v2 = *--srcLongPtr;
        *--destLongPtr = (v2 << ls) | (v1 >> rs);
    }

    len &= 3;

    if (len != 0) {
        srcCharPtr += src;
        do {
            *--destCharPtr = *--srcCharPtr;
        } while(--len);
    }
}