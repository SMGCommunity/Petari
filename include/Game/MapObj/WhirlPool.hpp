#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class WhirlPool : public LiveActor {
public:
    WhirlPool(const char*);
    /* 0x08 */ virtual ~WhirlPool();

private:
    u8 mPad[(0xD8) - sizeof(LiveActor)];
};
