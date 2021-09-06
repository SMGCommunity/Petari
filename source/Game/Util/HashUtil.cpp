#pragma once

#include "Game/Util/HashUtil.h"

namespace MR {

    u32 getHashCode(const char *pStr) {
        u32 val;
        for (val = 0; *pStr != 0; pStr++) {
            val = *pStr + (val * 31);
        }

        return val;
    }

};