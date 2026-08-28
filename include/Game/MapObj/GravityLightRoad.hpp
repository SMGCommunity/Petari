#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class GravityLightRoad : public LiveActor {
public:
    GravityLightRoad(const char*);
    /* 0x08 */ virtual ~GravityLightRoad();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
