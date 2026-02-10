#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class MeteorStrikeLauncher : public LiveActor {
public:
    MeteorStrikeLauncher(const char*);
    virtual ~MeteorStrikeLauncher();

private:
    u8 mPad[(0xA0) - sizeof(LiveActor)];
};
