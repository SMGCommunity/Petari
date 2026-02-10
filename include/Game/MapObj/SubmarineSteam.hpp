#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class SubmarineSteam : public LiveActor {
public:
    SubmarineSteam(const char*);
    virtual ~SubmarineSteam();

private:
    u8 mPad[(0x98) - sizeof(LiveActor)];
};
