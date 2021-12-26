#include "Game/Camera/CameraTargetArg.h"
#include "Game/Camera/CameraTargetMtx.h"

CameraTargetArg::CameraTargetArg(CameraTargetMtx *pTargetMtx) {
    mTargetObj = NULL;
    mTargetMtx = pTargetMtx;
    mLiveActor = NULL;
    mMarioActor = NULL;
}

void CameraTargetMtx::setMtx(register MtxPtr mtx) {
    register TMtx34f &dst = mMatrix;

    __asm {
        psq_l   f0, 0x00(mtx), 0x0, 0
        psq_l   f1, 0x08(mtx), 0x0, 0
        psq_l   f2, 0x10(mtx), 0x0, 0
        psq_l   f3, 0x18(mtx), 0x0, 0
        psq_l   f4, 0x20(mtx), 0x0, 0
        psq_l   f5, 0x28(mtx), 0x0, 0
        psq_st  f0, 0x00(dst), 0x0, 0
        psq_st  f1, 0x08(dst), 0x0, 0
        psq_st  f2, 0x10(dst), 0x0, 0
        psq_st  f3, 0x18(dst), 0x0, 0
        psq_st  f4, 0x20(dst), 0x0, 0
        psq_st  f5, 0x28(dst), 0x0, 0
    }
}