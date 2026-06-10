#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class LavaProminence : public LiveActor {
public:
    LavaProminence(const char*);
    virtual ~LavaProminence();

private:
    u8 mPad[(0x1BC) - sizeof(LiveActor)];
};
