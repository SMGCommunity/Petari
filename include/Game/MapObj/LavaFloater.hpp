#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class LavaFloater : public LiveActor {
public:
    LavaFloater(const char*);
    virtual ~LavaFloater();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
