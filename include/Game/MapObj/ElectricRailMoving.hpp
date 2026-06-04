#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class ElectricRailMoving : public LiveActor {
public:
    ElectricRailMoving(const char*);
    virtual ~ElectricRailMoving();

private:
    u8 mPad[(0xF4) - sizeof(LiveActor)];
};
