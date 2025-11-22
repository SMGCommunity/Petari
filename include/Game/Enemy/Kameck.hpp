#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Kameck : public LiveActor {
public:
    Kameck(const char*);

    void setBeamType(s32);

    u8 _8C[0xD0 - 0x8C];
};