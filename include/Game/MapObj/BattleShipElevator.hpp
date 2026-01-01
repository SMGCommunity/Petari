#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class BattleShipElevator : public MapObjActor {
public:
    BattleShipElevator(const char*);

    virtual ~BattleShipElevator();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeMove();
    void exeEnd();
};
