#include "Game/Camera/CameraWaterFollow.hpp"
#include "Game/Camera/CamTranslatorWaterFollow.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "JSystem/JGeometry/TVec.hpp"

CameraWaterFollow::CameraWaterFollow(const char* pName) : Camera(pName) {
    mAxisY = 300.0f;
    mAxisX = 1200.0f;
    mDist = 0.01f;
    _58 = 0.01f;
    _5C = 0;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 0.0f;
    _6C = 0;
    _70 = 0;
    _74 = 1.0f;
    _78 = 0.0f;
    _7C = 0.0f;
}

void CameraWaterFollow::reset() {
    _6C = 0;
    _58 = mDist;
    _5C = 0;
    _68 = 0.0f;
    _64 = 0.0f;
    _60 = 0.0f;
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setUpVecAndWatchUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
}

CameraWaterFollow::~CameraWaterFollow() {}

CamTranslatorBase* CameraWaterFollow::createTranslator() {
    return new CamTranslatorWaterFollow(this);
}

bool CameraWaterFollow::isEnableToReset() const {
    return true;
}
