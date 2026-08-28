#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MapObjConnector;

class SpinLeverSwitch : public LiveActor {
public:
    SpinLeverSwitch(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeSwitchOn();
    void exeEnd();

    /* 0x8C */ MapObjConnector* mConnector;
};
