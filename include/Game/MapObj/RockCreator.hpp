#pragma once

#include "Game/LiveActor//LiveActor.hpp"

class RockCreator : public LiveActor {
public:
    RockCreator(const char*);

    u8 _8C[0x1C];
};
