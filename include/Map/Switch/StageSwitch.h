#pragma once

#include <revolution.h>

class BitFlag128
{
public:
    bool get(s32) const;
    void set(s32, bool);

    s32 mFlags[0x4];
};

class ZoneSwitch
{
public:
    ZoneSwitch();

    BitFlag128 mFlags;
};