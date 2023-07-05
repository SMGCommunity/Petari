#ifndef NANDLOGGING_H
#define NANDLOGGING_H

#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*NANDLoggingCallback)(BOOL);

BOOL NANDLoggingAddMessageAsync(NANDLoggingCallback, const char *, ...);

#ifdef __cplusplus
}
#endif

#endif // NANDLOGGING_H