#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TakoHeiInk : public LiveActor {
public:
    TakoHeiInk(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void start(const TVec3f&, const TVec3f&, bool);
    void exeWait();
};
