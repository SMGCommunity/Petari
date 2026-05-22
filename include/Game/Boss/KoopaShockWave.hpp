#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaShockWave : public LiveActor {
public:
    KoopaShockWave(Koopa*);

    virtual ~KoopaShockWave();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void makeActorDead();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWaveAttack();

private:
    /* 0x8C */ Koopa* mKoopa;
    /* 0x90 */ f32 _90;
    /* 0x94 */ TPos3f _94;
    /* 0xC4 */ TVec3f _C4;
    /* 0xD0 */ TVec3f _D0;
    /* 0xDC */ TPos3f mBaseMtx;
    /* 0x10C */ TPos3f _10C;
    /* 0x13C */ PartsModel* mPartsModel;
    /* 0x140 */ ModelObj* mShadow;
};
