#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JetTurtle;

class KameckTurtle : public LiveActor {
public:
    KameckTurtle(const char*);

    /* 0x08 */ virtual ~KameckTurtle();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
