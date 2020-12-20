#pragma once

#include <revolution.h>

class BitFlag128
{
public:
    bool get(s32) const;
    void set(s32, bool);

    u8 mFlags[0x32];
};

class ZoneSwitch
{
public:
    ZoneSwitch();

    BitFlag128 mFlags;
};