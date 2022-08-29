#ifndef OSCONTEXT_H
#define OSCONTEXT_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSContext {
    u32 gpr[32];
    u32 cr;
    u32 lr;
    u32 ctr;
    u32 xer;
    f64 fpr[32];
    u32 fpscr_pad;
    u32 fpscr;
    u32 srr0;
    u32 srr1;
    u16 mode;
    u16 state;
    u32 gqr[8];
    u32 psf_pad;
    f64 psf[32];
} OSContext;

#define OS_CONTEXT_R0       0
#define OS_CONTEXT_R1       4
#define OS_CONTEXT_R2       8
#define OS_CONTEXT_R3       12
#define OS_CONTEXT_R4       16
#define OS_CONTEXT_R5       20
#define OS_CONTEXT_R6       24
#define OS_CONTEXT_R7       28
#define OS_CONTEXT_R8       32
#define OS_CONTEXT_R9       36
#define OS_CONTEXT_R10      40
#define OS_CONTEXT_R11      44
#define OS_CONTEXT_R12      48
#define OS_CONTEXT_R13      52
#define OS_CONTEXT_R14      56
#define OS_CONTEXT_R15      60
#define OS_CONTEXT_R16      64
#define OS_CONTEXT_R17      68
#define OS_CONTEXT_R18      72
#define OS_CONTEXT_R19      76
#define OS_CONTEXT_R20      80
#define OS_CONTEXT_R21      84
#define OS_CONTEXT_R22      88
#define OS_CONTEXT_R23      92
#define OS_CONTEXT_R24      96
#define OS_CONTEXT_R25      100
#define OS_CONTEXT_R26      104
#define OS_CONTEXT_R27      108
#define OS_CONTEXT_R28      112
#define OS_CONTEXT_R29      116
#define OS_CONTEXT_R30      120
#define OS_CONTEXT_R31      124

#define OS_CONTEXT_GQR0     420
#define OS_CONTEXT_GQR1     424
#define OS_CONTEXT_GQR2     428
#define OS_CONTEXT_GQR3     432
#define OS_CONTEXT_GQR4     436
#define OS_CONTEXT_GQR5     440
#define OS_CONTEXT_GQR6     444
#define OS_CONTEXT_GQR7     448

u32 OSGetStackPointer(void);

void OSDumpContext(OSContext *);

#ifdef __cplusplus
}
#endif

#endif // OSCONTEXT_H