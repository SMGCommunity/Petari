#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LavaProminence : public LiveActor {
public:
    LavaProminence(const char*);
    /* 0x08 */ virtual ~LavaProminence();

private:
    u8 mPad[(0x1BC) - sizeof(LiveActor)];
};
