#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PhantomShipBoxFloater : public LiveActor {
public:
    PhantomShipBoxFloater(const char*);
    /* 0x08 */ virtual ~PhantomShipBoxFloater();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
