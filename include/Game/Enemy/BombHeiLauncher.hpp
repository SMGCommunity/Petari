#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class BombHeiLauncher : public LiveActor {
public:
    BombHeiLauncher(const char*);
    virtual ~BombHeiLauncher();

private:
    u8 mPad[(0x9C) - sizeof(LiveActor)];
};
