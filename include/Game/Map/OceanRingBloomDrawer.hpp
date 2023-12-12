#pragma once

#include "Game/NameObj/NameObj.h"

class OceanRing;

class OceanRingBloomDrawer : public NameObj {
public:
    OceanRingBloomDrawer(OceanRing *);

    virtual ~OceanRingBloomDrawer();
    virtual void init(const JMapInfoIter &);
    virtual void draw() const;


    OceanRing* mRing;   // _C
};