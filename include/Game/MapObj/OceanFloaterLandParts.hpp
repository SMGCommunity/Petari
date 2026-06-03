#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class OceanFloaterLandParts : public LiveActor {
public:
    OceanFloaterLandParts(const char*);
    virtual ~OceanFloaterLandParts();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
