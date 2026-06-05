#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class BlueStarCupsulePlanet : public LiveActor {
public:
    BlueStarCupsulePlanet(const char*);
    virtual ~BlueStarCupsulePlanet();

private:
    u8 mPad[(0xA0) - sizeof(LiveActor)];
};
