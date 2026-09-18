#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class OceanSmallTurtle : public LiveActor {
public:
    OceanSmallTurtle(const char*);
    virtual ~OceanSmallTurtle();

public:
    u8 mPad[(0x114) - sizeof(LiveActor)];
};
