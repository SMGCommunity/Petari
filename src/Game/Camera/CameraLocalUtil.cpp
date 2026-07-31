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

void CameraLocalUtil_FORCE_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

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
        return getCameraDirector()->isForceCameraChange();
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

    f32 getFrontOffset(const Camera* pCamera) {
        return pCamera->mPoseParam->mFrontOffset;
    }

    f32 getUpperOffset(const Camera* pCamera) {
        return pCamera->mPoseParam->mUpperOffset;
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

    void recalcUpVec(TVec3f* pUp, const TVec3f& rFront) {
        TVec3f side = pUp->cross(rFront);
        MR::normalize(&side);
        pUp->cross(rFront, side);
        MR::normalize(pUp);
    }

    void makeWatchOffset(TVec3f* pDst, Camera* pCamera, CameraTargetObj* pTarget, f32 scale) {
        f32 offset;
        if (pCamera->mIsLOfsErpOff || pCamera->mCameraMan->_15) {
            offset = 1.0f;
        } else {
            offset = pTarget->getLastMove().length() * scale;
            if (offset > 1.0f) {
                offset = 1.0f;
            }
        }

        TVec3f localOffs = getLocalOffset(pCamera);
        localOffs += (pTarget->getFrontVec() * getFrontOffset(pCamera) + pTarget->getUpVec() * getUpperOffset(pCamera) - localOffs) * offset;
        setLocalOffset(pCamera, localOffs);

        TVec3f globalOffs = getGlobalOffset(pCamera);
        pCamera->mZoneMatrix.mult33(globalOffs);
        pDst->set(globalOffs + localOffs);
    }

    void makeWatchPoint(TVec3f* pDst, Camera* pCamera, CameraTargetObj* pTarget, f32 param4) {
        makeWatchOffset(pDst, pCamera, pTarget, param4);
        pDst->add(pTarget->getPosition());
    }

    void makeWatchOffsetImm(TVec3f* pDst, Camera* pCamera, CameraTargetObj* pTarget) {
        TVec3f offs = pTarget->getFrontVec() * getFrontOffset(pCamera) + pTarget->getUpVec() * getUpperOffset(pCamera);
        setLocalOffset(pCamera, offs);

        TVec3f globOffs = getGlobalOffset(pCamera);
        pCamera->mZoneMatrix.mult33(globOffs);
        pDst->set(globOffs + offs);
    }

    void makeWatchPointImm(TVec3f* pDst, Camera* pCamera, CameraTargetObj* pTarget) {
        makeWatchOffsetImm(pDst, pCamera, pTarget);
        pDst->add(pTarget->getPosition());
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

    void slerpCamera(TQuat4f* pDst, const TQuat4f& rA, const TQuat4f& rB, f32 ratio, bool reverse) {
        TQuat4f rotA, rotB;
        rotA.normalize(rA);
        rotB.normalize(rB);
        f32 cos = rotA.dot(rotB);
        bool neg;
        if (cos < 0.0f) {
            neg = true;
            cos = -cos;
        } else {
            neg = false;
        }

        f32 sratio;
        if (1.0f - cos <= 0.00001f) {
            sratio = 1.0f - ratio;
        } else {
            f32 angle = MR::acos(cos);
            if (reverse) {
                angle -= MR::pi() * 2;
            }
            f32 sin = MR::sin(angle);
            sratio = MR::sin((1.0f - ratio) * angle) / sin;
            ratio = MR::sin(ratio * angle) / sin;
        }

        if (neg) {
            ratio = -ratio;
        }

        pDst->set(sratio * rotA.x + ratio * rotB.x, sratio * rotA.y + ratio * rotB.y, sratio * rotA.z + ratio * rotB.z,
                  sratio * rotA.w + ratio * rotB.w);
    }

    bool makeTowerCameraMtx(TPos3f* pMtx, const TPos3f& rMtx, const TVec3f& rPos, const TVec3f& rUp, const TVec3f& rWatchPos) {
        TVec3f pos = rPos;
        TVec3f up = rUp;
        rMtx.mult(pos, pos);
        rMtx.mult33(up);

        TVec3f diff = rWatchPos - pos;
        TVec3f front = diff.killElement(up);

        if (MR::isNearZero(front)) {
            return false;
        }

        MR::normalize(&front);
        TVec3f side = up.cross(front);
        pMtx->setXDir(side);
        pMtx->setYDir(up);
        pMtx->setZDir(front);
        pMtx->setTrans(rWatchPos);
        return true;
    }

    void arrangeDistanceByFovy(Camera* pCamera, TVec3f pos, f32 offset) {
        // FIXME: regswaps
        // https://decomp.me/scratch/jeret
        TVec3f viewDir = getPos(pCamera) - getWatchPos(pCamera);
        if (MR::isNearZero(viewDir)) {
            return;
        }

        f32 viewDist = viewDir.length();
        MR::normalize(&viewDir);
        TVec3f posDiff = pos - getWatchPos(pCamera);
        f32 projDist = posDiff.dot(viewDir);
        TVec3f projZ = viewDir * projDist;

        f32 dist = projDist + (offset + posDiff.distance(projZ)) / MR::tanDegree(MR::getFovy() * 0.5f);
        if (dist > viewDist) {
            setPos(pCamera, viewDir * dist + getWatchPos(pCamera));
        }
    }

    void arrangeDistanceByPushAndPull(Camera* pCamera, f32 push, f32 pull) {
        TVec3f viewDir = getPos(pCamera) - getWatchPos(pCamera);
        f32 viewDist = viewDir.length();

        if (viewDist < push) {
            if (MR::isNearZero(viewDir)) {
                MR::getCameraInvViewMtx()->getZDir(viewDir);
                viewDir.negate();
            }
            viewDir.setLength(push);
        } else if (viewDist > pull) {
            viewDir.setLength(pull);
        }

        setPos(pCamera, getWatchPos(pCamera) + viewDir);
    }

    void calcSafePose(CameraMan* pCameraMan, Camera* pCamera) {
        // FIXME: TVec stack order
        // https://decomp.me/scratch/HxpaL

        TVec3f pos = getPos(pCamera);
        TVec3f watchPos = getWatchPos(pCamera);
        TVec3f up = getUpVec(pCamera);

        if (MR::isNan(pos) || MR::isNan(watchPos) || MR::isNan(up)) {
            return;
        }

        TVec3f watchOffset = watchPos - pos;
        f32 dist = watchOffset.length();
        if (dist < 300.0f) {
            if (dist < 1.0f) {
                watchPos.set(pos + getWatchPos(pCameraMan) - getPos(pCameraMan));
            } else {
                watchOffset.normalize();
                watchPos.set(pos + watchOffset * 300.0f);
            }
        }

        TVec3f camWatchDir = watchPos - pos;
        MR::normalize(&camWatchDir);
        MR::normalizeOrZero(&up);

        if (MR::isNearZero(up) || __fabsf(camWatchDir.dot(up)) > 0.98f) {
            TVec3f watchDir = getWatchPos(pCameraMan) - getPos(pCameraMan);
            MR::normalize(&watchDir);
            if (__fabsf(camWatchDir.dot(watchDir)) > 0.98f) {
                up.set(getUpVec(pCameraMan));
            } else {
                TQuat4f rot;
                rot.setRotate(watchDir, camWatchDir);
                rot.transform(getUpVec(pCameraMan), up);
            }
            recalcUpVec(&up, camWatchDir);
        }

        setPos(pCameraMan, pos);
        setUpVec(pCameraMan, up);
        setWatchPos(pCameraMan, watchPos);
        setWatchUpVec(pCameraMan, getWatchUpVec(pCamera));
        setGlobalOffset(pCameraMan, getGlobalOffset(pCamera));
        setLocalOffset(pCameraMan, getLocalOffset(pCamera));
        setFovy(pCameraMan, getFovy(pCamera));
        setRoll(pCameraMan, getRoll(pCamera));
    }
};  // namespace CameraLocalUtil

void DUMMY() {
    TPos3f m;
    TVec3f a, b, c, d;
    m.setXDir(a);
    m.setYDir(b);
    m.setZDir(c);
    m.setTrans(d);
}
