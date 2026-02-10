#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class SpringWaterFloaterSpot : public LiveActor {
public:
    SpringWaterFloaterSpot(const char*);
    virtual ~SpringWaterFloaterSpot();

private:
    u8 mPad[(0x100) - sizeof(LiveActor)];
};
