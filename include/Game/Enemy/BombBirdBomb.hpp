#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "revolution/types.h"

class BombBirdBomb : public LiveActor {
public:
    BombBirdBomb(const char*);

    virtual ~BombBirdBomb();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void hold(const TVec3f&);
    void start(const TVec3f&, const TVec3f&);
    void exeHold();
    void exeWait();
    void exeExplosion();

    TQuat4f _8C;
    TVec3f _9C;
};
