#pragma once

#include <revolution.h>

namespace MR {
    void copyMemory(void *, const void *, u32);
    void fillMemory(void *, u8, u32);
    void zeroMemory(void *, u32);
    s32 calcCheckSum(const void *, u32);
};