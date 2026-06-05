#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class SubmarineSteam : public LiveActor {
public:
    SubmarineSteam(const char*);
    virtual ~SubmarineSteam();

private:
    u8 mPad[(0x98) - sizeof(LiveActor)];
};
