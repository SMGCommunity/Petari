#pragma once

#include "Game/NameObj/NameObj.hpp"

class OceanBowl;

class OceanBowlBloomDrawer : public NameObj {
public:
    OceanBowlBloomDrawer(OceanBowl*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;

    /* 0x0C */ OceanBowl* mHost;
};