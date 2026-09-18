#include "Game/AreaObj/LightAreaHolder.hpp"
#include "Game/AreaObj/LightArea.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Map/LightZoneDataHolder.hpp"

LightAreaHolder::LightAreaHolder(s32 maxNum, const char* pName) : AreaObjMgr(maxNum, pName) {
    LightFunction::registerLightAreaHolder(this);
}

bool LightAreaHolder::tryFindLightID(const TVec3f& rArea, ZoneLightID* pLightID) const {
    AreaObj* pArea = find_in(rArea);

    if (pArea == nullptr) {
        if (pLightID->isOutOfArea()) {
            pLightID->clear();

            return false;
        } else {
            pLightID->clear();

            return true;
        }
    } else {
        const LightArea* pLightArea = static_cast< LightArea* >(pArea);

        if (pLightID->isTargetArea(pLightArea)) {
            return false;
        } else {
            pLightID->_0 = pLightArea->mPlacedZoneID;
            pLightID->mLightID = pLightArea->mObjArg0;

            return true;
        }
    }
}

void LightAreaHolder::initAfterPlacement() {
    sort();
}

void LightAreaHolder::sort() {
    for (s32 i = 0; i < mArray.size() - 1; i++) {
        AreaObj* pOriginal = mArray[i];
        AreaObj* pSelected = pOriginal;
        s32 selected = i;

        for (s32 j = i + 1; j < mArray.size(); j++) {
            AreaObj* pCandidate = mArray[j];

            if (pSelected->mObjArg1 > pCandidate->mObjArg1) {
                selected = j;
                pSelected = pCandidate;
            }
        }

        if (i != selected) {
            mArray[i] = pSelected;
            mArray[selected] = pOriginal;
        }
    }
}
