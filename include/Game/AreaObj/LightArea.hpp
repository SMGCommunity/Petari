#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class LightArea : public AreaObj {
public:
    LightArea(int, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    /* 0x28 */ virtual const char* getManagerName() const {
        return "LightArea";
    }

    /* 0x3C */ s32 mPlacedZoneID;
};
