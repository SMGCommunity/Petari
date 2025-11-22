#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/LiveActor/ShadowSurfaceBox.hpp"
#include "Game/LiveActor/ShadowSurfaceCircle.hpp"
#include "Game/LiveActor/ShadowSurfaceOval.hpp"
#include "Game/LiveActor/ShadowVolumeBox.hpp"
#include "Game/LiveActor/ShadowVolumeCylinder.hpp"
#include "Game/LiveActor/ShadowVolumeFlatModel.hpp"
#include "Game/LiveActor/ShadowVolumeLine.hpp"
#include "Game/LiveActor/ShadowVolumeSphere.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/ActorShadowLocalUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"

namespace MR {
    void initShadowController(LiveActor* pActor, u32 numShadows) { pActor->initShadowControllerList(numShadows); }

    void addShadowSurfaceCircle(LiveActor* pActor, const char* pName, f32 radius) {
        ShadowController* pController = ActorShadow::createShadowControllerSuefaceParam(pActor, pName);
        ShadowSurfaceCircle* pCircle = new ShadowSurfaceCircle();
        pController->setShadowDrawer(pCircle);
        pCircle->setRadius(radius);
    }

    void addShadowVolumeSphere(LiveActor* pActor, const char* pName, f32 radius) {
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor, pName);
        ShadowVolumeSphere* pSphere = new ShadowVolumeSphere();
        pController->setShadowDrawer(pSphere);
        pSphere->setRadius(radius);
    }

    // Minor mismatch: stack_8 not initialized correctly
    /* void addShadowVolumeOval(LiveActor *pActor, const char *pName, const TVec3f &rSize, MtxPtr mtx) {
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor, pName);
        ShadowVolumeOval* pOval = new ShadowVolumeOval();
        pController->setShadowDrawer(pOval);
        TVec3f stack_8(0.0f, 0.0f, 0.0f);
        pController->setDropPosMtxPtr(mtx, stack_8);
        pOval->setSize(rSize);
    } */

    void addShadowVolumeCylinder(LiveActor* pActor, const char* pName, f32 radius) {
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor, pName);
        ShadowVolumeCylinder* pCylinder = new ShadowVolumeCylinder();
        pController->setShadowDrawer(pCylinder);
        pController->offCalcCollision();
        pCylinder->setRadius(radius);
    }

    void addShadowVolumeBox(LiveActor* pActor, const char* pName, const TVec3f& a3) { addShadowVolumeBox(pActor, pName, a3, pActor->getBaseMtx()); }

    // Minor mismatch: stack_8 not initialized correctly
    /* void addShadowVolumeBox(LiveActor *pActor, const char *pName, const TVec3f &a3, MtxPtr mtx) {
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor, pName);
        ShadowVolumeBox* pBox = new ShadowVolumeBox();
        pController->setShadowDrawer(pBox);
        TVec3f stack_8(0.0f, 0.0f, 0.0f);
        pController->setDropPosMtxPtr(mtx, stack_8);
        pBox->setSize(a3);
    } */

    void addShadowVolumeLine(LiveActor* pActor1, const char* pName1, LiveActor* pActor2, const char* pName2, f32 fromWidth, LiveActor* pActor3,
                             const char* pName3, f32 toWidth) {
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor1, pName1);
        ShadowVolumeLine* pLine = new ShadowVolumeLine();
        pController->setShadowDrawer(pLine);
        pController->offCalcCollision();
        pLine->setFromWidth(fromWidth);
        pLine->setToWidth(toWidth);
        pLine->setFromShadowController(ActorShadow::getShadowController(pActor2, pName2));
        pLine->setToShadowController(ActorShadow::getShadowController(pActor3, pName3));
    }

    // Wrong registers
    /* void addShadowVolumeFlatModel(LiveActor *pActor, const char *pName1, const char *pName2) {
        MtxPtr mtx = pActor->getBaseMtx();
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor, pName1);
        ShadowVolumeFlatModel* pModel = new ShadowVolumeFlatModel(pName2);
        pController->setShadowDrawer(pModel);
        pController->offCalcCollision();
        pModel->setBaseMatrixPtr(mtx);

    } */

    // Wrong value is loaded but only because the FlatModel header is incomplete
    void addShadowVolumeFlatModel(LiveActor* pActor, const char* pName1, const char* pName2, MtxPtr mtx) {
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor, pName1);
        ShadowVolumeFlatModel* pModel = new ShadowVolumeFlatModel(pName2);
        pController->setShadowDrawer(pModel);
        pController->offCalcCollision();
        pModel->setBaseMatrixPtr(mtx);
    }

    void setShadowDropPosition(LiveActor* pActor, const char* pName, const TVec3f& rPos) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->setDropPosFix(rPos);
    }

    void setShadowDropPositionPtr(LiveActor* pActor, const char* pName, const TVec3f* pPos) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->setDropPosPtr(pPos);
    }

    void setShadowDropPositionMtxPtr(LiveActor* pActor, const char* pName, MtxPtr mtx, const TVec3f* pPos) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->setDropPosMtxPtr(mtx, *pPos);
    }

    void setShadowDropPositionAtJoint(LiveActor* pActor, const char* pName1, const char* pName2, const TVec3f* pPos) {
        MtxPtr pJoint = getJointMtx(pActor, pName2);
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName1);
        pController->setDropPosMtxPtr(pJoint, *pPos);
    }

    void setShadowDropDirection(LiveActor* pActor, const char* pName, const TVec3f& rDir) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->setDropDirFix(rDir);
    }

    void setShadowDropDirectionPtr(LiveActor* pActor, const char* pName, const TVec3f* pDir) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->setDropDirPtr(pDir);
    }

    void setShadowProjection(LiveActor* pActor, const char* pName, const TVec3f& a3, const TVec3f& a4, bool a5) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->setProjectionFix(a3, a4, a5);
    }

    void setShadowProjectionPtr(LiveActor* pActor, const char* pName, const TVec3f* a3, const TVec3f* a4) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->setProjectionPtr(a3, a4);
    }

    void setShadowDropLength(LiveActor* pActor, const char* pName, f32 length) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->setDropLength(length);
    }

    void setShadowDropStartOffset(LiveActor* pActor, const char* pName, f32 offset) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->setDropStartOffset(offset);
    }

    void setShadowSurfaceOvalColor(LiveActor* pActor, const char* pName, Color8 color) {
        ShadowSurfaceOval* pOval = (ShadowSurfaceOval*)ActorShadow::getShadowSurfaceOval(pActor, pName);
        pOval->setColor(color);
    }

    void setShadowSurfaceOvalAlpha(LiveActor* pActor, const char* pName, u8 alpha) {
        ShadowSurfaceOval* pOval = (ShadowSurfaceOval*)ActorShadow::getShadowSurfaceOval(pActor, pName);
        pOval->setAlpha(alpha);
    }

    void setShadowVolumeSphereRadius(LiveActor* pActor, const char* pName, f32 radius) {
        ShadowVolumeSphere* pSphere = (ShadowVolumeSphere*)ActorShadow::getShadowVolumeSphere(pActor, pName);
        pSphere->setRadius(radius);
    }

    void setShadowVolumeCylinderRadius(LiveActor* pActor, const char* pName, f32 radius) {
        ShadowVolumeCylinder* pCylinder = (ShadowVolumeCylinder*)ActorShadow::getShadowVolumeCylinder(pActor, pName);
        pCylinder->setRadius(radius);
    }

    void setShadowVolumeBoxSize(LiveActor* pActor, const char* pName, const TVec3f& rSize) {
        ShadowVolumeBox* pBox = (ShadowVolumeBox*)ActorShadow::getShadowVolumeBox(pActor, pName);
        pBox->setSize(rSize);
    }

    void setShadowVolumeStartDropOffset(LiveActor* pActor, const char* pName, f32 offset) {
        ShadowVolumeDrawer* pDrawer = (ShadowVolumeDrawer*)ActorShadow::getShadowVolumeDrawer(pActor, pName);
        pDrawer->setStartDrawShepeOffset(offset);
    }

    void setShadowVolumeEndDropOffset(LiveActor* pActor, const char* pName, f32 offset) {
        ShadowVolumeDrawer* pDrawer = (ShadowVolumeDrawer*)ActorShadow::getShadowVolumeDrawer(pActor, pName);
        pDrawer->setEndDrawShepeOffset(offset);
    }

    void onShadowVolumeCutDropLength(LiveActor* pActor, const char* pName) {
        ShadowVolumeDrawer* pDrawer = (ShadowVolumeDrawer*)ActorShadow::getShadowVolumeDrawer(pActor, pName);
        pDrawer->onCutDropShadow();
    }

    void onCalcShadow(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
            pController->onCalcCollision();
        } else {
            onCalcShadowAll(pActor);
        }
    }

    void onCalcShadowAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->onCalcCollision();
        }
    }

    void onCalcShadowOneTime(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
            pController->onCalcCollisionOneTime();
        } else {
            onCalcShadowOneTimeAll(pActor);
        }
    }

    void onCalcShadowOneTimeAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->onCalcCollisionOneTime();
        }
    }

    void offCalcShadow(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
            pController->offCalcCollision();
        } else {
            offCalcShadowAll(pActor);
        }
    }

    void offCalcShadowAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->offCalcCollision();
        }
    }

    void onCalcShadowDropGravity(LiveActor* pActor, const char* pName) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->onCalcDropGravity();
    }

    void onCalcShadowDropGravityOneTime(LiveActor* pActor, const char* pName) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->onCalcDropGravityOneTime();
    }

    void onCalcShadowDropPrivateGravity(LiveActor* pActor, const char* pName) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->onCalcDropPrivateGravity();
    }

    void onCalcShadowDropPrivateGravityOneTime(LiveActor* pActor, const char* pName) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->onCalcDropPrivateGravityOneTime();
    }

    void offCalcShadowDropPrivateGravity(LiveActor* pActor, const char* pName) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        pController->offCalcDropPrivateGravity();
    }

    void excludeCalcShadowToMyCollision(LiveActor* pActor, const char* pName) {
        CollisionParts* pCollisionParts = pActor->mCollisionParts;
        if (pName != nullptr) {
            excludeCalcShadowToCollision(pActor, pName, pCollisionParts);
        } else {
            excludeCalcShadowToSensorAll(pActor, pCollisionParts->mHitSensor);
        }
    }

    bool isExistShadow(const LiveActor* pActor, const char* pName) { return ActorShadow::isExistShadowController(pActor, pName); }

    void validateShadow(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ActorShadow::getShadowController(pActor, pName)->validate();
        } else {
            validateShadowAll(pActor);
        }
    }

    void validateShadowGroup(LiveActor* pActor, const char* pName) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            if (isEqualString(ActorShadow::getShadowController(pActor, i)->mGroupName, pName)) {
                ActorShadow::getShadowController(pActor, i)->validate();
            }
        }
    }

    void validateShadowAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->validate();
        }
    }

    void invalidateShadow(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ActorShadow::getShadowController(pActor, pName)->invalidate();
        } else {
            invalidateShadowAll(pActor);
        }
    }

    void invalidateShadowGroup(LiveActor* pActor, const char* pName) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            if (isEqualString(ActorShadow::getShadowController(pActor, i)->mGroupName, pName)) {
                ActorShadow::getShadowController(pActor, i)->invalidate();
            }
        }
    }

    void invalidateShadowAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->invalidate();
        }
    }

    void onShadowVisibleSyncHostAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->onVisibleSyncHost();
        }
    }

    void offShadowVisibleSyncHost(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ActorShadow::getShadowController(pActor, pName)->offVisibleSyncHost();
        } else {
            offShadowVisibleSyncHostAll(pActor);
        }
    }

    void offShadowVisibleSyncHostAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->offVisibleSyncHost();
        }
    }

    void onShadowFollowHostScale(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ActorShadow::getShadowController(pActor, pName)->onFollowHostScale();
        } else {
            onShadowFollowHostScaleAll(pActor);
        }
    }

    void onShadowFollowHostScaleAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->onFollowHostScale();
        }
    }

    // Minor mismatch: Stack positions are wrong
    /* bool calcClippingRangeIncludeShadow(TVec3f *pVecOutput, f32 *pF32Output, const LiveActor *pActor, f32 a4) {
        TVec3f projectionPos;
        if (ActorShadow::getShadowController(pActor, (char*)nullptr)->isProjected()) {
            ActorShadow::getShadowController(pActor, (char*)nullptr)->getProjectionPos(&projectionPos);
            TVec3f stack_8(pActor->mPosition);
            TVec3f* pProjectionPos = &projectionPos;
            JMathInlineVEC::PSVECAdd2((Vec*)&stack_8, (Vec*)pProjectionPos, (Vec*)&stack_8);
            TVec3f stack_14(stack_8);
            stack_14.mult(0.5f);
            pVecOutput->set(stack_14);
            *pF32Output = 0.5f * PSVECDistance((Vec*)pProjectionPos, (Vec*)&pActor->mPosition) + a4;
            return true;
        }
        else {
            pVecOutput->set(pActor->mPosition);
            *pF32Output = a4;
            return false;
        }
    } */

    void setClippingRangeIncludeShadow(LiveActor* pActor, TVec3f* a2, f32 a3) {
        float stack_8(a3);
        if (calcClippingRangeIncludeShadow(a2, &stack_8, pActor, a3)) {
            setClippingTypeSphere(pActor, stack_8, a2);
        } else {
            setClippingTypeSphere(pActor, a3);
        }
    }

    bool isShadowProjected(const LiveActor* pActor, const char* pName) { return ActorShadow::getShadowController(pActor, pName)->isProjected(); }

    bool isShadowProjectedAny(const LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        for (u32 i = 0; i < count; i++) {
            if (ActorShadow::getShadowController(pActor, i)->isProjected()) {
                return true;
            }
        }
        return false;
    }

    void getShadowProjectionPos(const LiveActor* pActor, const char* pName, TVec3f* pResult) {
        ActorShadow::getShadowController(pActor, pName)->getProjectionPos(pResult);
    }

    void getShadowProjectionNormal(const LiveActor* pActor, const char* pName, TVec3f* pResult) {
        ActorShadow::getShadowController(pActor, pName)->getProjectionNormal(pResult);
    }

    f32 getShadowProjectionLength(const LiveActor* pActor, const char* pName) {
        ShadowController* pController = ActorShadow::getShadowController(pActor, pName);
        if (pController->isProjected()) {
            return pController->getProjectionLength();
        }
        return FLOAT_MAX;
    }

    HitSensor* getShadowProjectedSensor(const LiveActor* pActor, const char* pName) {
        return ActorShadow::getShadowController(pActor, pName)->mProjectedSensor;
    }

    f32 getShadowNearProjectionLength(const LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        float result = FLOAT_MAX;
        for (u32 i = 0; i < count; i++) {
            if (ActorShadow::getShadowController(pActor, i)->isProjected()) {
                float length = ActorShadow::getShadowController(pActor, i)->getProjectionLength();
                if (length < result) {
                    result = length;
                }
            }
        }
        return result;
    }
};  // namespace MR
