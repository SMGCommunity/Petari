#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class OceanSmallTurtle : public LiveActor {
public:
    OceanSmallTurtle(const char*);
    /* 0x08 */ virtual ~OceanSmallTurtle();

private:
    u8 mPad[(0x114) - sizeof(LiveActor)];
};
