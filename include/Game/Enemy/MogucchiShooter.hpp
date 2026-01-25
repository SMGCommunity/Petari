#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class MogucchiShooter : public PartsModel {
public:
    MogucchiShooter(LiveActor*, const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAndSetBaseMtx();

    void panicDeath();
    void explosion();
    void hitShock();
    void anger();
    void stormStart();
    void storm();
    bool isLaughed() const;

    void exeWait();
    void exeShot();
    void exeTire();
    void exeDeathPanic();
    void exeExplosion();
    void exeLaugh();
    void exeShock();
    void exePanic();
    void exeHitShock();
    void exeHitPanic();
    void exeAnger();
    void exeStormStart();
    void exeStorm();

    void resetDirection() NO_INLINE;
    void faceToMario();

    /* 0x9C */ TVec3f mFront;
};
