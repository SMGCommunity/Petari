#pragma once

#include "Game/NameObj/NameObj.hpp"

class OceanRing;

class OceanRingBloomDrawer : public NameObj {
public:
    OceanRingBloomDrawer(OceanRing*);

    /* 0x08 */ virtual ~OceanRingBloomDrawer();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;

    OceanRing* mRing;  // 0xC
};