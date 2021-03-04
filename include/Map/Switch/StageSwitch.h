#pragma once

#include <revolution.h>

class BitFlag128
{
public:
    inline BitFlag128()
    {
        
    }

    bool get(s32) const;
    void set(s32, bool);

    u32 mFlags[0x4];
};

class ZoneSwitch : public BitFlag128
{
public:
    ZoneSwitch();
};