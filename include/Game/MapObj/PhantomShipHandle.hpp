#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class PhantomShipHandle : public LiveActor {
public:
    PhantomShipHandle(const char*);
    virtual ~PhantomShipHandle();

private:
    u8 mPad[(0x90) - sizeof(LiveActor)];
};
