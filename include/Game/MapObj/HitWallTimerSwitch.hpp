#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class HitWallTimerSwitch : public LiveActor {
public:
    HitWallTimerSwitch(const char*);
    virtual ~HitWallTimerSwitch();

private:
    u8 mPad[(0x9C) - sizeof(LiveActor)];
};
