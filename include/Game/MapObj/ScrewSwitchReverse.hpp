#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class ScrewSwitchReverse : public LiveActor {
public:
    ScrewSwitchReverse(const char*);
    virtual ~ScrewSwitchReverse();

private:
    u8 mPad[(0x94) - sizeof(LiveActor)];
};
