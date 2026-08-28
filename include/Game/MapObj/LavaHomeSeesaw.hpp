#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LavaHomeSeesaw : public LiveActor {
public:
    LavaHomeSeesaw(const char*);
    /* 0x08 */ virtual ~LavaHomeSeesaw();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
