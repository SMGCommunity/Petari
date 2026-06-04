#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class SpringWaterFloaterSpot : public LiveActor {
public:
    SpringWaterFloaterSpot(const char*);
    virtual ~SpringWaterFloaterSpot();

private:
    u8 mPad[(0x100) - sizeof(LiveActor)];
};
