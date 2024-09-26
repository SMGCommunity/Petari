#include "Game/AreaObj/LightAreaHolder.hpp"
#include "Game/Map/LightZoneDataHolder.hpp"
#include "Game/Map/LightFunction.hpp"

LightAreaHolder::LightAreaHolder(s32 type, const char *pName) : AreaObjMgr(type, pName) {
    LightFunction::registerLightAreaHolder(this);
}

#ifdef NON_MATCHING
// for some reason the register movement for isTargetArea are wrong
bool LightAreaHolder::tryFindLightID(const TVec3f &rArea, ZoneLightID *pLightID) const {
    const LightArea* lightArea = reinterpret_cast<LightArea*>(find_in(rArea));

    if (!lightArea) {
        if (pLightID->isOutOfArea()) {
            pLightID->clear();
            return false;
        }
        else {
            pLightID->clear();
            return true;
        }
    }
    else {
        if (pLightID->isTargetArea(lightArea)) {
            return false;
        }
        else {
            pLightID->_0 = lightArea->mPlacedZoneID;
            pLightID->mLightID = lightArea->mObjArg0;
            return true;
        }
    }
}
#endif

void LightAreaHolder::initAfterPlacement() {
    sort();
}

LightAreaHolder::~LightAreaHolder() {

}
