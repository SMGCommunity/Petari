#include <__ppc_eabi_linker.h>
#include <NMWException.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void __init_cpp_exceptions(void);
extern void __fini_cpp_exceptions(void);

#ifdef __cplusplus
}
#endif

static int fragmentID = -2;

void __init_cpp_exceptions(void) {
    if (fragmentID == -2) {
        register char *temp;
        asm {
            mr temp,r2
        }

        fragmentID = __register_fragment(_eti_init_info, (char*)temp);
    }
}

void __fini_cpp_exceptions(void) {
    if (fragmentID != -2) {
        __unregister_fragment(fragmentID);
        fragmentID = -2;
    }
}

__declspec(section ".ctors")
extern void * const __init_cpp_exceptions_reference = __init_cpp_exceptions;