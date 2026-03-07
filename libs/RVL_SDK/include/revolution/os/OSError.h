#ifndef OSERROR_H
#define OSERROR_H

#include <revolution/os/OSContext.h>
#include <revolution/types.h>


#ifdef __cplusplus
extern "C" {
#endif

#define OS_ERROR(...) OSPanic(__FILE__, __LINE__, __VA_ARGS__)

typedef u16 OSError;
typedef void (*OSErrorHandler)(OSError, OSContext*, ...);

#define OS_ERROR_SYSTEM_RESET 0
#define OS_ERROR_MACHINE_CHECK 1
#define OS_ERROR_DSI 2
#define OS_ERROR_ISI 3
#define OS_ERROR_EXTERNAL_INTERRUPT 4
#define OS_ERROR_ALIGNMENT 5
#define OS_ERROR_PROGRAM 6
#define OS_ERROR_FLOATING_POINT 7
#define OS_ERROR_DECREMENTER 8
#define OS_ERROR_SYSTEM_CALL 9
#define OS_ERROR_TRACE 10
#define OS_ERROR_PERFORMACE_MONITOR 11
#define OS_ERROR_BREAKPOINT 12
#define OS_ERROR_SYSTEM_INTERRUPT 13
#define OS_ERROR_THERMAL_INTERRUPT 14
#define OS_ERROR_PROTECTION 15
#define OS_ERROR_FPE 16

#define OS_ERROR_MAX (OS_ERROR_FPE + 1)

OSErrorHandler OSSetErrorHandler(OSError, OSErrorHandler);

extern u32 __OSFpscrEnableBits;

#ifdef __cplusplus
}
#endif

#endif  // OSERROR_H
