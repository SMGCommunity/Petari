#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class PhantomShipBoxFloater : public LiveActor {
public:
    PhantomShipBoxFloater(const char*);
    virtual ~PhantomShipBoxFloater();

private:
    u8 mPad[(0xA4) - sizeof(LiveActor)];
};
