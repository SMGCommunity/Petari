#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

typedef u32 size_t;

void* memcpy(void *, const void *, u32);
void* memset(void *, int, size_t);

int strcmp(const char *, const char *);

const char* strstr(const char *, const char *);

char* strcpy(char *pDest, const char *pSrc);

size_t strlen(const char *pStr);

char* strcat(char *, const char *);

char* strncpy(char *, const char *, size_t);
const char* strchr(const char *, int);
char* strrchr(char *, int);

#ifdef __cplusplus
}
#endif