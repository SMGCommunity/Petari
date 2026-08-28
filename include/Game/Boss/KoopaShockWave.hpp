#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class Koopa;
class ModelObj;
class PartsModel;

class KoopaShockWave : public LiveActor {
public:
    KoopaShockWave(Koopa*);

    /* 0x08 */ virtual ~KoopaShockWave();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x50 */ virtual void updateHitSensor(HitSensor*);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWaveAttack();

private:
    /* 0x08C */ Koopa* mKoopa;
    /* 0x090 */ f32 mAngle;
    /* 0x094 */ TPos3f _94;
    /* 0x0C4 */ TVec3f mUp;
    /* 0x0D0 */ TVec3f mFront;
    /* 0x0DC */ TPos3f mBaseMtx;
    /* 0x10C */ TPos3f mMtx;
    /* 0x13C */ PartsModel* mPartsModel;
    /* 0x140 */ ModelObj* mShadow;
};
