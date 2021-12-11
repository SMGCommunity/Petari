#include "Game/Camera/CameraHeightArrange.h"

CameraHeightArrange::CameraHeightArrange(Camera *pCamera) : NameObj("CameraHeightArrange") {
    mCamera = pCamera;
    _10 = 0;
    _11 = 0;
    _12 = 0;
    _14 = new CameraPoseParam();
    _18 = new CameraPoseParam();
    _28 = 0.0f;
    _2C = 0.0f;
    _30 = 0.0f;
    _34 = 0.0f;
    _38 = 0.0f;
    _3C = 0.0f;                           
    _40 = 0;
    _44 = 0;
    _4C = 0;
    _50 = 0;
    _54 = 0;
    _58 = 0;
    _5C = 0;
    _60 = 0;
    _64 = 0.0f;
    _68 = 1.0f;
    _6C = 0.0f;
    _70 = 0.1f;
    _74 = 15.0f;
    _78 = 0.3f;
    _7C = 0.1f;
    _80 = 160;
    _84 = 500.0f;
    _88 = 800.0f;
    _8C = 120;
    _90 = 120;
    _94 = 120;
    _98 = 1;
    _9C = 0.0f;
    _A0 = 1.0f;
    _A4 = 0.0f;
}