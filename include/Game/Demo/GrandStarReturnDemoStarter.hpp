#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class GrandStarReturnDemoStarter : public LiveActor {
public:
    GrandStarReturnDemoStarter(const char*);
    virtual ~GrandStarReturnDemoStarter();

private:
    u8 mPad[(0x114) - sizeof(LiveActor)];
};
