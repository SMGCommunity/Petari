#ifndef PRINTF_H
#define PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

int sprintf(char *, const char *, ...);
int snprintf(char *, size_t, const char *, ...);

#ifdef __cplusplus
}
#endif

#endif // PRINTF_H