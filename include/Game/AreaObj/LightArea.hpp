#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class LightArea : public AreaObj {
public:
    LightArea(int, const char*);

    virtual void init(const JMapInfoIter&);

    virtual const char* getManagerName() const {
        return "LightArea";
    }

    /* 0x3C */ s32 mPlacedZoneID;
};
