#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MapObjConnector;

class HitWallTimerSwitch : public LiveActor {
public:
    HitWallTimerSwitch(const char*);

    virtual ~HitWallTimerSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void calcAnim();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    bool trySwitchDown();
    bool tryOn();
    bool trySwitchUp();
    bool tryOff();
    void exeOff();
    void exeSwitchDown();
    void exeOn();
    void exeSwitchUp();

    /* 0x8C */ CollisionParts* _8C;
    /* 0x90 */ MapObjConnector* mConnector;
    /* 0x94 */ s32 mTimer;
    /* 0x98 */ u8 _98;
};
