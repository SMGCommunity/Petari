#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ModelObj;
class RingBeam;

class RingBeamer : public LiveActor {
public:
    RingBeamer(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void syncSwitchOffB();
    void setupAttack();
    bool tryAttack();
    void exeWait();
    void exeAttack();
    void exeInter();

    RingBeam** mBeams;
    /*
    bool _90;
    // needed to get _98 at correct offset
    u8 _94[4]; // unused?
    TMtx34f _98;
    */
};
