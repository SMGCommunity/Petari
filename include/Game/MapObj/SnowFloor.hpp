#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class SnowFloor : public LiveActor {
public:
    SnowFloor(const char*);
    virtual ~SnowFloor();

private:
    u8 mPad[(0x120) - sizeof(LiveActor)];
};
