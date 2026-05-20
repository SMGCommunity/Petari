#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaFireShort : public LiveActor {
public:
    KoopaFireShort(const Koopa*);

    virtual ~KoopaFireShort();
    virtual void init(const JMapInfoIter&);
    virtual void appear();

    void emitNormal();
    void emitFast();
    void emitCurve();
    void emitLongTime();

    void exeFly();
    void exeDisappear();
    void exeErase2P();

    void calcAndSetBaseMtx();
    void attackSensor(HitSensor*, HitSensor*);
    void updateFly();
    void updateFrontAndVelocity();

private:
    /* 0x8C */ const Koopa* mKoopa;
    /* 0x90 */ TVec3f mFront;
    /* 0x9C */ f32 mSpeed;
    /* 0xA0 */ u32 mDuration;
    /* 0xA4 */ f32 mOffset;
};
