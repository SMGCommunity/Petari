#include "mbstring.h"
#include "locale.h"
#include <cstring>
#pragma exceptions on

int mbtowc(wchar_t *pDest, const char *s, size_t num) {
    return _current_locale.ctype_cmpt_ptr->decode_mb(pDest, s, num);
}

inline int wctomb(char *pSrc, wchar_t wchar) {
    return _current_locale.ctype_cmpt_ptr->encode_wc(pSrc, wchar);
}

int __mbtowc_noconv(wchar_t *pDest, const char *pSrc, size_t n) {
    if (pSrc == 0) {
        return 0;
    }

    if (n == 0) {
        return -1;
    }

    if (pDest) {
        *pDest = (unsigned char)*pSrc;
    }

    if (!*pSrc) {
        return 0;
    }

    return 1;
}

int __wctomb_noconv(char *pSrc, wchar_t cr) {
    if (pSrc == 0) {
        return 0;
    }

    *pSrc = cr;
    return 1;
}

size_t mbstowcs(wchar_t *pDest, const char *pSrc, size_t num) {
    int res;
    char* src;
    int count;
    size_t src_len;

    src_len = strlen(pSrc);

    if (pDest != 0) {
        src = (char*)pSrc;

        for (count = 0; count < num; count++) {
            if (*src) {
                res = mbtowc(pDest++, src, src_len);

                if (res > 0) {
                    src += res;
                    src_len -= res;
                }
                else {
                    return -1;
                }
            }
            else {
                *pDest = 0;
                break;
            }
        }
    }
    else {
        count = 0;
    }

    return count;
}

size_t wcstombs(char *pSrc, const wchar_t *pwcs, size_t n) {
    int chars_written = 0;
    int result;
    char temp[3];
    wchar_t* source;

    if (!pSrc || !pwcs) {
        return 0;
    }

    source = (wchar_t*)pwcs;

    while (chars_written <= n) {
        if (!*source) {
            *(pSrc + chars_written) = '\0';
            break;
        }
        else {
            result = wctomb(temp, *source++);

            if ((chars_written + result) <= n) {
                strncpy(pSrc + chars_written, temp, result);
                chars_written += result;
            }
            else {
                break;
            }
        }
    }

    return chars_written;
}
