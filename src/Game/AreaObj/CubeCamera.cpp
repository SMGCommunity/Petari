#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/AreaObj/AreaObj.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "revolution/types.h"

#include <cstring>

CubeCameraArea::CubeCameraArea(int type, const char* pName) : AreaObj(type, pName), _3C(0), mZoneID(0) {}

// needed to get sCubeCategory emitted in sbss;
s32 CubeCameraArea::sCubeCategory = 0;

void CubeCameraArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    const char* valid;
    bool retVal = rIter.getValue("Validity", &valid);

    if (retVal) {
        if (!strcmp(valid, "Invalid")) {
            mIsValid = false;
        }
    }

    if (isValidSwitchA()) {
        mIsValid = false;
    }

    if (rIter.isValid()) {
        mZoneID = MR::getPlacedZoneId(rIter);
    } else {
        mZoneID = 0;
    }

    _3C = getCategoryArg();
    if (_3C < 0) {
        _3C = 0;
    }

    u32 temp = 1 << (_3C & 0xF);
    _3C = _3C >> 4;
    if (_3C == 0) {
        _3C |= temp;
    }

    MR::connectToSceneAreaObj(this);
}

void CubeCameraArea::movement() {
    bool val = false;

    if (mIsValid && _15 && mIsAwake) {
        val = true;
    }

    if (!val) {
        if (isValidSwitchA() && isOnSwitchA()) {
            mIsValid = true;
        }
    } else if (isValidSwitchB() && isOnSwitchB()) {
        mIsValid = false;
    }
}

bool CubeCameraArea::isValidCameraID() const {
    return mObjArg0 >= 0;
}

u16 CubeCameraArea::getCameraID() const {
    return mObjArg0;
}

bool CubeCameraArea::isInVolume(const TVec3f& rVec) const {
    return (_3C & sCubeCategory) != 0 && AreaObj::isInVolume(rVec);
}

void CubeCameraArea::setCurrentCategory(s32 category) {
    sCubeCategory = 1 << category;
}

s32 CubeCameraArea::getCategoryArg() const {
    return mObjArg3;
}

CubeCameraMgr::CubeCameraMgr(s32 type, const char* pName) : AreaObjMgr(type, pName) {
    CubeCameraArea::sCubeCategory = 1;
}

void CubeCameraMgr::initAfterLoad() {
    sort();
}

void CubeCameraMgr::sort() {
    if (mArray.size() != 0) {
        for (u32 i = 0; i < mArray.size() - 1; i++) {
            int swapIndex = i;
            AreaObj* swapObj = getArray(i);
            AreaObj* curObj = swapObj;
            for (u32 j = i + 1; j < mArray.size(); j++) {
                AreaObj* nextObj = getArray(j);
                if (swapObj->mObjArg2 > nextObj->mObjArg2) {
                    swapIndex = j;
                    swapObj = nextObj;
                }
            }
            
            if (swapIndex != i) {
                mArray[i] = swapObj;
                mArray[swapIndex] = curObj;
            }
        }
    }
}

CubeCameraMgr::~CubeCameraMgr() {}
