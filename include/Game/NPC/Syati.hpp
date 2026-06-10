#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class Syati : public LiveActor {
public:
    Syati(const char*);
    virtual ~Syati();

private:
    u8 mPad[(0x15C) - sizeof(LiveActor)];
};
