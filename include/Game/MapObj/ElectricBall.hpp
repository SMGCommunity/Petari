#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class ElectricBall : public LiveActor {
public:
    ElectricBall(const char*);
    virtual ~ElectricBall();

private:
    u8 mPad[(0xDC) - sizeof(LiveActor)];
};
