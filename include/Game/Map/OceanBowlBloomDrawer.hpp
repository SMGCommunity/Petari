#pragma once

#include "Game/NameObj/NameObj.hpp"

class OceanBowl;

class OceanBowlBloomDrawer : public NameObj {
public:
    OceanBowlBloomDrawer(OceanBowl*);

    virtual ~OceanBowlBloomDrawer();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;

    OceanBowl* mOwner;  // 0xC
};