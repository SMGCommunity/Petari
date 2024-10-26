#include "Game/Boss/SkeletalFishJointCalc.hpp"

void SkeletalFishJointCalc::calcJoint(TPos3f *pOutMtx, const TPos3f *pInMtx, SkeletalFishRailControl *pRailCtrl, const JointControllerInfo &rJointInfo) {
    pOutMtx->concat(*pInMtx, *pOutMtx);
    TVec3f pos;
    f32 z = pOutMtx->mMtx[2][3];
    f32 y = pOutMtx->mMtx[1][3];
    f32 x = pOutMtx->mMtx[0][3];
    pos.set(x, y, z);
    TMtx34f mtx;
    f32 negZ = -pos.z;

    mtx.mMtx[0][0] = 1.0f;
    mtx.mMtx[1][0] = 0.0f;
    mtx.mMtx[2][0] = 0.0f;
    mtx.mMtx[0][1] = 0.0f;
    mtx.mMtx[1][1] = 1.0f;
    mtx.mMtx[2][1] = 0.0f;
    mtx.mMtx[0][2] = 0.0f;
    mtx.mMtx[1][2] = 0.0f;
    mtx.mMtx[2][2] = 1.0f;
    mtx.mMtx[0][3] = 0.0f;
    mtx.mMtx[1][3] = 0.0f;
    mtx.mMtx[2][3] = negZ;

    pOutMtx->concat(mtx, *pOutMtx);
    TPos3f railMtx;
    pRailCtrl->getMtx(&railMtx, -pos.z);
    pOutMtx->concat(railMtx, *pOutMtx);
}