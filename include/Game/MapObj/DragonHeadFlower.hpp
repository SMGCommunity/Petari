#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DragonHeadFlower : public LiveActor {
public:
    DragonHeadFlower(const char*);
    virtual ~DragonHeadFlower();

private:
    u8 mPad[(0xE0) - sizeof(LiveActor)];
};
