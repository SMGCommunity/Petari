#include "TRK_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern u32 OSRestoreInterrupts(u32);
extern u32 OSDisableInterrupts(void);

u32 MWExitCriticalSection(u32 *level) {
    return OSRestoreInterrupts(*level);
}

void MWEnterCriticalSection(u32 *level) {
    *level = OSDisableInterrupts();
}

void MWInitializeCriticalSection(u32 *level) {
    return;
}

#ifdef __cplusplus
}
#endif