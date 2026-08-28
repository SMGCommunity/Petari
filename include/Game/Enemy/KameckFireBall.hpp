#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KameckBeamEventListener;

class KameckFireBall : public LiveActor {
public:
    KameckFireBall(const char*);

    /* 0x08 */ virtual ~KameckFireBall();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void appearDirection(const TVec3f&);
    void setEventListener(KameckBeamEventListener*);
    void exeWait();
    void exeSweep();

    TQuat4f _8C;
    KameckBeamEventListener* mEventListener;  // 0x9C
};
