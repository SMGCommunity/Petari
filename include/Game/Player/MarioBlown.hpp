#pragma once

#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Util/MathUtil.hpp"

class MarioBlown : public MarioState {
public:
    MarioBlown(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    f32 vecKillActor240(const TVec3f& rVec) {
        return MR::vecKillElement(rVec, mActor->_240, &_18);
    }

    /* 0x12 */ u16 mTimer;
    /* 0x14 */ u16 _14;
    /* 0x18 */ TVec3f _18;
    /* 0x24 */ bool _24;
    /* 0x25 */ bool _25;
};
