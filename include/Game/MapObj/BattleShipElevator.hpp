#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class BattleShipElevator : public MapObjActor {
public:
    BattleShipElevator(const char*);

    /* 0x08 */ virtual ~BattleShipElevator();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeMove();
    void exeEnd();
};
