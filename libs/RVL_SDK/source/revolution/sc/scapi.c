#include <revolution/sc.h>

s8 SCGetDisplayOffsetH(void) {
    s8 offset;

    if (!SCFindS8Item(&offset, 5)) {
        offset = 0;
    }
    else {
        if (offset < -32) {
            offset = -32;
        }
        else if (offset > 32) {
            offset = 32;
        }
    }

    offset &= ~1;
    return offset;
}

u8 SCGetEuRgb60Mode(void) {
    u8 mode;

    if (!SCFindU8Item(&mode, 6)) {
        mode = 0;
    }
    else {
        if (mode != 1) {
            mode = 0;
        }
    }

    return mode;
}

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

u8 SCGetScreenSaverMode(void) {
    u8 mode;

    if (!SCFindU8Item(&mode, 15)) {
        mode = 1;
    }
    else {
        if (mode != 1) {
            mode = 0;
        }
    }

    return mode;
}

u32 SCGetCounterBias(void) {
    u32 bias;

    if (!SCFindU32Item(&bias, 0)) {
        bias = 189388800;
    }

    return bias;
}