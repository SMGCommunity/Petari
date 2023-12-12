#ifndef OSERROR_H
#define OSERROR_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u16 OSError;
typedef void (*OSErrorHandler)(OSError, OSContext *, ...);

OSErrorHandler OSSetErrorHandler(OSError, OSErrorHandler);

extern u32 __OSFpscrEnableBits;

#ifdef __cplusplus
}
#endif

#endif // OSERROR_H