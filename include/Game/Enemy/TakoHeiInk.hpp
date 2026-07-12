#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TakoHeiInk : public LiveActor {
public:
    TakoHeiInk(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void start(const TVec3f&, const TVec3f&, bool);
    void exeWait();
};
