#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpringWaterFloaterSpot : public LiveActor {
public:
    SpringWaterFloaterSpot(const char*);
    /* 0x08 */ virtual ~SpringWaterFloaterSpot();

private:
    u8 mPad[(0x100) - sizeof(LiveActor)];
};
