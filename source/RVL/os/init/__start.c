#include <revolution.h>

#pragma section code_type ".init"

#ifdef __cplusplus
extern "C" {
#endif

extern void __start(void);

#ifdef __cplusplus
}
#endif

__declspec (weak) asm void __start(void) {
    
}