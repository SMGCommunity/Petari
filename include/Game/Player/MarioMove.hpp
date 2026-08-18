#pragma once

#include "Game/Player/MarioModule.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class MarioActor;

class MarioMove : public MarioModule {
public:
    MarioMove(MarioActor*);
    void initAfter();

    /* 0x08 */ TVec3f mHeadVec;
    /* 0x14 */ TVec3f mFrontVec;
    /* 0x20 */ TVec3f mSideVec;

    /* 0x2C */ TVec3f mHeadVec2;
    /* 0x38 */ TVec3f mFrontVec2;
    /* 0x44 */ TVec3f mSideVec2;

    /* 0x50 */ f32 _50;
    /* 0x54 */ f32 _54;
};
