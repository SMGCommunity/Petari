#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MapObjConnector;

class HitWallTimerSwitch : public LiveActor {
public:
    HitWallTimerSwitch(const char*);

    /* 0x08 */ virtual ~HitWallTimerSwitch();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x1C */ virtual void calcAnim();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    bool trySwitchDown();
    bool tryOn();
    bool trySwitchUp();
    bool tryOff();
    void exeOff();
    void exeSwitchDown();
    void exeOn();
    void exeSwitchUp();

    CollisionParts* _8C;
    MapObjConnector* mConnector;  // 0x90
    s32 mTimer;                   // 0x94
    u8 _98;
};
