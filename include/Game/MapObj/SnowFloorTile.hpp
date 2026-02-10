#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class SnowFloorTile : public LiveActor {
public:
    SnowFloorTile(const char*);
    virtual ~SnowFloorTile();

private:
    u8 mPad[(0x10C) - sizeof(LiveActor)];
};
