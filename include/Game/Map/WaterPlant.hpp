#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class WaterPlant : public LiveActor {
public:
    WaterPlant(const char*);
    virtual ~WaterPlant();

private:
    u8 mPad[(0xA8) - sizeof(LiveActor)];
};
