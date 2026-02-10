#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class PlantGroup : public LiveActor {
public:
    PlantGroup(const char*);
    virtual ~PlantGroup();

private:
    u8 mPad[(0xBC) - sizeof(LiveActor)];
};
