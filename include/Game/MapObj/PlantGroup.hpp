#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PlantGroup : public LiveActor {
public:
    PlantGroup(const char*);
    /* 0x08 */ virtual ~PlantGroup();

private:
    u8 mPad[(0xBC) - sizeof(LiveActor)];
};
