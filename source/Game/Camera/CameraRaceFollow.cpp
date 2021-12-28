#include "Game/Camera/CameraRaceFollow.h"

CameraRaceFollow::CameraRaceFollow(const char *pName) : Camera(pName) {
    mWPointX = 500.0f;
    mWPointY = 1200.0f;
    mAngleA = 0.2617994f;
    _58 = false;
    mWPointZ = 0.0f;
}