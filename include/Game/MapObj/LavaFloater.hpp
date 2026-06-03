#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class LavaFloater : public LiveActor {
public:
    LavaFloater(const char*);
    virtual ~LavaFloater();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
