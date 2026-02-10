#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class OceanFloaterLandParts : public LiveActor {
public:
    OceanFloaterLandParts(const char*);
    virtual ~OceanFloaterLandParts();

private:
    u8 mPad[(0xAC) - sizeof(LiveActor)];
};
