#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class ElectricRail : public LiveActor {
public:
    ElectricRail(const char*);
    virtual ~ElectricRail();

private:
    u8 mPad[(0xB8) - sizeof(LiveActor)];
};
