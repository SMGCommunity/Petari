#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class GravityLightRoad : public LiveActor {
public:
    GravityLightRoad(const char*);
    virtual ~GravityLightRoad();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
