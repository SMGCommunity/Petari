#pragma once

#include "revolution/types.h"

class ByamlContainerHeader {
public:
    s32 getType() const { return mType >> 24; }
    s32 getCount() const { return mType & 0xFFFFFF; }

    u32 mType;
};