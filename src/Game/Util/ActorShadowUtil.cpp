#include "Game/LiveActor/ShadowSurfaceCircle.hpp"
#include "Game/LiveActor/ShadowSurfaceOval.hpp"
#include "Game/LiveActor/ShadowVolumeBox.hpp"
#include "Game/LiveActor/ShadowVolumeCylinder.hpp"
#include "Game/LiveActor/ShadowVolumeFlatModel.hpp"
#include "Game/LiveActor/ShadowVolumeLine.hpp"
#include "Game/LiveActor/ShadowVolumeOval.hpp"
#include "Game/LiveActor/ShadowVolumeSphere.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/ActorShadowLocalUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CollisionPartsFilter.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace MR {
    void initShadowFromCSV(LiveActor* pActor, const char* pName) {
        ResourceHolder* pResourceHolder = getResourceHolder(pActor);
        JMapInfo* pInfo = tryCreateCsvParser(pResourceHolder, "%s.bcsv", pName);

        if (pInfo == nullptr) {
            pActor->initShadowControllerList(1);
            return;
        }

        pActor->initShadowControllerList(pInfo->getNumEntries());

        for (JMapInfoIter iter = pInfo->begin(); iter.isValid(); iter.mIndex++) {
            ActorShadow::addShadowFromCSV(pActor, iter);
        }
    }

    void initShadowSurfaceCircle(LiveActor* pActor, f32 radius) {
        pActor->initShadowControllerList(1);
        addShadowSurfaceCircle(pActor, "水面丸影", radius);
    }

    void initShadowVolumeSphere(LiveActor* pActor, f32 radius) {
        pActor->initShadowControllerList(1);
        addShadowVolumeSphere(pActor, "ボリューム影(球)", radius);
    }

    void initShadowVolumeOval(LiveActor* pActor, const TVec3f& rSize) {
        pActor->initShadowControllerList(1);
        const char* pName = "ボリューム影(楕球)";
        addShadowVolumeOval(pActor, pName, rSize, pActor->getBaseMtx());
    }

    void initShadowVolumeCylinder(LiveActor* pActor, f32 radius) {
        pActor->initShadowControllerList(1);
        addShadowVolumeCylinder(pActor, "ボリューム影(円柱)", radius);
    }

    void initShadowVolumeBox(LiveActor* pActor, const TVec3f& rSize) {
        pActor->initShadowControllerList(1);
        addShadowVolumeBox(pActor, "ボリューム影(ボックス)", rSize);
    }

    void initShadowVolumeFlatModel(LiveActor* pActor, const char* pModelName) {
        pActor->initShadowControllerList(1);
        addShadowVolumeFlatModel(pActor, "ボリューム影(板モデル)", pModelName);
    }

    void initShadowController(LiveActor* pActor, u32 numShadows) {
        pActor->initShadowControllerList(numShadows);
    }

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

    void addShadowVolumeOval(LiveActor* pActor, const char* pName, const TVec3f& rSize, MtxPtr pMtx) {
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor, pName);
        ShadowVolumeOval* pOval = new ShadowVolumeOval();
        pController->setShadowDrawer(pOval);
        pController->setDropPosMtxPtr(pMtx, TVec3f(0.0f, 0.0f, 0.0f));
        pOval->setSize(rSize);
    }

    void addShadowVolumeCylinder(LiveActor* pActor, const char* pName, f32 radius) {
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor, pName);
        ShadowVolumeCylinder* pCylinder = new ShadowVolumeCylinder();
        pController->setShadowDrawer(pCylinder);
        pController->offCalcCollision();
        pCylinder->setRadius(radius);
    }

    void addShadowVolumeBox(LiveActor* pActor, const char* pName, const TVec3f& rSize) {
        addShadowVolumeBox(pActor, pName, rSize, pActor->getBaseMtx());
    }

    void addShadowVolumeBox(LiveActor* pActor, const char* pName, const TVec3f& rSize, MtxPtr pMtx) {
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor, pName);
        ShadowVolumeBox* pBox = new ShadowVolumeBox();
        pController->setShadowDrawer(pBox);
        pController->setDropPosMtxPtr(pMtx, TVec3f(0.0f, 0.0f, 0.0f));
        pBox->setSize(rSize);
    }

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

    void addShadowVolumeFlatModel(LiveActor* pActor, const char* pName, const char* pModelName) {
        addShadowVolumeFlatModel(pActor, pName, pModelName, pActor->getBaseMtx());
    }

    void addShadowVolumeFlatModel(LiveActor* pActor, const char* pName1, const char* pName2, MtxPtr pMtx) {
        ShadowController* pController = ActorShadow::createShadowControllerVolumeParam(pActor, pName1);
        ShadowVolumeFlatModel* pModel = new ShadowVolumeFlatModel(pName2);
        pController->setShadowDrawer(pModel);
        pController->offCalcCollision();
        pModel->setBaseMatrixPtr(pMtx);
    }

    void initShadowVolumeBox(LiveActor* pActor, const TVec3f& rSize, MtxPtr pMtx) {
        pActor->initShadowControllerList(1);
        addShadowVolumeBox(pActor, "ボリューム影(ボックス)", rSize, pMtx);
    }

    void initShadowVolumeFlatModel(LiveActor* pActor, const char* pModelName, MtxPtr pMtx) {
        pActor->initShadowControllerList(1);
        addShadowVolumeFlatModel(pActor, "ボリューム影(板モデル)", pModelName, pMtx);
    }

    void setShadowDropPositionPtr(LiveActor* pActor, const char* pName, const TVec3f* pPos) {
        ActorShadow::getShadowController(pActor, pName)->setDropPosPtr(pPos);
    }

    void setShadowDropPositionMtxPtr(LiveActor* pActor, const char* pName, MtxPtr pMtx, const TVec3f& rPos) {
        ActorShadow::getShadowController(pActor, pName)->setDropPosMtxPtr(pMtx, rPos);
    }

    void setShadowDropPositionAtJoint(LiveActor* pActor, const char* pName1, const char* pName2, const TVec3f& rPos) {
        setShadowDropPositionMtxPtr(pActor, pName1, getJointMtx(pActor, pName2), rPos);
    }

    void setShadowDropDirectionPtr(LiveActor* pActor, const char* pName, const TVec3f* pDir) {
        ActorShadow::getShadowController(pActor, pName)->setDropDirPtr(pDir);
    }

    void setShadowProjectionPtr(LiveActor* pActor, const char* pName, const TVec3f* pPosition, const TVec3f* pDirection) {
        ActorShadow::getShadowController(pActor, pName)->setProjectionPtr(pPosition, pDirection);
    }

    void setShadowDropPosition(LiveActor* pActor, const char* pName, const TVec3f& rPos) {
        ActorShadow::getShadowController(pActor, pName)->setDropPosFix(rPos);
    }

    void setShadowDropDirection(LiveActor* pActor, const char* pName, const TVec3f& rDir) {
        ActorShadow::getShadowController(pActor, pName)->setDropDirFix(rDir);
    }

    void setShadowDropLength(LiveActor* pActor, const char* pName, f32 length) {
        ActorShadow::getShadowController(pActor, pName)->setDropLength(length);
    }

    void setShadowDropStartOffset(LiveActor* pActor, const char* pName, f32 offset) {
        ActorShadow::getShadowController(pActor, pName)->setDropStartOffset(offset);
    }

    void setShadowProjection(LiveActor* pActor, const char* pName, const TVec3f& rPosition, const TVec3f& rDirection, bool a5) {
        ActorShadow::getShadowController(pActor, pName)->setProjectionFix(rPosition, rDirection, a5);
    }

    void setShadowSurfaceOvalColor(LiveActor* pActor, const char* pName, const Color8 color) {
        ShadowSurfaceOval* pOval = ActorShadow::getShadowSurfaceOval(pActor, pName);
        pOval->setColor(color);
    }

    void setShadowSurfaceOvalAlpha(LiveActor* pActor, const char* pName, u8 alpha) {
        ActorShadow::getShadowSurfaceOval(pActor, pName)->setAlpha(alpha);
    }

    void setShadowVolumeSphereRadius(LiveActor* pActor, const char* pName, f32 radius) {
        ActorShadow::getShadowVolumeSphere(pActor, pName)->setRadius(radius);
    }

    void setShadowVolumeCylinderRadius(LiveActor* pActor, const char* pName, f32 radius) {
        ActorShadow::getShadowVolumeCylinder(pActor, pName)->setRadius(radius);
    }

    void setShadowVolumeBoxSize(LiveActor* pActor, const char* pName, const TVec3f& rSize) {
        ActorShadow::getShadowVolumeBox(pActor, pName)->setSize(rSize);
    }

    void setShadowVolumeStartDropOffset(LiveActor* pActor, const char* pName, f32 offset) {
        ActorShadow::getShadowVolumeDrawer(pActor, pName)->setStartDrawShepeOffset(offset);
    }

    void setShadowVolumeEndDropOffset(LiveActor* pActor, const char* pName, f32 offset) {
        ActorShadow::getShadowVolumeDrawer(pActor, pName)->setEndDrawShepeOffset(offset);
    }

    void onShadowVolumeCutDropLength(LiveActor* pActor, const char* pName) {
        ActorShadow::getShadowVolumeDrawer(pActor, pName)->onCutDropShadow();
    }

    void onCalcShadow(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ActorShadow::getShadowController(pActor, pName)->onCalcCollision();
        } else {
            onCalcShadowAll(pActor);
        }
    }

    void offCalcShadow(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ActorShadow::getShadowController(pActor, pName)->offCalcCollision();
        } else {
            offCalcShadowAll(pActor);
        }
    }

    void onCalcShadowOneTime(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ActorShadow::getShadowController(pActor, pName)->onCalcCollisionOneTime();
        } else {
            onCalcShadowOneTimeAll(pActor);
        }
    }

    void onCalcShadowAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);

        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->onCalcCollision();
        }
    }

    void offCalcShadowAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);

        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->offCalcCollision();
        }
    }

    void onCalcShadowOneTimeAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);

        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->onCalcCollisionOneTime();
        }
    }

    void onCalcShadowDropGravity(LiveActor* pActor, const char* pName) {
        ActorShadow::getShadowController(pActor, pName)->onCalcDropGravity();
    }

    void onCalcShadowDropGravityOneTime(LiveActor* pActor, const char* pName) {
        ActorShadow::getShadowController(pActor, pName)->onCalcDropGravityOneTime();
    }

    void onCalcShadowDropPrivateGravity(LiveActor* pActor, const char* pName) {
        ActorShadow::getShadowController(pActor, pName)->onCalcDropPrivateGravity();
    }

    void onCalcShadowDropPrivateGravityOneTime(LiveActor* pActor, const char* pName) {
        ActorShadow::getShadowController(pActor, pName)->onCalcDropPrivateGravityOneTime();
    }

    void offCalcShadowDropPrivateGravity(LiveActor* pActor, const char* pName) {
        ActorShadow::getShadowController(pActor, pName)->offCalcDropPrivateGravity();
    }

    void excludeCalcShadowToMyCollision(LiveActor* pActor, const char* pName) {
        CollisionParts* pCollisionParts = pActor->mCollisionParts;

        if (pName != nullptr) {
            excludeCalcShadowToCollision(pActor, pName, pCollisionParts);
        } else {
            excludeCalcShadowToSensorAll(pActor, pCollisionParts->mHitSensor);
        }
    }

    inline void excludeCalcShadowToCollision(LiveActor* pActor, const char* pName, const HitSensor* pSensor) {
        if (pName == nullptr) {
            excludeCalcShadowToSensorAll(pActor, pSensor);
        } else {
            ShadowController* pShadowCtrl = ActorShadow::getShadowController(pActor, pName);
            pShadowCtrl->setCollisionPartsFilter(new CollisionPartsFilterSensor(pSensor));
        }
    }

    void excludeCalcShadowToCollision(LiveActor* pActor, const char* pName, CollisionParts* pCollision) {
        if (pName != nullptr) {
            const HitSensor* pSensor = pCollision->mHitSensor;
            excludeCalcShadowToCollision(pActor, pName, pSensor);
        } else {
            excludeCalcShadowToSensorAll(pActor, pCollision->mHitSensor);
        }
    }

    void excludeCalcShadowToSensorAll(LiveActor* pActor, const HitSensor* pExcluded) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);

        if (count == 0) {
            return;
        }

        CollisionPartsFilterSensor* pFilter = new CollisionPartsFilterSensor(pExcluded);

        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->setCollisionPartsFilter(pFilter);
        }
    }

    void excludeCalcShadowToActorAll(LiveActor* pActor, const LiveActor* pExcluded) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);

        if (count == 0) {
            return;
        }

        CollisionPartsFilterActor* pFilter = new CollisionPartsFilterActor(pExcluded);

        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->setCollisionPartsFilter(pFilter);
        }
    }

    bool isExistShadow(const LiveActor* pActor, const char* pName) {
        return ActorShadow::isExistShadowController(pActor, pName);
    }

    void invalidateShadow(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ActorShadow::getShadowController(pActor, pName)->invalidate();
        } else {
            invalidateShadowAll(pActor);
        }
    }

    void validateShadow(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ActorShadow::getShadowController(pActor, pName)->validate();
        } else {
            validateShadowAll(pActor);
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

    void validateShadowGroup(LiveActor* pActor, const char* pName) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);

        for (u32 i = 0; i < count; i++) {
            if (isEqualString(ActorShadow::getShadowController(pActor, i)->mGroupName, pName)) {
                ActorShadow::getShadowController(pActor, i)->validate();
            }
        }
    }

    void invalidateShadowAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);

        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->invalidate();
        }
    }

    void validateShadowAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);

        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->validate();
        }
    }

    void offShadowVisibleSyncHost(LiveActor* pActor, const char* pName) {
        if (pName != nullptr) {
            ActorShadow::getShadowController(pActor, pName)->offVisibleSyncHost();
        } else {
            offShadowVisibleSyncHostAll(pActor);
        }
    }

    void onShadowVisibleSyncHostAll(LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);

        for (u32 i = 0; i < count; i++) {
            ActorShadow::getShadowController(pActor, i)->onVisibleSyncHost();
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

    bool calcClippingRangeIncludeShadow(TVec3f* pVecOutput, f32* pF32Output, const LiveActor* pActor, f32 a4) {
        TVec3f projectionPos;

        if (ActorShadow::getShadowController(pActor, static_cast< const char* >(nullptr))->isProjected()) {
            getShadowProjectionPos(pActor, nullptr, &projectionPos);
            pVecOutput->set((pActor->mPosition + projectionPos) * 0.5f);
            *pF32Output = projectionPos.distance(pActor->mPosition) * 0.5f + a4;
            return true;
        } else {
            pVecOutput->set(pActor->mPosition);
            *pF32Output = a4;
            return false;
        }
    }

    void setClippingRangeIncludeShadow(LiveActor* pActor, TVec3f* pCenter, f32 a3) {
        f32 stack_8 = a3;

        if (calcClippingRangeIncludeShadow(pCenter, &stack_8, pActor, a3)) {
            setClippingTypeSphere(pActor, stack_8, pCenter);
        } else {
            setClippingTypeSphere(pActor, a3);
        }
    }

    bool isShadowProjected(const LiveActor* pActor, const char* pName) {
        return ActorShadow::getShadowController(pActor, pName)->isProjected();
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

    bool isShadowProjectedAny(const LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);

        for (u32 i = 0; i < count; i++) {
            if (ActorShadow::getShadowController(pActor, i)->isProjected()) {
                return true;
            }
        }

        return false;
    }

    f32 getShadowNearProjectionLength(const LiveActor* pActor) {
        u32 count = ActorShadow::getShadowControllerCount(pActor);
        f32 result = FLOAT_MAX;

        for (u32 i = 0; i < count; i++) {
            if (ActorShadow::getShadowController(pActor, i)->isProjected()) {
                f32 length = ActorShadow::getShadowController(pActor, i)->getProjectionLength();

                if (length < result) {
                    result = length;
                }
            }
        }

        return result;
    }
}  // namespace MR
