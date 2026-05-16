#ifndef OSEXCEPTION_H
#define OSEXCEPTION_H

#include <revolution/os/OSContext.h>
#include <revolution/types.h>


#ifdef __cplusplus
extern "C" {
#endif

#define __OS_EXCEPTION_SYSTEM_RESET 0
#define __OS_EXCEPTION_MACHINE_CHECK 1
#define __OS_EXCEPTION_DSI 2
#define __OS_EXCEPTION_ISI 3
#define __OS_EXCEPTION_EXTERNAL_INTERRUPT 4
#define __OS_EXCEPTION_ALIGNMENT 5
#define __OS_EXCEPTION_PROGRAM 6
#define __OS_EXCEPTION_FLOATING_POINT 7
#define __OS_EXCEPTION_DECREMENTER 8
#define __OS_EXCEPTION_SYSTEM_CALL 9
#define __OS_EXCEPTION_TRACE 10
#define __OS_EXCEPTION_PERFORMACE_MONITOR 11
#define __OS_EXCEPTION_BREAKPOINT 12
#define __OS_EXCEPTION_SYSTEM_INTERRUPT 13
#define __OS_EXCEPTION_THERMAL_INTERRUPT 14
#define __OS_EXCEPTION_MAX (__OS_EXCEPTION_THERMAL_INTERRUPT + 1)

typedef u8 __OSException;
typedef void (*__OSExceptionHandler)(__OSException, OSContext*);

__OSExceptionHandler __OSSetExceptionHandler(__OSException, __OSExceptionHandler);
__OSExceptionHandler __OSGetExceptionHandler(__OSException);

#ifdef __cplusplus
}
#endif

#endif  // OSEXCEPTION_H
