#include <revolution/sc.h>

BOOL SCGetIdleMode(SCIdleModeInfo *info) {
    return SCFindByteArrayItem(info, sizeof(*info), 9);
}

u32 SCGetCounterBias(void) {
    u32 bias;

    if (!SCFindU32Item(&bias, 0)) {
        bias = 189388800;
    }

    return bias;
}