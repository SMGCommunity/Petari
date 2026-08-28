#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CoconutTreeLeafGroup : public LiveActor {
public:
    CoconutTreeLeafGroup(const char*);
    /* 0x08 */ virtual ~CoconutTreeLeafGroup();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
