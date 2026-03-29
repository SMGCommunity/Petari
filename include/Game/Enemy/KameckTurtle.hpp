#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JetTurtle;

class KameckTurtle : public LiveActor {
public:
    KameckTurtle(const char*);

    virtual ~KameckTurtle();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initJetTurtle();
    void appearDirection(const TVec3f&);
    void exeRun();
    void exeWait();
    bool isEnableAttack() const;
    void appearJetTurtle(bool);

    u32 _8C;
    JetTurtle* mTurtle;  // 0x90
    TQuat4f _94;
    TVec3f _A4;
    TVec3f _B0;
};
