#include "revolution/ax.h"
#include "revolution/os.h"
#include <mem.h>

static u8 __clearAuxA[3];

static AXAuxCallback __AXCallbackAuxA;
static void* __AXContextAuxA;

// These SDK entry points are retained in the retail binary for the product.sel export symbol table.
#pragma push
#pragma force_active on
void AXRegisterAuxACallback(AXAuxCallback callback, void* context) {
    BOOL enabled = OSDisableInterrupts();

    __AXCallbackAuxA = callback;
    __AXContextAuxA = context;

    if (callback == NULL) {
        memset(&__clearAuxA, TRUE, 3);
    }

    OSRestoreInterrupts(enabled);
}


void AXGetAuxACallback(AXAuxCallback* callback, void** context) {
    *callback = __AXCallbackAuxA;
    *context = __AXContextAuxA;
}
#pragma pop
