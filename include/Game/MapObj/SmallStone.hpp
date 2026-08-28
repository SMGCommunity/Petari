#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SmallStone : public LiveActor {
public:
    SmallStone(const char*);
    /* 0x08 */ virtual ~SmallStone();

private:
    u8 mPad[(0xA0) - sizeof(LiveActor)];
};
