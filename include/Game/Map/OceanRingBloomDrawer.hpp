#pragma once

#include "Game/NameObj/NameObj.hpp"

class OceanRing;

class OceanRingBloomDrawer : public NameObj {
public:
    OceanRingBloomDrawer(OceanRing*);

    virtual ~OceanRingBloomDrawer();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;

    OceanRing* mRing;  // 0xC
};