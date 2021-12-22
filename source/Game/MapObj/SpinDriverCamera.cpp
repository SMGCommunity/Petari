#include "Game/Camera/CameraTargetArg.h"

CameraTargetArg::CameraTargetArg(CameraTargetMtx *pTargetMtx) {
    mTargetMtx = pTargetMtx;
    mTargetObj = NULL;
    mLiveActor = NULL;
    mMarioActor = NULL;
}