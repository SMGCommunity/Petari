#include "Game/AreaObj/CubeCamera.hpp"

#include <cstring>

CubeCameraArea::CubeCameraArea(int type, const char* pName) : AreaObj(type, pName), _3C(0), mZoneID(0) {}


void CubeCameraArea::init(const JMapInfoIter& rIter) {
    //non-matching
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

    int r4 = 0;

    if (rIter.isValid()) {

    }
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

// CubeCameraMgr::sort

CubeCameraMgr::~CubeCameraMgr() {}
