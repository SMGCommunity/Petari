#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class ScenarioStarter : public LiveActor {
public:
    ScenarioStarter(const char*);
    virtual ~ScenarioStarter();

private:
    u8 mPad[(0x128) - sizeof(LiveActor)];
};
