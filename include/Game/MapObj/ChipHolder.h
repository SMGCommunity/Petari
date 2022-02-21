#pragma once

#include <revolution.h>

class ChipBase;

namespace MR {
    void registerChip(s32, ChipBase *, s32);

    void createChipHolder(s32);

    s32 getGotChipCount(s32, s32);
    void noticeGetChip(s32, ChipBase *, s32);
};