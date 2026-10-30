#include "Game/Camera/CameraPolygonCodeUtil.hpp"
#include "Game/Camera/CameraDirector.hpp"

void MR::initCameraCodeCollection(const char* pName, s32 code) {
    MR::getCameraDirector()->initCameraCodeCollection(pName, code);
}

void MR::registerCameraCode(u32 code) {
    MR::getCameraDirector()->registerCameraCode(code);
}

void MR::termCameraCodeCollection() {
    MR::getCameraDirector()->termCameraCodeCollection();
}
