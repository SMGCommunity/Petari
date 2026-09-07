#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KameckBeamEventListener;

class KameckFireBall : public LiveActor {
public:
    /// @brief Creates a new `KameckFireBall`.
    /// @param pName A pointer to the null-terminated name of the object.
    KameckFireBall(const char* pName);

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

    /* 0x8C */ TQuat4f _8C;
    /* 0x9C */ KameckBeamEventListener* mEventListener;
    /* 0xA0 */ u8 _A0[4];
};
