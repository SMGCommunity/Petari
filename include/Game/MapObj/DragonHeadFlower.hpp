#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DragonHeadFlower : public LiveActor {
public:
    DragonHeadFlower(const char*);
    /* 0x08 */ virtual ~DragonHeadFlower();

private:
    u8 mPad[(0xE0) - sizeof(LiveActor)];
};
