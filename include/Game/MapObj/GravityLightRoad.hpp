#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class GravityLightRoad : public LiveActor {
public:
    GravityLightRoad(const char*);
    virtual ~GravityLightRoad();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
