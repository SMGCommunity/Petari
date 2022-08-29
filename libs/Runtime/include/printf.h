#ifndef PRINTF_H
#define PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <va_list.h>

int vprintf(const char *, va_list);

#ifdef __cplusplus
}
#endif

#endif // PRINTF_H