#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class PomponPlant : public LiveActor {
public:
    PomponPlant(const char*);
    virtual ~PomponPlant();

private:
    u8 mPad[(0x98) - sizeof(LiveActor)];
};
