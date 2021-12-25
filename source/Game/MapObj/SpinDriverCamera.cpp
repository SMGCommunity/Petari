#include "Game/Camera/CameraTargetArg.h"

CameraTargetArg::CameraTargetArg(CameraTargetMtx *pTargetMtx) {
    mTargetObj = NULL;
    mTargetMtx = pTargetMtx;
    mLiveActor = NULL;
    mMarioActor = NULL;
}