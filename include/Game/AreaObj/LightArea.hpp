#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class LightArea : public AreaObj {
public:
    LightArea(int, const char*);

    virtual ~LightArea();
    virtual void init(const JMapInfoIter&);

    virtual const char* getManagerName() const;

    s32 mPlacedZoneID;  // 0x3C
};