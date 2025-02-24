#pragma once

#include <revolution.h>
#include "Game/System/WPad.hpp"

class WPadStick {
public:
    WPadStick(const WPad *);

    void update();
    bool isChanged() const;

    const WPad* mPad;           // 0x00
    Vec2 mStick;                // 0x04
    f32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u8 _1C;
    u8 _1D;
    u8 _1E;
    u8 _1F;
};
