#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/SpringValue.hpp"
#include "revolution/types.h"

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

    CollisionParts* mCollisionParts;
    SpringValue* mSpringValue;          // 0x90
    MapObjConnector* mMapObjConnector;  // 0x94
    bool _98;
    bool _99;
    bool _9A;
    s32 _9C;
    const char* _A0;
};

namespace MR {
    bool isMsgHipDropFloor(u32);
}

const char* gDesertLandMoveSwitchvar = 0;
