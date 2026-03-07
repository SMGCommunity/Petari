#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class ShockWaveGenerator : public LiveActor {
public:
    ShockWaveGenerator(const char*);
    virtual ~ShockWaveGenerator();

private:
    u8 mPad[(0x90) - sizeof(LiveActor)];
};
