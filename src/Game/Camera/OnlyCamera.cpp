#include "Game/Camera/CameraPoseParam.hpp"
#include "Game/Camera/OnlyCamera.hpp"

OnlyCamera::OnlyCamera(const char *pName) : NameObj(pName) {
    mPoseParam = new CameraPoseParam();
    _10 = true;
    _11 = false;
    _14 = 0.0f;
    _18 = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _28 = 0;
    _2C = true;
    _2D = true;
    _30 = 0.0f;
    _34 = 0.0f;
    _38 = 0.0f;
    _3C = false;
    _3D = false;
}

OnlyCamera::~OnlyCamera() {
    
}