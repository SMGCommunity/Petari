#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/SpringValue.hpp"
#include <revolution/types.h>

class DesertLandMoveSwitch : public LiveActor {
public:
    DesertLandMoveSwitch(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void calcAnim();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    void initModelAndCollision(const JMapInfoIter& rIter);
    bool tryOn();
    bool trySwitchDown();
    bool tryConnect();
    void updateTimerSE();
    void exeWait();
    void exeSwitchDown();
    void exeOn();
    void exeReturn();

    /* 0x8C */ CollisionParts* mCollisionParts;
    /* 0x90 */ SpringValue* mSpringValue;
    /* 0x94 */ MapObjConnector* mMapObjConnector;
    /* 0x98 */ bool _98;
    /* 0x99 */ bool _99;
    /* 0x9A */ bool _9A;
    /* 0x9C */ s32 _9C;
    /* 0xA0 */ const char* _A0;
};
