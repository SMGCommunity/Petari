#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FishGroup : public LiveActor {
public:
    FishGroup(const char*);
    /* 0x08 */ virtual ~FishGroup();

private:
    u8 mPad[(0xB0) - sizeof(LiveActor)];
};
