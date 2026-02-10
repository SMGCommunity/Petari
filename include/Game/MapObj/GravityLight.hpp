#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class GravityLight : public LiveActor {
public:
    GravityLight(const char*);
    virtual ~GravityLight();

private:
    u8 mPad[(0xDC) - sizeof(LiveActor)];
};
