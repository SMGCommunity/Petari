#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class ElectricRailMoving : public LiveActor {
public:
    ElectricRailMoving(const char*);
    virtual ~ElectricRailMoving();

private:
    u8 mPad[(0xF4) - sizeof(LiveActor)];
};
