#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class SandUpDownEffectObj : public LiveActor {
public:
    SandUpDownEffectObj(const char*);
    virtual ~SandUpDownEffectObj();

private:
    u8 mPad[(0xD4) - sizeof(LiveActor)];
};
