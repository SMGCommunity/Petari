#pragma once

#include "Game/Screen/WipeLayoutBase.hpp"

class WipeRing : public WipeLayoutBase {
public:
    WipeRing(bool, const char*);

    virtual ~WipeRing();
    virtual void init(const JMapInfoIter&);
    virtual void wipe(s32);
    virtual void forceClose();

    TVec2f mFollowPos;  // 0x20
    s32 mWipeStep;      // 0x28
    f32 _2C;
    f32 _30;
    u8 _34;
};