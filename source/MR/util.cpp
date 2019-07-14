#include "MR/util.h"

namespace MR
{
    // todo -- make me better
    s32 getHashCode(const char *r3)
    {
        s32 r0 = 0;
        s32 r5 = 0;
        char r4;
        goto loadChar;

        genHash:
            r0 = r5 * 0x1F;
            r5 = (s32)r4 + r0;
            r3 = r3 + 1;
            goto loadChar;

        loadChar:
            r4 = *r3;
            
            if (r4 != '\0')
                goto genHash;

            return r5;
    }
};