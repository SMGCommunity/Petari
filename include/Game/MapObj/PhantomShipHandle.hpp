#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PhantomShipHandle : public LiveActor {
public:
    PhantomShipHandle(const char*);
    /* 0x08 */ virtual ~PhantomShipHandle();

private:
    u8 mPad[(0x90) - sizeof(LiveActor)];
};
