#include "Game/Camera/CameraCharmedVecReg.h"

CameraCharmedVecReg::CameraCharmedVecReg(const char *pName) : Camera(pName) {
    _4C = 0;
    _50 = 1000.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 0.0f;
    _60 = 0.5f;
    _64 = 0.02f;
}