#include "Game/Camera/CameraTargetObj.h"

CameraTargetObj::CameraTargetObj(const char *pName) : NameObj(pName) {
    mCameraWall = 0;
}

CameraTargetActor::CameraTargetActor(const char *pName) : CameraTargetObj(pName) {
    _10 = 0;
    _14 = 0.0f;
    _18 = 1.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _24 = 0.0f;
    _28 = 1.0f;
    _2C = 0.0f;
    _30 = 0.0f;
    _34 = 1.0f;
    _38 = 0;
}

CameraTargetPlayer::CameraTargetPlayer(const char * pName) : CameraTargetObj(pName) {
    _38 = 0.0f;
    _3C = -1.0f;
    _40 = 0.0f;
    _44 = 0.0f;
    _48 = 0.0f;
    _4C = 0.0f;
    _50 = 0;
    _54 = 0;
    _58 = 0;
    _5A = 1;
}