#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaFireShort : public LiveActor {
public:
    KoopaFireShort(const Koopa* pKoopa);

    /* 0x08 */ virtual ~KoopaFireShort();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

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
