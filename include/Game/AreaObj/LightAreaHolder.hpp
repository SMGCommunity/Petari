#pragma once

#include "Game/AreaObj/AreaObj.hpp"
#include "Game/AreaObj/LightArea.hpp"

class ZoneLightID;

class LightAreaHolder : public AreaObjMgr {
public:
    LightAreaHolder(s32, const char*);

    virtual ~LightAreaHolder();
    virtual void initAfterPlacement();

    bool tryFindLightID(const TVec3f&, ZoneLightID*) const;
    void sort();
};