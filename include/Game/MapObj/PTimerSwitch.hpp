#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapObjConnector.hpp"

class PTimerSwitch : public LiveActor {
public:
    PTimerSwitch(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void calcAnim();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void reset();
    bool trySwitchDown();
    bool tryOn();
    void exeOff();
    void exeSwitchDown();
    void exeOn();

    /* 0x8C */ CollisionParts* mSwitchCollision;
    /* 0x90 */ MapObjConnector* mConnector;
    /* 0x94 */ s32 _94;
    /* 0x98 */ bool _98;
};
