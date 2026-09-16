#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class ZoneLightID;

class LightAreaHolder : public AreaObjMgr {
public:
    LightAreaHolder(s32 maxNum, const char* pName);

    virtual void initAfterPlacement();

    bool tryFindLightID(const TVec3f& rArea, ZoneLightID* pLightID) const;
    void sort();
};
