#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/ClippingActorHolder.hpp"
#include "Game/LiveActor/ClippingDirector.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <cstdio>

namespace {
    CollisionParts* createCollisionParts(ResourceHolder* pResHolder, const char* pName, HitSensor* pSensor, const TPos3f& rMtx, MR::CollisionScaleType scaleType, s32 unk) {
        CollisionParts* parts = new CollisionParts();
        char            kcl[0x80];
        snprintf(kcl, sizeof(kcl), "%s.kcl", pName);
        char pa[0x80];
        snprintf(pa, sizeof(pa), "%s.pa", pName);
        void* paRes = nullptr;
        void* kclRes = pResHolder->mFileInfoTable->getRes(kcl);
        if (pResHolder->mFileInfoTable->isExistRes(pa)) {
            paRes = pResHolder->mFileInfoTable->getRes(pa);
        }

        switch (scaleType) {
        case 0:
            parts->initWithAutoEqualScale(rMtx, pSensor, kclRes, paRes, unk, false);
            break;
        case 1:
            parts->initWithNotUsingScale(rMtx, pSensor, kclRes, paRes, unk, false);
            break;
        case 2:
        case 3:
            parts->init(rMtx, pSensor, kclRes, paRes, unk, false);
            break;
        }

        return parts;
    }
}; // namespace

namespace MR {
    bool isExistIndirectTexture(const LiveActor* pActor) {
        const char* name = "IndDummy";
        return MR::getJ3DModelData(pActor)->mMaterialTable.mTextureNameTable->getIndex(name) != -1;
    }

    void initDefaultPos(LiveActor* pActor, const JMapInfoIter& rIter) {
        if (!rIter.isValid()) {
            return;
        }

        MR::getJMapInfoTrans(rIter, &pActor->mPosition);
        MR::getJMapInfoRotate(rIter, &pActor->mRotation);
        MR::getJMapInfoScale(rIter, &pActor->mScale);

        f32 rot_x = fmod(360.0f + (pActor->mRotation.x) - 0.0f, 360.0f);
        f32 rot_y = fmod(360.0f + (pActor->mRotation.y) - 0.0f, 360.0f);
        pActor->mRotation.x = 0.0f + rot_x;
    }

    void initDefaultPosNoRepeat(LiveActor* pActor, const JMapInfoIter& rIter) {
        MR::getJMapInfoTrans(rIter, &pActor->mPosition);
        MR::getJMapInfoRotate(rIter, &pActor->mRotation);
        MR::getJMapInfoScale(rIter, &pActor->mScale);
    }

    bool isValidMovement(const LiveActor* pActor) {
        if (pActor->mFlag.mIsDead || pActor->mFlag.mIsClipped) {
            return false;
        }

        return true;
    }

    bool isValidCalcAnim(const LiveActor* pActor) {
        if (pActor->mFlag.mIsDead || pActor->mFlag.mIsClipped || pActor->mFlag.mIsNoCalcAnim) {
            return false;
        }

        return true;
    }

    bool isValidCalcViewAndEntry(const LiveActor* pActor) {
        if (pActor->mFlag.mIsDead || pActor->mFlag.mIsClipped) {
            return false;
        }

        return true;
    }

    bool isValidDraw(const LiveActor* pActor) {
        if (pActor->mFlag.mIsDead || pActor->mFlag.mIsClipped || pActor->mFlag.mIsHiddenModel) {
            return false;
        }

        return true;
    }

    void calcAnimDirect(LiveActor* pActor) {
        bool isNoCalcAnim = pActor->mFlag.mIsNoCalcAnim;
        pActor->mFlag.mIsNoCalcAnim = false;
        pActor->calcAnim();
        if (isNoCalcAnim) {
            pActor->mFlag.mIsNoCalcAnim = true;
        }
    }

    void setClippingTypeSphere(LiveActor* pActor, f32 rad) {
        MR::getClippingDirector()->mActorHolder->setTypeToSphere(pActor, rad, 0);
    }

    void setClippingTypeSphere(LiveActor* pActor, f32 rad, const TVec3f* a3) {
        MR::getClippingDirector()->mActorHolder->setTypeToSphere(pActor, rad, a3);
    }

