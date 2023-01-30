#include "revolution/base/PPCArch.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*funcPtr) (void);
__declspec(section ".init") extern funcPtr _ctors[];
__declspec(section ".init") extern funcPtr _dtors[];
extern void exit(int);
static void __init_cpp(void);

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

#ifdef __cplusplus
}
#endif
