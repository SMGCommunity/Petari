#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class WaterPlant : public LiveActor {
public:
    WaterPlant(const char*);
    virtual ~WaterPlant();

private:
    u8 mPad[(0xA8) - sizeof(LiveActor)];
};
