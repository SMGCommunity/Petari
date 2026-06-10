#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class SnowFloor : public LiveActor {
public:
    SnowFloor(const char*);
    virtual ~SnowFloor();

private:
    u8 mPad[(0x120) - sizeof(LiveActor)];
};