    void setClippingTypeSphereContainsModelBoundingBox(LiveActor* pActor, f32 a2) {
        f32 radius = 0.0f;
        MR::calcModelBoundingRadius(&radius, pActor);
        f32 rad = radius + a2;
        MR::getClippingDirector()->mActorHolder->setTypeToSphere(pActor, rad, 0);
    }

    void setClippingFar50m(LiveActor* pActor) {
        MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 7);
    }

    void setClippingFar100m(LiveActor* pActor) {
        MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 6);
    }

    void setClippingFar200m(LiveActor* pActor) {
        MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 5);
    }

    void setClippingFar300m(LiveActor* pActor) {
        MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 4);
    }

    void setClippingFarMax(LiveActor* pActor) {
        MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 0);
    }

    void setClippingFar(LiveActor* pActor, f32 clipping) {
        s32 clip = clipping;

        switch (clip) {
        case 50:
            MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 7);
            break;
        case 100:
            MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 6);
            break;
        case 200:
            MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 5);
            break;
        case 300:
            MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 4);
            break;
        case 400:
            MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 3);
            break;
        case 500:
            MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 2);
            break;
        case 600:
            MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 1);
            break;
        case -1:
            MR::getClippingDirector()->mActorHolder->setFarClipLevel(pActor, 0);
            break;
        }
    }

    void setGroupClipping(LiveActor* pActor, const JMapInfoIter& rIter, int a3) {
        MR::getClippingDirector()->joinToGroupClipping(pActor, rIter, a3);
    }

    void validateClipping(LiveActor* pActor) {
        if (pActor->mFlag.mIsInvalidClipping) {
            MR::getClippingDirector()->mActorHolder->validateClipping(pActor);
        }
    }

    void invalidateClipping(LiveActor* pActor) {
        if (pActor->mFlag.mIsInvalidClipping) {
            if (pActor->mFlag.mIsClipped) {
                pActor->endClipped();
            }
        } else {
            MR::getClippingDirector()->mActorHolder->invalidateClipping(pActor);
        }
    }

    bool isBckStopped(const LiveActor* pActor) {
        return pActor->mModelManager->isBckStopped();
    }

    bool isBtkStopped(const LiveActor* pActor) {
        return pActor->mModelManager->isBtkStopped();
    }

    bool isBrkStopped(const LiveActor* pActor) {
        return pActor->mModelManager->isBrkStopped();
    }

    bool isBtpStopped(const LiveActor* pActor) {
        return pActor->mModelManager->isBtpStopped();
    }

    bool isBpkStopped(const LiveActor* pActor) {
        return pActor->mModelManager->isBpkStopped();
    }

    bool isBvaStopped(const LiveActor* pActor) {
        return pActor->mModelManager->isBvaStopped();
    }

    bool isBckOneTimeAndStopped(const LiveActor* pActor) {
        return pActor->mModelManager->isBckStopped();
    }

    bool isBrkOneTimeAndStopped(const LiveActor* pActor) {
        return pActor->mModelManager->isBrkStopped();
    }

    // isBckLooped
    // checkPassBckFrame
    // setBckFrameAtRandom
    // setBtkFrameAtRandom
    // setBckFrameAndStop
    // setBtkFrameAndStop
    // setBrkFrameAndStop
    // setBtpFrameAndStop
    // setBpkFrameAndStop
    // setBvaFrameAndStop
    // setBrkFrameEndAndStop

    void startBtkAndSetFrameAndStop(const LiveActor* pActor, const char* pBtkName, f32 frame) {
        startBtk(pActor, pBtkName);
        setBtkFrameAndStop(pActor, frame);
    }

    void startBrkAndSetFrameAndStop(const LiveActor* pActor, const char* pBrkName, f32 frame) {
        startBrk(pActor, pBrkName);
        setBrkFrameAndStop(pActor, frame);
    }

    void startBtpAndSetFrameAndStop(const LiveActor* pActor, const char* pBtpName, f32 frame) {
        startBtp(pActor, pBtpName);
        setBtpFrameAndStop(pActor, frame);
    }

    void startBtk(const LiveActor* pActor, const char* pBtkName) {
        pActor->mModelManager->startBtk(pBtkName);
    }

    void startBrk(const LiveActor* pActor, const char* pBrkName) {
        pActor->mModelManager->startBrk(pBrkName);
    }

    void startBtp(const LiveActor* pActor, const char* pBtpName) {
        pActor->mModelManager->startBtp(pBtpName);
    }

    void startBpk(const LiveActor* pActor, const char* pBpkName) {
        pActor->mModelManager->startBpk(pBpkName);
    }

    void startBva(const LiveActor* pActor, const char* pBvaName) {
        pActor->mModelManager->startBva(pBvaName);
    }

    // startBckIfExist
    // startBtkIfExist
    // startBrkIfExist
    // startBtpIfExist
    // startBpkIfExist
    // startBvaIfExist

    bool isBtkPlaying(const LiveActor* pActor, const char* pBtkName) {
        return pActor->mModelManager->isBtkPlaying(pBtkName);
    }

    bool isBrkPlaying(const LiveActor* pActor, const char* pBrkName) {
        return pActor->mModelManager->isBrkPlaying(pBrkName);
    }

    bool isBtpPlaying(const LiveActor* pActor, const char* pBtpName) {
        return pActor->mModelManager->isBtpPlaying(pBtpName);
    }

    bool isBpkPlaying(const LiveActor* pActor, const char* pBpkName) {
        return pActor->mModelManager->isBpkPlaying(pBpkName);
    }

    bool isBvaPlaying(const LiveActor* pActor, const char* pBvaName) {
        return pActor->mModelManager->isBvaPlaying(pBvaName);
    }

    bool isBckExist(const LiveActor* pActor, const char* pBckName) {
        ResTable* pBckResTable = getResourceHolder(pActor)->mMotionResTable;

        if (pBckResTable->mCount != 0) {
            if (pBckResTable->isExistRes(pBckName)) {
                return true;
            }
        }

        return false;
    }

    bool isBtkExist(const LiveActor* pActor, const char* pBtkName) {
        ResTable* pBtkResTable = getResourceHolder(pActor)->mBtkResTable;

        if (pBtkResTable->mCount != 0) {
            if (pBtkResTable->isExistRes(pBtkName)) {
                return true;
            }
        }

        return false;
    }

    bool isBrkExist(const LiveActor* pActor, const char* pBrkName) {
        ResTable* pBrkResTable = getResourceHolder(pActor)->mBrkResTable;

        if (pBrkResTable->mCount != 0) {
            if (pBrkResTable->isExistRes(pBrkName)) {
                return true;
            }
        }

        return false;
    }

    bool isBpkExist(const LiveActor* pActor, const char* pBpkName) {
        ResTable* pBpkResTable = getResourceHolder(pActor)->mBpkResTable;

        if (pBpkResTable->mCount != 0) {
            if (pBpkResTable->isExistRes(pBpkName)) {
                return true;
            }
        }

        return false;
    }

    bool isBtpExist(const LiveActor* pActor, const char* pBtpName) {
        ResTable* pBtpResTable = getResourceHolder(pActor)->mBtpResTable;

        if (pBtpResTable->mCount != 0) {
            if (pBtpResTable->isExistRes(pBtpName)) {
                return true;
            }
        }

        return false;
    }

    bool isBvaExist(const LiveActor* pActor, const char* pBvaName) {
        ResTable* pBvaResTable = getResourceHolder(pActor)->mBvaResTable;

        if (pBvaResTable->mCount != 0) {
            if (pBvaResTable->isExistRes(pBvaName)) {
                return true;
            }
        }

        return false;
    }

    // stopBck

    void stopBtk(const LiveActor* pActor) {
        pActor->mModelManager->stopBtk();
    }

    void stopBrk(const LiveActor* pActor) {
        pActor->mModelManager->stopBrk();
    }

    void stopBtp(const LiveActor* pActor) {
        pActor->mModelManager->stopBtp();
    }

    void stopBva(const LiveActor* pActor) {
        pActor->mModelManager->stopBva();
    }

    // setBckRate
    // setBtkRate
    // setBrkRate
    // setBvaRate
    // setBckFrame
    // setBtkFrame
    // setBrkFrame
    // setBtpFrame
    // setBpkFrame
    // setBvaFrame
    // isBckPlaying
    // getBckCtrl
    // getBtkCtrl
    // getBrkCtrl
    // getBtpCtrl
    // getBpkCtrl
    // getBvaCtrl

    void updateMaterial(LiveActor* pActor) {
        pActor->mModelManager->updateDL(true);
    }

    // setMirrorReflectionInfoFromMtxYUp
    // setMirrorReflectionInfoFromModel
    // changeModelDataTexAll

    void initJointTransform(const LiveActor* pActor) {
        pActor->mModelManager->initJointTransform();
    }

    // getJointTransform
    // setJointTransformLocalMtx
    // getBckFrame
    // getBrkFrame
    // getBtpFrame
    // getBvaFrame
    // getBckRate
    // getBckFrameMax
    // getBtkFrameMax
    // getBrkFrameMax
    // reflectBckCtrlData

    void initLightCtrl(LiveActor* pActor) {
        pActor->initActorLightCtrl();
        pActor->mActorLightCtrl->init(-1, false);
    }

    void initLightCtrlForPlayer(LiveActor* pActor) {
        pActor->initActorLightCtrl();
        pActor->mActorLightCtrl->init(-1, false);
        LightFunction::registerPlayerLightCtrl(pActor->mActorLightCtrl);
    }

    void initLightCtrlNoDrawEnemy(LiveActor* pActor) {
        pActor->initActorLightCtrl();
        pActor->mActorLightCtrl->init(1, true);
    }

    void initLightCtrlNoDrawMapObj(LiveActor* pActor) {
        pActor->initActorLightCtrl();
        pActor->mActorLightCtrl->init(3, true);
    }

    void updateLightCtrl(LiveActor* pActor) {
        pActor->mActorLightCtrl->update(false);
    }

    void updateLightCtrlDirect(LiveActor* pActor) {
        pActor->mActorLightCtrl->update(true);
    }

    void loadActorLight(const LiveActor* pActor) {
        pActor->mActorLightCtrl->loadLight();
    }

    void calcLightPos0(TVec3f* pPos, const LiveActor* pActor) {
        LightFunction::calcLightWorldPos(pPos, pActor->mActorLightCtrl->getActorLight()->mInfo0);
    }

    void calcLightPos1(TVec3f* pPos, const LiveActor* pActor) {
        LightFunction::calcLightWorldPos(pPos, pActor->mActorLightCtrl->getActorLight()->mInfo1);
    }

    const GXColor* getLightAmbientColor(const LiveActor* pActor) {
        return &pActor->mActorLightCtrl->getActorLight()->mColor;
    }

    ActorLightCtrl* getLightCtrl(const LiveActor* pActor) {
        return pActor->mActorLightCtrl;
    }

    bool isStep(const LiveActor* pActor, s32 step) {
        return pActor->getNerveStep() == step;
    }

    bool isFirstStep(const LiveActor* pActor) {
        return isStep(pActor, 0);
    }

    bool isLessStep(const LiveActor* pActor, s32 step) {
        return pActor->getNerveStep() < step;
    }

    bool isLessEqualStep(const LiveActor* pActor, s32 step) {
        return pActor->getNerveStep() <= step;
    }

    bool isGreaterStep(const LiveActor* pActor, s32 step) {
        return pActor->getNerveStep() > step;
    }

    bool isGreaterEqualStep(const LiveActor* pActor, s32 step) {
        return pActor->getNerveStep() >= step;
    }

    bool isIntervalStep(const LiveActor* pActor, s32 step) {
        return pActor->getNerveStep() % step == 0;
    }

    bool isNewNerve(const LiveActor* pActor) {
        return pActor->getNerveStep() < 0;
    }

    f32 calcNerveRate(const LiveActor* pActor, s32 stepMax) {
        return stepMax <= 0 ? 1.0f : clamp(static_cast<f32>(pActor->getNerveStep()) / stepMax, 0.0f, 1.0f);
    }

    f32 calcNerveRate(const LiveActor* pActor, s32 stepMin, s32 stepMax) {
        return clamp(normalize(pActor->getNerveStep(), stepMin, stepMax), 0.0f, 1.0f);
    }

    f32 calcNerveEaseInRate(const LiveActor* pActor, s32 stepMax) {
        return getEaseInValue(calcNerveRate(pActor, stepMax), 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveEaseOutRate(const LiveActor* pActor, s32 stepMax) {
        return getEaseOutValue(calcNerveRate(pActor, stepMax), 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveEaseOutRate(const LiveActor* pActor, s32 stepMin, s32 stepMax) {
        return getEaseOutValue(calcNerveRate(pActor, stepMin, stepMax), 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveEaseInOutRate(const LiveActor* pActor, s32 stepMax) {
        return getEaseInOutValue(calcNerveRate(pActor, stepMax), 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveEaseInOutRate(const LiveActor* pActor, s32 stepMin, s32 stepMax) {
        return getEaseInOutValue(calcNerveRate(pActor, stepMin, stepMax), 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveValue(const LiveActor* pActor, s32 stepMax, f32 valueStart, f32 valueEnd) {
        return getLinerValue(calcNerveRate(pActor, stepMax), valueStart, valueEnd, 1.0f);
    }

    f32 calcNerveValue(const LiveActor* pActor, s32 stepMin, s32 stepMax, f32 valueStart, f32 valueEnd) {
        return getLinerValue(calcNerveRate(pActor, stepMin, stepMax), valueStart, valueEnd, 1.0f);
    }

    f32 calcNerveEaseInValue(const LiveActor* pActor, s32 stepMax, f32 valueStart, f32 valueEnd) {
        return getEaseInValue(calcNerveRate(pActor, stepMax), valueStart, valueEnd, 1.0f);
    }

    f32 calcNerveEaseInValue(const LiveActor* pActor, s32 stepMin, s32 stepMax, f32 valueStart, f32 valueEnd) {
        return getEaseInValue(calcNerveRate(pActor, stepMin, stepMax), valueStart, valueEnd, 1.0f);
    }

    f32 calcNerveEaseOutValue(const LiveActor* pActor, s32 stepMax, f32 valueStart, f32 valueEnd) {
        return getEaseOutValue(calcNerveRate(pActor, stepMax), valueStart, valueEnd, 1.0f);
    }

    f32 calcNerveEaseInOutValue(const LiveActor* pActor, s32 stepMax, f32 valueStart, f32 valueEnd) {
        return getEaseInOutValue(calcNerveRate(pActor, stepMax), valueStart, valueEnd, 1.0f);
    }

    f32 calcNerveEaseInOutValue(const LiveActor* pActor, s32 stepMin, s32 stepMax, f32 valueStart, f32 valueEnd) {
        return getEaseInOutValue(calcNerveRate(pActor, stepMin, stepMax), valueStart, valueEnd, 1.0f);
    }

    void setNerveAtStep(LiveActor* pActor, const Nerve* pNerve, s32 step) {
        if (pActor->getNerveStep() == step) {
            pActor->setNerve(pNerve);
        }
    }

    void setNerveAtBckStopped(LiveActor* pActor, const Nerve* pNerve) {
        if (pActor->mModelManager->isBckStopped()) {
            pActor->setNerve(pNerve);
        }
    }

    bool trySetNerve(LiveActor* pActor, const Nerve* pNerve) {
        if (pActor->isNerve(pNerve)) {
            return false;
        }

        pActor->setNerve(pNerve);

        return true;
    }

    const TVec3f* getGroundNormal(const LiveActor* pActor) {
        return pActor->mBinder->mGroundInfo.mParentTriangle.getNormal(0);
    }

    const TVec3f* getWallNormal(const LiveActor* pActor) {
        return pActor->mBinder->mWallInfo.mParentTriangle.getNormal(0);
    }

    const TVec3f* getRoofNormal(const LiveActor* pActor) {
        return pActor->mBinder->mRoofInfo.mParentTriangle.getNormal(0);
    }

    const TVec3f* getBindedNormal(const LiveActor* pActor) {
        if (isBindedGround(pActor)) {
            return getGroundNormal(pActor);
        }

        if (isBindedWall(pActor)) {
            return getWallNormal(pActor);
        }

        if (isBindedRoof(pActor)) {
            return getRoofNormal(pActor);
        }

        return getGroundNormal(pActor);
    }

    const TVec3f* getBindedHitPos(const LiveActor* pActor) {
        if (isBindedGround(pActor)) {
            return getGroundHitPos(pActor);
        }

        if (isBindedWall(pActor)) {
            return getWallHitPos(pActor);
        }

        if (isBindedRoof(pActor)) {
            return getRoofHitPos(pActor);
        }

        return getGroundHitPos(pActor);
    }

    const TVec3f* getGroundHitPos(const LiveActor* pActor) {
        return &pActor->mBinder->mGroundInfo.mHitPos;
    }

    const TVec3f* getWallHitPos(const LiveActor* pActor) {
        return &pActor->mBinder->mWallInfo.mHitPos;
    }

    const TVec3f* getRoofHitPos(const LiveActor* pActor) {
        return &pActor->mBinder->mRoofInfo.mHitPos;
    }

    void calcWallNormalHorizontal(TVec3f* pVec, const LiveActor* pActor) {
        const TVec3f* normal = getWallNormal(pActor);
        const TVec3f* grav = &pActor->mGravity;
        f32           dot = grav->dot(*normal);
        JMAVECScaleAdd(grav, normal, pVec, -dot);
    }

    f32 calcHitPowerToGround(const LiveActor* pActor) {
        if (!isBindedGround(pActor)) {
            return 0.0f;
        }

        f32 dot = pActor->mVelocity.dot(*getGroundNormal(pActor));
        if (-dot > 0.0f) {
            return -dot;
        }

        return 0.0f;
    }

    f32 calcHitPowerToWall(const LiveActor* pActor) {
        if (!isBindedWall(pActor)) {
            return 0.0f;
        }

        f32 dot = pActor->mVelocity.dot(*getWallNormal(pActor));
        if (-dot > 0.0f) {
            return -dot;
        }

        return 0.0f;
    }

    int getBindedPlaneNum(const LiveActor* pActor) {
        return pActor->mBinder->mPlaneNum;
    }

    const TVec3f* getBindedPlaneNormal(const LiveActor* pActor, int planeIndex) {
        return pActor->mBinder->getPlane(planeIndex)->getNormal(0);
    }

    HitSensor* getBindedPlaneSensor(const LiveActor* pActor, int planeIndex) {
        return pActor->mBinder->getPlane(planeIndex)->mSensor;
    }

    TVec3f* getBindedFixReactionVector(const LiveActor* pActor) {
        return &pActor->mBinder->mFixReactionVector;
    }

    void setBinderOffsetVec(LiveActor* pActor, const TVec3f* pVec, bool b) {
        Binder* binder = pActor->mBinder;
        binder->mOffsetVec = pVec;
        binder->_1EC._4 = b;
    }

    void setBinderRadius(LiveActor* pActor, f32 radius) {
        pActor->mBinder->mRadius = radius;
    }

    f32 getBinderRadius(const LiveActor* pActor) {
        return pActor->mBinder->mRadius;
    }

    // setBinderCollisionPartsFilter
    // setBinderExceptActor

    void setBindTriangleFilter(LiveActor* pActor, TriangleFilterBase* pTriangleFilter) {
        pActor->mBinder->setTriangleFilter(pTriangleFilter);
    }

    bool isExistBinder(const LiveActor* pActor) {
        return pActor->mBinder != nullptr;
    }

    void onEntryDrawBuffer(LiveActor* pActor) {
        if (!isNoEntryDrawBuffer(pActor)) {
            return;
        }

        if (!isDead(pActor) && !pActor->mFlag.mIsClipped) {
            connectToDrawTemporarily(pActor);
        }

        pActor->mFlag.mIsHiddenModel = false;
    }

    void offEntryDrawBuffer(LiveActor* pActor) {
        if (isNoEntryDrawBuffer(pActor)) {
            return;
        }

        if (!isDead(pActor) && !pActor->mFlag.mIsClipped) {
            disconnectToDrawTemporarily(pActor);
        }

        pActor->mFlag.mIsHiddenModel = true;
    }

    bool isDead(const LiveActor* pActor) {
        return pActor->mFlag.mIsDead;
    }

    bool isHiddenModel(const LiveActor* pActor) {
        return pActor->mFlag.mIsHiddenModel;
    }

    void showModel(LiveActor* pActor) {
        if (isNoCalcAnim(pActor)) {
            onCalcAnim(pActor);
        }

        if (isNoCalcView(pActor)) {
            pActor->mFlag.mIsNoCalcView = false;
        }

        if (isNoEntryDrawBuffer(pActor)) {
            onEntryDrawBuffer(pActor);
        }
    }

    void hideModel(LiveActor* pActor) {
        if (!isNoCalcAnim(pActor)) {
            offCalcAnim(pActor);
        }

        if (!isNoCalcView(pActor)) {
            pActor->mFlag.mIsNoCalcView = true;
        }

        if (!isNoEntryDrawBuffer(pActor)) {
            offEntryDrawBuffer(pActor);
        }
    }

    void hideModelAndOnCalcAnim(LiveActor* pActor) {
        hideModel(pActor);
        onCalcAnim(pActor);
    }

    void showModelIfHidden(LiveActor* pActor) {
        if (isHiddenModel(pActor)) {
            showModel(pActor);
        }
    }

    void hideModelIfShown(LiveActor* pActor) {
        if (isHiddenModel(pActor)) {
            return;
        }

        hideModel(pActor);
    }

    void hideModelAndOnCalcAnimIfShown(LiveActor* pActor) {
        if (isHiddenModel(pActor)) {
            return;
        }

        hideModel(pActor);
        onCalcAnim(pActor);
    }

    void stopAnimFrame(LiveActor* pActor) {
        pActor->mFlag.mIsStoppedAnim = true;
    }

    void releaseAnimFrame(LiveActor* pActor) {
        pActor->mFlag.mIsStoppedAnim = false;
    }

    bool isNoCalcAnim(const LiveActor* pActor) {
        return pActor->mFlag.mIsNoCalcAnim;
    }

    void onCalcAnim(LiveActor* pActor) {
        pActor->mFlag.mIsNoCalcAnim = false;
    }

    void offCalcAnim(LiveActor* pActor) {
        pActor->mFlag.mIsNoCalcAnim = true;
    }

    bool isNoCalcView(const LiveActor* pActor) {
        return pActor->mFlag.mIsNoCalcView;
    }

    bool isNoEntryDrawBuffer(const LiveActor* pActor) {
        return pActor->mFlag.mIsHiddenModel;
    }

    bool isNoBind(const LiveActor* pActor) {
        return pActor->mFlag.mIsNoBind;
    }

    void onBind(LiveActor* pActor) {
        pActor->mFlag.mIsNoBind = false;
    }

    void offBind(LiveActor* pActor) {
        pActor->mFlag.mIsNoBind = true;
    }

    bool isCalcGravity(const LiveActor* pActor) {
        return pActor->mFlag.mIsCalcGravity;
    }

    void onCalcGravity(LiveActor* pActor) {
        if (!isDead(pActor)) {
            calcGravity(pActor);
        }

        pActor->mFlag.mIsCalcGravity = true;
    }

    void offCalcGravity(LiveActor* pActor) {
        pActor->mFlag.mIsCalcGravity = false;
    }

    void setCollisionMtx(LiveActor* pActor) {
        setCollisionMtx(pActor, getCollisionParts(pActor));
    }

    // setCollisionMtx

    CollisionParts* getCollisionParts(const LiveActor* pActor) {
        return pActor->mCollisionParts;
    }

    bool isExistCollisionParts(const LiveActor* pActor) {
        return pActor->mCollisionParts != nullptr;
    }
}; // namespace MR
