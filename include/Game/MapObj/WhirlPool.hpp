#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class WhirlPool : public LiveActor {
public:
    WhirlPool(const char*);
    virtual ~WhirlPool();

public:
    u8 mPad[(0xD8) - sizeof(LiveActor)];
};
