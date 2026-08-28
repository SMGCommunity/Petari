#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SnowFloor : public LiveActor {
public:
    SnowFloor(const char*);
    /* 0x08 */ virtual ~SnowFloor();

private:
    u8 mPad[(0x120) - sizeof(LiveActor)];
};
