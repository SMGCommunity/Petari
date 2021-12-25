#include "Game/Camera/CameraPoseParam.h"
#include "Game/Camera/OnlyCamera.h"

OnlyCamera::OnlyCamera(const char *pName) : NameObj(pName) {
    mPoseParam = new CameraPoseParam();
    _10 = 1;
    _11 = 0;
    _14 = 0.0f;
    _18 = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _28 = 0;
    _2C = 1;
    _2D = 1;
    _30 = 0.0f;
    _34 = 0.0f;
    _38 = 0.0f;
    _3C = false;
    _3D = 0;
}

OnlyCamera::~OnlyCamera() {
    
}