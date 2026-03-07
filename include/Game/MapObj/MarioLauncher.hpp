#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class MarioLauncher : public LiveActor {
public:
    MarioLauncher(const char*);
    virtual ~MarioLauncher();

private:
    u8 mPad[(0x220) - sizeof(LiveActor)];
};
