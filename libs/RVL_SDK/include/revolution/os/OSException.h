#ifndef OSEXCEPTION_H
#define OSEXCEPTION_H

#include <revolution/types.h>
#include <revolution/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u8   __OSException;
typedef void (*__OSExceptionHandler)(__OSException, OSContext *);

__OSExceptionHandler __OSSetExceptionHandler(__OSException, __OSExceptionHandler);
__OSExceptionHandler __OSGetExceptionHandler(__OSException);

#ifdef __cplusplus
}
#endif

#endif // OSEXCEPTION_H