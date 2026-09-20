#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;

class WhirlPool : public LiveActor {
public:
    WhirlPool(const char*);

    virtual void init(const JMapInfoIter& rITer);
    virtual void movement();

    void initPoints();

    /* 0x8C */ f32 _8C;
    /* 0x90 */ f32 _90;
    /* 0x94 */ TVec3f _94;
    u8 mPad[0x24];
    /* 0xC4 */ JUTTexture* _C4;
    /* 0xC8 */ JUTTexture* _C8;
    /* 0xCC */ TVec3f _CC;
};
