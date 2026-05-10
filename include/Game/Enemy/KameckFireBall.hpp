#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KameckBeamEventListener;

class KameckFireBall : public LiveActor {
public:
    KameckFireBall(const char*);

    virtual ~KameckFireBall();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void appearDirection(const TVec3f&);
    void setEventListener(KameckBeamEventListener*);
    void exeWait();
    void exeSweep();

    TQuat4f _8C;
    KameckBeamEventListener* mEventListener;  // 0x9C
};
