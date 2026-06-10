#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class TeresaWater : public LiveActor {
public:
    TeresaWater(const char*);
    virtual ~TeresaWater();

private:
    u8 mPad[(0xBC) - sizeof(LiveActor)];
};
