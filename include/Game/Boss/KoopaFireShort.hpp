#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaFireShort : public LiveActor {
public:
    KoopaFireShort(const Koopa* pKoopa);

    virtual ~KoopaFireShort();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void emitNormal();
    void emitFast();
    void emitCurve();
    void emitLongTime();

    void exeFly();
    void exeDisappear();
    void exeErase2P();

    void updateFly();
    void updateFrontAndVelocity();

private:
    /* 0x8C */ const Koopa* mKoopa;
    /* 0x90 */ TVec3f mFront;
    /* 0x9C */ f32 mSpeed;
    /* 0xA0 */ s32 mDuration;
    /* 0xA4 */ f32 mOffset;
};
