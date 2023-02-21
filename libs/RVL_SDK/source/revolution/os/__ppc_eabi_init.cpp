#include "revolution/base/PPCArch.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*funcPtr) (void);
__declspec(section ".init") extern funcPtr _ctors[];
__declspec(section ".init") extern funcPtr _dtors[];
extern void exit(int);
static void __init_cpp(void);

extern void __OSCacheInit(void);
extern void __OSFPRInit(void);
extern void __OSPSInit(void);

asm void __init_hardware(void) {
    nofralloc
    bl __OSPSInit
    bl __OSFPRInit
    bl __OSCacheInit
    mtlr r31
    blr
}

/* written in assembly to generate the frame allocations */
asm void __init_user(void) {
    fralloc
    bl __init_cpp
    frfree
    blr
}

static void __init_cpp(void) {
    funcPtr* ctor;

    for (ctor = _ctors; *ctor; ctor++) {
        (*ctor)();
    }
}

static void __fini_cpp(void) {
    funcPtr* dtor;

    for (dtor = _dtors; *dtor; dtor++) {
        (*dtor)();
    }
}

void exit(int status) {
    __fini_cpp();
    PPCHalt();
}

asm void __flush_cache(void *, unsigned int) {
    nofralloc

    lis     r5, 0xFFFF
    ori     r5, r5, 0xFFF1
    and     r5, r5, r3
    subf    r3, r5, r3
    add     r4, r4, r3

loop:
    dcbst   0,r5
    sync
    icbi    0,r5
    addic   r5,r5,0x8
    subic.  r4,r4,0x8
    bge     loop
    isync

    blr
}

#ifdef __cplusplus
}
#endif
