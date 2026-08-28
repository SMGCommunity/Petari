#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SurprisedGalaxy : public LiveActor {
public:
    SurprisedGalaxy(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeExit();
    void exeWait();

    const char* mGalaxyName;
};
