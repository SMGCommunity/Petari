#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Balloonfish : public LiveActor {
public:
    Balloonfish(const char*);
    /* 0x08 */ virtual ~Balloonfish();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
