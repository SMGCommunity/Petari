#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class WhirlPoolAccelerator : public LiveActor {
public:
    WhirlPoolAccelerator(const char*);
    virtual ~WhirlPoolAccelerator();

private:
    u8 mPad[(0xC4) - sizeof(LiveActor)];
};
