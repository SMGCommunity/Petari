#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CameraPoseParam.hpp"
#include "Game/Util/SceneUtil.hpp"

Camera::Camera(const char* pName) : NameObj(pName) {
    mPoseParam = new CameraPoseParam();
    mVPan = nullptr;
    mIsLOfsErpOff = false;
    mZoneMatrix.identity();
}

void Camera::setZoneMtx(s32 zoneID) {
    if (zoneID <= 0) {
        mZoneMatrix.identity();
    } else {
        mZoneMatrix.identity();
        TMtx34f* matrix = MR::getZonePlacementMtx(zoneID);
        mZoneMatrix.setInline(*matrix);
    }

    if (doesVPanExist()) {
        mVPan->_60 = 1;
    }
}

CamTranslatorBase* Camera::createTranslator() {
    return new CamTranslatorDummy(this);
}

void Camera::createVPanObj() {
    mVPan = new CameraHeightArrange(this);
}
