#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class HipDropTimerSwitch : public LiveActor {
public:
    HipDropTimerSwitch(const char*);
    virtual ~HipDropTimerSwitch();

private:
    u8 mPad[(0xD8) - sizeof(LiveActor)];
};
