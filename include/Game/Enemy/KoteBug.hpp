#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoteBug : public LiveActor {
public:
    KoteBug(const char*);
    /* 0x08 */ virtual ~KoteBug();

private:
    u8 mPad[(0xBC) - sizeof(LiveActor)];
};
