#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class BegomanLauncher : public LiveActor {
public:
    BegomanLauncher(const char*);
    virtual ~BegomanLauncher();

private:
    u8 mPad[(0x9C) - sizeof(LiveActor)];
};
