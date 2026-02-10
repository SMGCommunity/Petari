#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class OceanSmallTurtle : public LiveActor {
public:
    OceanSmallTurtle(const char*);
    virtual ~OceanSmallTurtle();

private:
    u8 mPad[(0x114) - sizeof(LiveActor)];
};
