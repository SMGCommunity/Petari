#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Kameck : public LiveActor {
public:
    Kameck(const char*);

    void setBeamType(s32);

    void makeActorDeadForce();

    u8 _8C[0xD0 - 0x8C];
};

namespace MR {
    NameObj* createFireBallBeamKameck(const char*);
};
