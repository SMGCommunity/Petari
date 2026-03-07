#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class PomponPlant : public LiveActor {
public:
    PomponPlant(const char*);
    virtual ~PomponPlant();

private:
    u8 mPad[(0x98) - sizeof(LiveActor)];
};
