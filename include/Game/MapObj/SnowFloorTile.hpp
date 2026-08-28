#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SnowFloorTile : public LiveActor {
public:
    SnowFloorTile(const char*);
    /* 0x08 */ virtual ~SnowFloorTile();

private:
    u8 mPad[(0x10C) - sizeof(LiveActor)];
};
