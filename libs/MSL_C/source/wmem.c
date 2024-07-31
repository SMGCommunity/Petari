#include "wchar_io.h"
#include <cstring>

wchar_t* wmemcpy(wchar_t *pDest, const wchar_t *pSrc, size_t num) {
    return (wchar_t*)memcpy(pDest, pSrc, num * sizeof(wchar_t));
}

wchar_t* wmemchr(const wchar_t *pSrc, wchar_t val, size_t num) {
    while (num != 0) {
        if (*pSrc == val) {
            return (wchar_t*)pSrc;
        }

        pSrc++;
        num--;
    }

    return 0;
}