#include "Game/AreaObj/CubeCamera.hpp"

#include <cstring>

CubeCameraArea::CubeCameraArea(int a1, const char *pName) : AreaObj(a1, pName) {
    _3C = 0;
    mZoneID = 0;
}

/*
void CubeCameraArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);

    JMapInfo* info = rIter.mInfo;

    s32 inf = info->searchItemInfo("Validity");
    bool retVal;
    const char* valid;

    if (inf >= 0) {
        retVal = info->getValueFast(rIter._4, inf, &valid);
    }
    else {
        retVal = false;
    }

    if (retVal) {
        if (!strcmp(valid, "Invalid")) {
            mValid = false;
        }
    }

    if (isValidSwitchA()) {
        mValid = false;
    }

    int r4 = 0;
    bool r3 = false;

    if (rIter.mInfo) {
        if (rIter._4 >= 0) {
            r3 = true;
        }
    }

    if (r3) {

    }
}
*/

void CubeCameraArea::movement() {
    bool val = false;

    if (mValid && _15 && mAwake) {
        val = true;
    }

    if (!val) {
        if (isValidSwitchA()) {
            if (isOnSwitchA()) {
                mValid = true;
            }
        }
    }
    else {
        if (isValidSwitchB()) {
            if (isOnSwitchB()) {
                mValid = false;
            }
        }
    }
}

bool CubeCameraArea::isValidCameraID() const {
    return mObjArg0 >= 0;
}

u16 CubeCameraArea::getCameraID() const {
    return mObjArg0;
}

bool CubeCameraArea::isInVolume(const TVec3f &rVec) const {
    bool ret = false;

    if ((_3C & sCubeCategory)) {
        if (AreaObj::isInVolume(rVec)) {
            ret = true;
        }
    }

    return ret;
}

void CubeCameraArea::setCurrentCategory(s32 category) {
    sCubeCategory = 1 << category;
}

s32 CubeCameraArea::getCategoryArg() const {
    return mObjArg3;
}

CubeCameraMgr::CubeCameraMgr(s32 type, const char *pName) : AreaObjMgr(type, pName) {
    CubeCameraArea::sCubeCategory = 1;
}

void CubeCameraMgr::initAfterLoad() {
    sort();
}

// CubeCameraMgr::sort

CubeCameraMgr::~CubeCameraMgr() {
    
}