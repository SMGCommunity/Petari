#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "Game/Camera/CameraPoseParam.hpp"
#include "Game/Camera/CameraRegisterHolder.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"

namespace CameraLocalUtil {
    const TVec3f& getWatchPos(const CameraMan* pCameraMan) {
        return pCameraMan->mPoseParam->mWatchPos;
    }

    const TVec3f& getPos(const CameraMan* pCameraMan) {
        return pCameraMan->mPoseParam->mPos;
    }

    const TVec3f& getUpVec(const CameraMan* pCameraMan) {
        return pCameraMan->mPoseParam->mUpVec;
    }

    const TVec3f& getWatchUpVec(const CameraMan* pCameraMan) {
        return pCameraMan->mPoseParam->mWatchUpVec;
    }

    f32 getFovy(const CameraMan* pCameraMan) {
        return pCameraMan->mPoseParam->mFovy;
    }

    const TVec3f& getGlobalOffset(const CameraMan* pCameraMan) {
        return pCameraMan->mPoseParam->mGlobalOffset;
    }

    const TVec3f& getLocalOffset(const CameraMan* pCameraMan) {
        return pCameraMan->mPoseParam->mLocalOffset;
    }

    f32 getRoll(const CameraMan* pCameraMan) {
        return pCameraMan->mPoseParam->mRoll;
    }

    void setWatchPos(CameraMan* pCameraMan, const TVec3f& rWatchPos) {
        pCameraMan->mPoseParam->mWatchPos.set(rWatchPos);
    }

    void setPos(CameraMan* pCameraMan, const TVec3f& rPos) {
        pCameraMan->mPoseParam->mPos.set(rPos);
    }

    void setUpVec(CameraMan* pCameraMan, const TVec3f& rUpVec) {
        pCameraMan->mPoseParam->mUpVec.set(rUpVec);
    }

    void setWatchUpVec(CameraMan* pCameraMan, const TVec3f& rWatchUpVec) {
        pCameraMan->mPoseParam->mWatchUpVec.set(rWatchUpVec);
    }

    void setFovy(CameraMan* pCameraMan, f32 fovy) {
        pCameraMan->mPoseParam->mFovy = fovy;
    }

    void setGlobalOffset(CameraMan* pCameraMan, const TVec3f& rGlobalOffset) {
        pCameraMan->mPoseParam->mGlobalOffset.set(rGlobalOffset);
    }

    void setLocalOffset(CameraMan* pCameraMan, const TVec3f& rLocalOffset) {
        pCameraMan->mPoseParam->mLocalOffset.set(rLocalOffset);
    }

    void setFrontOffset(CameraMan* pCameraMan, f32 frontOffset) {
        pCameraMan->mPoseParam->mFrontOffset = frontOffset;
    }

    void setUpperOffset(CameraMan* pCameraMan, f32 upperOffset) {
        pCameraMan->mPoseParam->mUpperOffset = upperOffset;
    }

    void setRoll(CameraMan* pCameraMan, f32 roll) {
        pCameraMan->mPoseParam->mRoll = roll;
    }

    CameraTargetObj* getTarget(const CameraMan* pCameraMan) {
        return pCameraMan->mDirector->getTarget();
    }

    const MtxPtr getMtxReg(const char* pRegName) {
        return getCameraDirector()->mRegisterHolder->getMtx(pRegName);
    }

    const TVec3f* getVecReg(const char* pRegName) {
        return getCameraDirector()->mRegisterHolder->getVec(pRegName);
    }

    const char* getDummyVecRegName() {
        return getCameraDirector()->mRegisterHolder->getDummyVecRegName();
    }

    bool isForceCameraChange() {
        return MR::getCameraDirector()->isForceCameraChange();
    }

    CameraDirector* getCameraDirector() {
        return MR::getCameraDirector();
    }

    void setUsedTarget(const CameraMan* pCameraMan, CameraTargetObj* pUsedTarget) {
        pCameraMan->mDirector->mUsedTarget = pUsedTarget;
    }

    CameraTargetObj* getTarget(const Camera* pCamera) {
        return pCamera->mCameraMan->mDirector->getTarget();
    }

    const TVec3f& getWatchPos(const Camera* pCamera) {
        return pCamera->mPoseParam->mWatchPos;
    }

    const TVec3f& getPos(const Camera* pCamera) {
        return pCamera->mPoseParam->mPos;
    }

    const TVec3f& getUpVec(const Camera* pCamera) {
        return pCamera->mPoseParam->mUpVec;
    }

    const TVec3f& getWatchUpVec(const Camera* pCamera) {
        return pCamera->mPoseParam->mWatchUpVec;
    }

    f32 getFovy(const Camera* pCamera) {
        return pCamera->mPoseParam->mFovy;
    }

