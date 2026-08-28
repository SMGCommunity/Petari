#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Shellfish : public LiveActor {
public:
    Shellfish(const char*);
    /* 0x08 */ virtual ~Shellfish();

private:
    u8 mPad[(0xA8) - sizeof(LiveActor)];
};
