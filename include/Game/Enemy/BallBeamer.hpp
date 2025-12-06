#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class NameObjArchiveListCollector;
class RingBeam;

class BallBeamer : public LiveActor {
public:
    BallBeamer(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    void syncSwitchOnA();
    void syncSwitchOffA();
    void syncSwitchOnB();
    void setupAttack();
    bool tryAttack();
    void exeWait();
    void exeAttack();
    void exeInter();

    RingBeam** mBeams;
    bool _90;
    // needed to get _98 at correct offset
    u8 _94[4];  // unused?
    TMtx34f _98;
};