    const TVec3f& getGlobalOffset(const Camera* pCamera) {
        return pCamera->mPoseParam->mGlobalOffset;
    }

    const TVec3f& getLocalOffset(const Camera* pCamera) {
        return pCamera->mPoseParam->mLocalOffset;
    }

    f32 getRoll(const Camera* pCamera) {
        return pCamera->mPoseParam->mRoll;
    }

    void setWatchPos(Camera* pCamera, const TVec3f& rWatchPos) {
        pCamera->mPoseParam->mWatchPos.set(rWatchPos);
    }

    void setPos(Camera* pCamera, const TVec3f& rPos) {
        pCamera->mPoseParam->mPos.set(rPos);
    }

    void setUpVec(Camera* pCamera, const TVec3f& rUpVec) {
        pCamera->mPoseParam->mUpVec.set(rUpVec);
    }

    void setWatchUpVec(Camera* pCamera, const TVec3f& rWatchUpVec) {
        pCamera->mPoseParam->mWatchUpVec.set(rWatchUpVec);
    }

    void setUpVecAndWatchUpVec(Camera* pCamera, const TVec3f& rUpVec) {
        CameraPoseParam* pPoseParam = pCamera->mPoseParam;

        pPoseParam->mUpVec.set(rUpVec);
        pPoseParam->mWatchUpVec.set(rUpVec);
    }

    void setFovy(Camera* pCamera, f32 fovy) {
        pCamera->mPoseParam->mFovy = fovy;
    }

    void setGlobalOffset(Camera* pCamera, const TVec3f& rGlobalOffset) {
        pCamera->mPoseParam->mGlobalOffset.set(rGlobalOffset);
    }

    void setLocalOffset(Camera* pCamera, const TVec3f& rLocalOffset) {
        pCamera->mPoseParam->mLocalOffset.set(rLocalOffset);
    }

    void setFrontOffset(Camera* pCamera, f32 frontOffset) {
        pCamera->mPoseParam->mFrontOffset = frontOffset;
    }

    void setUpperOffset(Camera* pCamera, f32 upperOffset) {
        pCamera->mPoseParam->mUpperOffset = upperOffset;
    }

    void setRoll(Camera* pCamera, f32 roll) {
        pCamera->mPoseParam->mRoll = roll;
    }

    void recalcUpVec(TVec3f* pDst, const TVec3f& rSrc) {
        TVec3f temp;

        PSVECCrossProduct(pDst, &rSrc, &temp);
        MR::normalize(&temp);
        PSVECCrossProduct(&rSrc, &temp, pDst);
        MR::normalize(pDst);
    }

    // makeWatchOffset

    void makeWatchPoint(TVec3f* pDst, Camera* pCamera, CameraTargetObj* pTarget, f32 param4) {
        makeWatchOffset(pDst, pCamera, pTarget, param4);
        pDst->add(*pTarget->getPosition());
    }

    // makeWatchOffsetImm

    void makeWatchPointImm(TVec3f* pDst, Camera* pCamera, CameraTargetObj* pTarget) {
        makeWatchOffsetImm(pDst, pCamera, pTarget);
        pDst->add(*pTarget->getPosition());
    }

    bool tryCameraReset() {
        if (getCameraDirector()->isEnableToReset()) {
            return testCameraPadButtonReset();
        }

        return false;
    }

    bool tryCameraResetTrigger() {
        if (!getCameraDirector()->isEnableToReset()) {
            return false;
        }

        if (getCameraDirector()->isSubjectiveCamera()) {
            return false;
        }

        return testCameraPadTriggerReset();
    }

    bool testCameraPadButtonReset() {
        if (getCameraDirector()->isSubjectiveCamera()) {
            return false;
        }

        return MR::testSubPadButtonC(WPAD_CHAN0);
    }

    bool testCameraPadTriggerReset() {
        if (getCameraDirector()->isSubjectiveCamera()) {
            return false;
        }

        return MR::testSubPadTriggerC(WPAD_CHAN0);
    }

    bool testCameraPadTriggerRoundLeft() {
        if (MR::isDemoActive()) {
            return false;
        }

        if (MR::isFirstPersonCamera()) {
            return false;
        }

        return MR::testCorePadTriggerLeft(WPAD_CHAN0);
    }

    bool testCameraPadTriggerRoundRight() {
        if (MR::isDemoActive()) {
            return false;
        }

        if (MR::isFirstPersonCamera()) {
            return false;
        }

        return MR::testCorePadTriggerRight(WPAD_CHAN0);
    }

    // slerpCamera
    // makeTowerCameraMtx
    // arrangeDistanceByFovy
    // arrangeDistanceByPushAndPull
    // calcSafePose
};  // namespace CameraLocalUtil
