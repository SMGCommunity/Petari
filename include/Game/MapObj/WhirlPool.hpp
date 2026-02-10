#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class WhirlPool : public LiveActor {
public:
    WhirlPool(const char*);
    virtual ~WhirlPool();

private:
    u8 mPad[(0xD8) - sizeof(LiveActor)];
};
