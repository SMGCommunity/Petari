#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ShootingStar : public LiveActor {
public:
    ShootingStar(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();

    void appearPreShooting();
    void exePreShooting();
    void exeShooting();
    void exeWaitForNextShoot();

    /* 0x8C */ TVec3f mBasePos;
    /* 0x98 */ TVec3f mFallAxis;
    /* 0xA4 */ TVec3f mGravityPreShoot;
    /* 0xB0 */ s32 mNumBurstStarPiece;
    /* 0xB4 */ s32 mWaitForNextShootTime;
    /* 0xB8 */ f32 mFallHeight;
};
