#include <revolution/sc.h>

BOOL SCGetIdleMode(SCIdleModeInfo *info) {
    return SCFindByteArrayItem(info, sizeof(*info), 9);
}

u8 SCGetLanguage(void) {
    u8 lang;
    s8 area;

    if (!SCFindU8Item(&lang, 11)) {
        area = SCGetProductArea();

        if (area == 0) {
            lang = 0;
        }
        else {
            lang = 1;
        }
    }
    else {
        if (lang > 9) {
            lang = 1;
        }
    }

    return lang;
}

u32 SCGetCounterBias(void) {
    u32 bias;

    if (!SCFindU32Item(&bias, 0)) {
        bias = 189388800;
    }

    return bias;
}