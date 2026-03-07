#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class TeresaWater : public LiveActor {
public:
    TeresaWater(const char*);
    virtual ~TeresaWater();

private:
    u8 mPad[(0xBC) - sizeof(LiveActor)];
};
