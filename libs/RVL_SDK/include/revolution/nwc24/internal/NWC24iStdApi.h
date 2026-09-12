#ifndef RVL_SDK_NWC24_INTERNAL_STD_API_H
#define RVL_SDK_NWC24_INTERNAL_STD_API_H
#include <revolution/types.h>
#include <cstdlib>
#ifdef __cplusplus
extern "C" {
#endif

char* Mail_strcpy(char* pDst, const char* pSrc);
int Mail_strlen(const char* pStr);
int STD_strnlen(const char* pStr, size_t maxlen);
void* Mail_memcpy(void* pDst, const void* pSrc, size_t n);
void* Mail_memset(void* pDst, int ch, size_t n);
char* Mail_strcat(char* pDst, const char* pSrc);
char* Mail_strncat(char* pDst, const char* pSrc, size_t maxlen);
int Mail_sprintf(char* pDst, char* pFmt, ...);
int Mail_vsprintf(char* pDst, char* pFmt, va_list argv);

#ifdef __cplusplus
}
#endif
#endif
