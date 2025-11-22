#pragma once

#include <revolution.h>

class AudStageBgmTable {
public:
    static u32 getBgmId(const char*, s32);

    static s32 getBgmState(const char*, s32);
};