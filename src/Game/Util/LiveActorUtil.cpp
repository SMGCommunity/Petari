#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Animation/BckCtrl.hpp"
#include "Game/Animation/XanimeCore.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/LiveActor/ActorAnimKeeper.hpp"
#include "Game/LiveActor/DisplayListMaker.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/ClippingActorHolder.hpp"
#include "Game/LiveActor/ClippingDirector.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/MirrorActor.hpp"
#include "Game/LiveActor/MirrorCamera.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/NameObj/NameObjFinder.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Inline.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/JAudio2/JAUSoundAnimator.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <cstdio>
#include <cstring>
#include <new>

class GroupCheckManager {
public:
    void add(const NameObj*, long);
    bool isExist(const NameObj*, long) const;
};

class Flag {
public:
    Flag(const char*);
    void setInfoPos(const char*, const TVec3f*, const TVec3f&, f32, f32, f32, s32, s32, f32);
    void initWithoutIter();
};

class JAUSoundAnimation {
public:
    u32 getStartSoundIndex(f32) const;
    u32 getEndSoundIndex(f32) const;
};

class CollisionPartsFilterActor : public CollisionPartsFilterBase {
public:
    CollisionPartsFilterActor(const LiveActor* pActor) : mActor(pActor) {}

    virtual bool isInvalidParts(const CollisionParts* pParts) const { return pParts->mHitSensor->mHost == mActor; }

    const LiveActor* mActor;
};

namespace {
    const f32 sAnimRateScale = 1.0f;

    CollisionParts* createCollisionParts(ResourceHolder* pResHolder, const char* pName, HitSensor* pSensor, const TPos3f& rMtx,
                                         MR::CollisionScaleType scaleType, s32 unk) {
        CollisionParts* parts = new CollisionParts();
        char kcl[0x80];
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

    const char* createSubModelObjName(const LiveActor* pActor, const char* pSubName) {
        u32 len_sub = strlen(pSubName);
        u32 len_actor = strlen(pActor->mName);
        u32 len_bracket = strlen("（）");
        u32 len = len_actor + len_sub + len_bracket + 1;
        char* buf = new char[len];
        snprintf(buf, len, "%s（%s）", pActor->mName, pSubName);
        return buf;
    }

    PartsModel* createSubModel(LiveActor* pActor, const char* pSubModelName, MtxPtr pMtx, int drawBufferType) NO_INLINE {
        const char* modelResName = MR::getModelResourceHolder(pActor)->mModelResTable->getResName(0UL);

        if (!MR::isExistSubModel(modelResName, pSubModelName)) {
            return nullptr;
        }

        char subModelName[0x100];
        snprintf(subModelName, sizeof(subModelName), "%s%s", modelResName, pSubModelName);

        const char* objName = createSubModelObjName(pActor, pSubModelName);
        PartsModel* parts = new PartsModel(pActor, objName, subModelName, pMtx, drawBufferType, false);
        parts->initWithoutIter();
        MR::tryStartAllAnim(parts, subModelName);
        return parts;
    }

    volatile void* sCreateSubModelAddr = (void*)createSubModel;

    void changeBckForEffectKeeper(const LiveActor* pActor) NO_INLINE {
        EffectKeeper* keeper = pActor->mEffectKeeper;
        if (keeper != nullptr) {
            keeper->changeBck();
        }
    }

    void calcCollisionMtx(TPos3f* pMtx, const LiveActor* pActor) NO_INLINE {
        pMtx->set(pActor->getBaseMtx());

        f32 scale = pActor->mScale.x;
        pMtx->mMtx[0][0] *= scale;
        pMtx->mMtx[0][1] *= scale;
        pMtx->mMtx[0][2] *= scale;
        pMtx->mMtx[1][0] *= scale;
        pMtx->mMtx[1][1] *= scale;
        pMtx->mMtx[1][2] *= scale;
        pMtx->mMtx[2][0] *= scale;
        pMtx->mMtx[2][1] *= scale;
        pMtx->mMtx[2][2] *= scale;
    }

    bool isShowModel(LiveActor* pActor) NO_INLINE {
        return !pActor->mFlag.mIsHiddenModel;
    }

    void callFuncAllGroupMember(const LiveActor* pActor, void (*pFunc)(LiveActor*)) NO_INLINE {
        LiveActorGroupArray* pGroupArray = (LiveActorGroupArray*)MR::getSceneObjHolder()->getObj(SceneObj_LiveActorGroupArray);
        LiveActorGroup* pGroup = pGroupArray->getLiveActorGroup(pActor);
        if (pGroup == nullptr) {
            return;
        }

        for (s32 i = 0; i < pGroup->getObjectCount(); i++) {
            LiveActor* pMember = pGroup->getActor(i);
            if (pMember == pActor) {
                continue;
            }

            pFunc(pMember);
        }
    }

    void callMethodAllGroupMember(const LiveActor* pActor, void (LiveActor::*pMethod)()) NO_INLINE {
        LiveActorGroupArray* pGroupArray = (LiveActorGroupArray*)MR::getSceneObjHolder()->getObj(SceneObj_LiveActorGroupArray);
        LiveActorGroup* pGroup = pGroupArray->getLiveActorGroup(pActor);
        if (pGroup == nullptr) {
            return;
        }

        for (s32 i = 0; i < pGroup->getObjectCount(); i++) {
            LiveActor* pMember = pGroup->getActor(i);
            if (pMember == pActor) {
                continue;
            }

            (pMember->*pMethod)();
        }
    }

    template < typename T >
    s32 countGroupMember(const LiveActor* pActor, T pPred) NO_INLINE {
        LiveActorGroupArray* pGroupArray = (LiveActorGroupArray*)MR::getSceneObjHolder()->getObj(SceneObj_LiveActorGroupArray);
        LiveActorGroup* pGroup = pGroupArray->getLiveActorGroup(pActor);
        if (pGroup == nullptr) {
            return 0;
        }

        s32 count = 0;
        for (s32 i = 0; i < pGroup->getObjectCount(); i++) {
            LiveActor* pMember = pGroup->getActor(i);
            if (pMember == pActor) {
                continue;
            }

            if (pPred(pMember)) {
                count++;
            }
        }

        return count;
    }
};  // namespace

namespace MR {
    bool isBckPlaying(XanimePlayer*, const char*);
    bool isUseTex(J3DMaterial*, u16);
    void initMultiFur(LiveActor*, s32);
    void initDLMakerChangeTex(LiveActor*, const char*);
    void startBas(const LiveActor*, const char*, bool, f32, f32);
    bool startBckIfExist(const LiveActor*, const char*);
    bool startBtkIfExist(const LiveActor*, const char*);
    bool startBrkIfExist(const LiveActor*, const char*);
    bool startBpkIfExist(const LiveActor*, const char*);
    bool startBtpIfExist(const LiveActor*, const char*);
    bool startBvaIfExist(const LiveActor*, const char*);
    void setBpkFrameAndStop(const LiveActor*, f32);
    void calcGravity(LiveActor*, const TVec3f&);
    void calcGravityOrZero(LiveActor*, const TVec3f&);

    bool isExistIndirectTexture(const LiveActor* pActor) {
        const char* name = "IndDummy";
        return MR::getJ3DModelData(pActor)->mMaterialTable.mTextureName->getIndex(name) != -1;
    }

    void initMirrorReflection(LiveActor* pActor) {
        initDLMakerChangeTex(pActor, "MirrorTex");
        pActor->mModelManager->newDifferedDLBuffer();
        changeModelDataTexAll(pActor, "MirrorTex", *MR::getScreenResTIMG());
        pActor->mModelManager->mDisplayListMaker->addMirrorReflectionMtxSetter();
    }

    CollisionParts* tryCreateCollisionMoveLimit(LiveActor* pActor, HitSensor* pSensor) {
        char kcl[0x80];
        snprintf(kcl, sizeof(kcl), "%s.kcl", "MoveLimit");

        if (!MR::getResourceHolder(pActor)->mFileInfoTable->isExistRes(kcl)) {
            return nullptr;
        }

        TPos3f mtx;
        MR::makeMtxTRS(mtx, pActor);
        CollisionParts* parts = createCollisionParts(MR::getResourceHolder(pActor), "MoveLimit", pSensor, mtx, MR::UNKNOWN_2, 3);
        if (parts != nullptr) {
            MR::validateCollisionParts(parts);
        }

        return parts;
    }

    CollisionParts* tryCreateCollisionWaterSurface(LiveActor* pActor, HitSensor* pSensor) {
        char kcl[0x80];
        snprintf(kcl, sizeof(kcl), "%s.kcl", "WaterSurface");

        if (!MR::getResourceHolder(pActor)->mFileInfoTable->isExistRes(kcl)) {
            return nullptr;
        }

        TPos3f mtx;
        MR::makeMtxTRS(mtx, pActor);
        CollisionParts* parts = createCollisionParts(MR::getResourceHolder(pActor), "WaterSurface", pSensor, mtx, MR::UNKNOWN_2, 2);
        if (parts != nullptr) {
            MR::validateCollisionParts(parts);
        }

        return parts;
    }

    CollisionParts* tryCreateCollisionSunshade(LiveActor* pActor, HitSensor* pSensor) {
        char kcl[0x80];
        snprintf(kcl, sizeof(kcl), "%s.kcl", "Sunshade");

        if (!MR::getResourceHolder(pActor)->mFileInfoTable->isExistRes(kcl)) {
            return nullptr;
        }

        TPos3f mtx;
        MR::makeMtxTRS(mtx, pActor);
        CollisionParts* parts = createCollisionParts(MR::getResourceHolder(pActor), "Sunshade", pSensor, mtx, MR::UNKNOWN_2, 1);
        if (parts != nullptr) {
            MR::validateCollisionParts(parts);
        }

        return parts;
    }

    const char* createLowModelObjName(const LiveActor* pActor) {
        return createSubModelObjName(pActor, "Low");
    }

    const char* createMiddleModelObjName(const LiveActor* pActor) {
        return createSubModelObjName(pActor, "Middle");
    }

    PartsModel* createBloomModel(LiveActor* pActor, MtxPtr pMtx) {
        PartsModel* parts = createSubModel(pActor, "Bloom", pMtx, 30);
        if (parts != nullptr) {
            MR::registerDemoSimpleCastAll(parts);
        }

        return parts;
    }

    PartsModel* createWaterModel(LiveActor* pActor, MtxPtr pMtx) {
        return createSubModel(pActor, "Water", pMtx, 8);
    }

    PartsModel* createIndirectPlanetModel(LiveActor* pActor, MtxPtr pMtx) {
        return createSubModel(pActor, "Indirect", pMtx, 0x1D);
    }

    MirrorActor* tryCreateMirrorActor(LiveActor* pActor, const char* pModelName) {
        if (!MR::isInAreaObj("MirrorArea", pActor->mPosition)) {
            return nullptr;
        }

        const char* objName = createSubModelObjName(pActor, "鏡内モデル");
        MirrorActor* mirror = new MirrorActor(pActor, objName, pModelName);
        mirror->initWithoutIter();
        return mirror;
    }

    bool isOnGround(const LiveActor* pActor) {
        Binder* binder = pActor->mBinder;
        if (binder == nullptr) {
            return false;
        }

        if (!(0.0f <= binder->_C8)) {
            return false;
        }

        const TVec3f* normal = binder->mGroundInfo.mParentTriangle.getNormal(0);
        return pActor->mVelocity.dot(*normal) <= 0.0f;
    }

    bool isOnGroundCos(const LiveActor* pActor, f32 cos) {
        if (isOnGround(pActor)) {
            const TVec3f* normal = pActor->mBinder->mGroundInfo.mParentTriangle.getNormal(0);
            if (MR::isFloorPolygonCos(*normal, pActor->mGravity, cos)) {
                return true;
            }
        }

        return false;
    }

    bool isBindedGround(const LiveActor* pActor) {
        Binder* binder = pActor->mBinder;
        if (binder == nullptr) {
            return false;
        }

        return 0.0f <= binder->_C8;
    }

    bool isBindedGround(const LiveActor* pActor, HitSensor* pSensor) {
        if (isBindedGround(pActor)) {
            return MR::getGroundSensor(pActor) == pSensor;
        }

        return false;
    }

    bool isBindedWall(const LiveActor* pActor) {
        Binder* binder = pActor->mBinder;
        if (binder == nullptr) {
            return false;
        }

        return 0.0f <= binder->_158;
    }

    bool isBindedWall(const LiveActor* pActor, HitSensor* pSensor) {
        if (isBindedWall(pActor)) {
            return MR::getWallSensor(pActor) == pSensor;
        }

        return false;
    }

    bool isBindedWallOfMap(const LiveActor* pActor) {
        if (isBindedWall(pActor)) {
            const CollisionParts* parts = pActor->mBinder->mWallInfo.mParentTriangle.mParts;
            if (parts->mKeeperIndex == 0) {
                return true;
            }
        }

        return false;
    }

    bool isBindedWallOfMoveLimit(const LiveActor* pActor) {
        if (isBindedWall(pActor)) {
            const CollisionParts* parts = pActor->mBinder->mWallInfo.mParentTriangle.mParts;
            if (parts->mKeeperIndex == 3) {
                return true;
            }
        }

        return false;
    }

    bool isBindedWallOrSlopeGround(const LiveActor* pActor, f32 cos, TVec3f* pNormal) {
        if (isBindedWall(pActor)) {
            if (pNormal != nullptr) {
                const TVec3f* normal = pActor->mBinder->mWallInfo.mParentTriangle.getNormal(0);
                pNormal->set(*normal);
            }

            return true;
        }

        if (isOnGround(pActor)) {
            const TVec3f* normal = pActor->mBinder->mGroundInfo.mParentTriangle.getNormal(0);
            if (!MR::isFloorPolygonCos(*normal, pActor->mGravity, cos)) {
                if (pNormal != nullptr) {
                    pNormal->set(*normal);
                }

                return true;
            }
        }

        return false;
    }

    bool isBindedRoof(const LiveActor* pActor) {
        Binder* binder = pActor->mBinder;
        if (binder == nullptr) {
            return false;
        }

        return 0.0f <= binder->_1E8;
    }

    bool isBindedRoof(const LiveActor* pActor, HitSensor* pSensor) {
        if (isBindedRoof(pActor)) {
            return MR::getRoofSensor(pActor) == pSensor;
        }

        return false;
    }

    bool isBinded(const LiveActor* pActor) {
        if (isBindedGround(pActor) || isBindedRoof(pActor) || isBindedWall(pActor)) {
            return true;
        }

        return false;
    }

    bool isBinded(const LiveActor* pActor, HitSensor* pSensor) {
        if (isBindedGround(pActor, pSensor) || isBindedRoof(pActor, pSensor) || isBindedWall(pActor, pSensor)) {
            return true;
        }

        return false;
    }

    bool isPressedRoofAndGround(const LiveActor* pActor) {
        if (!isBindedRoof(pActor) || !isBindedGround(pActor)) {
            return false;
        }

        if (!MR::isSensorPressObj(MR::getGroundSensor(pActor)) && !MR::isSensorPressObj(MR::getRoofSensor(pActor))) {
            return false;
        }

        const Binder* binder = pActor->mBinder;
        TVec3f roofPower;
        TVec3f groundPower;
        binder->mRoofInfo.mParentTriangle.calcForceMovePower(&roofPower, binder->mRoofInfo.mHitPos);
        binder->mGroundInfo.mParentTriangle.calcForceMovePower(&groundPower, binder->mGroundInfo.mHitPos);

        TVec3f diff(roofPower);
        diff.sub(groundPower);
        return 0.0f < diff.dot(pActor->mGravity);
    }

    bool isPressedMovingWall(const LiveActor* pActor) {
        if (!isBindedWall(pActor)) {
            return false;
        }

        s32 planeNum = pActor->mBinder->mPlaneNum;
        s32 last = planeNum - 1;
        for (s32 i = 0; i < last; ++i) {
            HitInfo* plane1 = (HitInfo*)pActor->mBinder->getPlane(i);
            if (!MR::isWallPolygon(*plane1->mParentTriangle.getFaceNormal(), pActor->mGravity)) {
                continue;
            }

            TVec3f power1;
            plane1->mParentTriangle.calcForceMovePower(&power1, plane1->mHitPos);

            for (s32 j = i + 1; j < planeNum; ++j) {
                HitInfo* plane2 = (HitInfo*)pActor->mBinder->getPlane(j);
                if (!MR::isWallPolygon(*plane2->mParentTriangle.getFaceNormal(), pActor->mGravity)) {
                    continue;
                }

                const TVec3f* normal2 = plane2->mParentTriangle.getFaceNormal();
                const TVec3f* normal1 = plane1->mParentTriangle.getFaceNormal();
                if (0.0f <= normal1->dot(*normal2)) {
                    continue;
                }

                TVec3f power2;
                plane2->mParentTriangle.calcForceMovePower(&power2, plane2->mHitPos);

                TVec3f diffHit(plane1->mHitPos);
                diffHit.sub(plane2->mHitPos);

                TVec3f diffPower(power2);
                diffPower.sub(power1);

                if (0.0f < diffPower.dot(diffHit)) {
                    return true;
                }
            }
        }

        return false;
    }

    void copyTransRotateScale(const LiveActor* pSrc, LiveActor* pDst) {
        pDst->mPosition.set(pSrc->mPosition);
        pDst->mRotation.set(pSrc->mRotation);
        pDst->mScale.set(pSrc->mScale);
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

    bool changeShowModelFlagSyncNearClipping(LiveActor* pActor, f32 nearClip) {
        if (MR::isJudgedToNearClip(pActor->mPosition, nearClip)) {
            MR::hideModelAndOnCalcAnimIfShown(pActor);
            return false;
        }

        MR::showModelIfHidden(pActor);
        return true;
    }

    bool isClipped(const LiveActor* pActor) {
        return pActor->mFlag.mIsClipped;
    }

    bool isInvalidClipping(const LiveActor* pActor) {
        return pActor->mFlag.mIsInvalidClipping;
    }

    void setBaseTRMtx(LiveActor* pActor, MtxPtr pMtx) {
        PSMTXCopy(pMtx, (MtxPtr)&getJ3DModel(pActor)->mBaseTransformMtx);
    }

    void setBaseTRMtx(LiveActor* pActor, const TPos3f& rMtx) {
        PSMTXCopy((MtxPtr)rMtx.mMtx, (MtxPtr)&getJ3DModel(pActor)->mBaseTransformMtx);
    }

    void setBaseTRMtx(LiveActor* pActor, const TQuat4f& rQuat) {
        TPos3f mtx;
        const f32 twice = 2.0f;

        const f32 x2 = twice * rQuat.x;
        const f32 y2 = twice * rQuat.y;
        const f32 z2 = twice * rQuat.z;
        const f32 w2 = twice * rQuat.w;

        mtx.mMtx[0][0] = 1.0f - (y2 * rQuat.y) - (z2 * rQuat.z);
        mtx.mMtx[0][1] = (x2 * rQuat.y) - (w2 * rQuat.z);
        mtx.mMtx[0][2] = (x2 * rQuat.z) + (w2 * rQuat.y);
        mtx.mMtx[1][0] = (x2 * rQuat.y) + (w2 * rQuat.z);
        mtx.mMtx[1][1] = 1.0f - (x2 * rQuat.x) - (z2 * rQuat.z);
        mtx.mMtx[1][2] = (y2 * rQuat.z) - (w2 * rQuat.x);
        mtx.mMtx[2][0] = (x2 * rQuat.z) - (w2 * rQuat.y);
        mtx.mMtx[2][1] = (y2 * rQuat.z) + (w2 * rQuat.x);
        mtx.mMtx[2][2] = 1.0f - (x2 * rQuat.x) - (y2 * rQuat.y);
        mtx.mMtx[0][3] = pActor->mPosition.x;
        mtx.mMtx[1][3] = pActor->mPosition.y;
        mtx.mMtx[2][3] = pActor->mPosition.z;

        PSMTXCopy((MtxPtr)mtx.mMtx, (MtxPtr)&getJ3DModel(pActor)->mBaseTransformMtx);
    }

    void setBaseScale(LiveActor* pActor, const TVec3f& rScale) {
        J3DModel* pModel = getJ3DModel(pActor);
        pModel->mBaseScale.x = rScale.x;
        pModel->mBaseScale.y = rScale.y;
        pModel->mBaseScale.z = rScale.z;
    }

    ResourceHolder* getResourceHolder(const LiveActor* pActor) {
        if (pActor->mModelManager != nullptr) {
            return pActor->mModelManager->getResourceHolder();
        }

        return nullptr;
    }

    ResourceHolder* getModelResourceHolder(const LiveActor* pActor) {
        if (pActor->mModelManager != nullptr) {
            return pActor->mModelManager->getModelResourceHolder();
        }

        return nullptr;
    }

    ResTIMG* getTexFromModel(const char* pName, const LiveActor* pActor) {
        J3DModelData* pModelData = getJ3DModelData(pActor);
        s32 texIndex = pModelData->mMaterialTable.getTextureName()->getIndex(pName);
        return pModelData->mMaterialTable.getTexture()->getResTIMG(texIndex);
    }

    ResTIMG* getTexFromArc(const char* pName, const LiveActor* pActor) {
        return (ResTIMG*)getResourceHolder(pActor)->mFileInfoTable->getRes(pName);
    }

    const char* getModelResName(const LiveActor* pActor) {
        return getModelResourceHolder(pActor)->mModelResTable->getResName(0UL);
    }

    bool isExistAnim(const LiveActor* pActor, const char* pName) {
        if (isExistBck(pActor, pName)) {
            return true;
        }

        if (isExistBtk(pActor, pName)) {
            return true;
        }

        if (isExistBrk(pActor, pName)) {
            return true;
        }

        if (isExistBtp(pActor, pName)) {
            return true;
        }

        if (isExistBpk(pActor, pName)) {
            return true;
        }

        return isExistBva(pActor, pName);
    }

    bool isExistBck(const LiveActor* pActor, const char* pName) {
        ResTable* pTable = getResourceHolder(pActor)->mMotionResTable;
        if (pName == nullptr) {
            return pTable->mCount != 0;
        }

        return pTable->isExistRes(pName);
    }

    bool isExistBtk(const LiveActor* pActor, const char* pName) {
        ResTable* pTable = getResourceHolder(pActor)->mBtkResTable;
        if (pName == nullptr) {
            return pTable->mCount != 0;
        }

        return pTable->isExistRes(pName);
    }

    bool isExistBrk(const LiveActor* pActor, const char* pName) {
        ResTable* pTable = getResourceHolder(pActor)->mBrkResTable;
        if (pName == nullptr) {
            return pTable->mCount != 0;
        }

        return pTable->isExistRes(pName);
    }

    bool isExistBtp(const LiveActor* pActor, const char* pName) {
        ResTable* pTable = getResourceHolder(pActor)->mBtpResTable;
        if (pName == nullptr) {
            return pTable->mCount != 0;
        }

        return pTable->isExistRes(pName);
    }

    bool isExistBpk(const LiveActor* pActor, const char* pName) {
        ResTable* pTable = getResourceHolder(pActor)->mBpkResTable;
        if (pName == nullptr) {
            return pTable->mCount != 0;
        }

        return pTable->isExistRes(pName);
    }

    bool isExistBva(const LiveActor* pActor, const char* pName) {
        ResTable* pTable = getResourceHolder(pActor)->mBvaResTable;
        if (pName == nullptr) {
            return pTable->mCount != 0;
        }

        return pTable->isExistRes(pName);
    }

    bool isExistTexture(const LiveActor* pActor, const char* pName) {
        return getJ3DModelData(pActor)->mMaterialTable.getTextureName()->getIndex(pName) != -1;
    }

    void newDifferedDLBuffer(LiveActor* pActor) {
        pActor->mModelManager->newDifferedDLBuffer();
    }

    void initDLMakerFog(LiveActor* pActor, bool enable) {
        pActor->mModelManager->mDisplayListMaker->addFogCtrl(enable);
    }

    void initDLMakerMatColor0(LiveActor* pActor, const char* pMatName, const J3DGXColor* pColor) {
        pActor->mModelManager->mDisplayListMaker->addMatColorCtrl(pMatName, 0, pColor);
    }

    void initDLMakerChangeTex(LiveActor* pActor, const char* pTexName) {
        J3DModelData* pModelData = getJ3DModelData(pActor);
        DisplayListMaker* pDLMaker = pActor->mModelManager->mDisplayListMaker;
        J3DTexture* pTexture = pModelData->mMaterialTable.getTexture();

        for (u16 texIndex = 0; texIndex < pTexture->getNum(); ++texIndex) {
            if (strcmp(pModelData->mMaterialTable.getTextureName()->getName(texIndex), pTexName) != 0) {
                continue;
            }

            for (u16 matIndex = 0; matIndex < pModelData->mMaterialTable.getMaterialNum(); ++matIndex) {
                if (isUseTex(pModelData->mMaterialTable.getMaterialNodePointer(matIndex), texIndex)) {
                    pDLMaker->onPrgFlag(matIndex, 0x4020000);
                }
            }
        }
    }

    TexMtxCtrl* initDLMakerTexMtx(LiveActor* pActor, const char* pName) {
        return pActor->mModelManager->mDisplayListMaker->addTexMtxCtrl(pName);
    }

    ProjmapEffectMtxSetter* initDLMakerProjmapEffectMtxSetter(LiveActor* pActor) {
        return pActor->mModelManager->mDisplayListMaker->addProjmapEffectMtxSetter();
    }

    void startAction(const LiveActor* pActor, const char* pName) {
        if (pActor->mAnimKeeper != nullptr && pActor->mAnimKeeper->start(pName)) {
            return;
        }

        tryStartAllAnim(pActor, pName);
    }

    bool isActionEnd(const LiveActor* pActor) {
        return pActor->mModelManager->isBckStopped();
    }

    bool isActionStart(const LiveActor* pActor, const char* pName) {
        if (pActor->mAnimKeeper != nullptr) {
            return pActor->mAnimKeeper->isPlaying(pName);
        }

        return isBckPlaying(pActor->mModelManager->mXanimePlayer, pName);
    }

    bool tryStartAction(const LiveActor* pActor, const char* pName) {
        if (isActionStart(pActor, pName)) {
            return false;
        }

        startAction(pActor, pName);
        return true;
    }

    void startAllAnim(const LiveActor* pActor, const char* pName) {
        tryStartAllAnim(pActor, pName);
    }

    bool tryStartAllAnim(const LiveActor* pActor, const char* pName) {
        bool started = false;
        if (startBckIfExist(pActor, pName)) {
            started = true;
        }

        if (startBtkIfExist(pActor, pName)) {
            started = true;
        }

        if (startBrkIfExist(pActor, pName)) {
            started = true;
        }

        if (startBpkIfExist(pActor, pName)) {
            started = true;
        }

        if (startBtpIfExist(pActor, pName)) {
            started = true;
        }

        if (startBvaIfExist(pActor, pName)) {
            started = true;
        }

        return started;
    }

    void startBck(const LiveActor* pActor, const char* pBckName, const char* pBrkName) {
        pActor->mModelManager->startBck(pBckName, pBrkName);
        changeBckForEffectKeeper(pActor);
        startBas(pActor, pBckName, false, 0.0f, 0.0f);
    }

    void startBckWithInterpole(const LiveActor* pActor, const char* pBckName, s32 interpole) {
        pActor->mModelManager->startBckWithInterpole(pBckName, interpole);
        startBas(pActor, pBckName, false, 0.0f, 0.0f);
        changeBckForEffectKeeper(pActor);
    }

    void startBckNoInterpole(const LiveActor* pActor, const char* pBckName) {
        pActor->mModelManager->startBckWithInterpole(pBckName, 0);
        startBas(pActor, pBckName, false, 0.0f, 0.0f);
        changeBckForEffectKeeper(pActor);
    }

    void startBckAtFirstStep(const LiveActor* pActor, const char* pBckName) {
        if (!isStep(pActor, 0)) {
            return;
        }

        pActor->mModelManager->startBck(pBckName, nullptr);
        changeBckForEffectKeeper(pActor);
        startBas(pActor, pBckName, false, 0.0f, 0.0f);
    }

    bool tryStartBck(const LiveActor* pActor, const char* pBckName, const char* pBrkName) {
        if (isBckPlaying(pActor->mModelManager->mXanimePlayer, pBckName)) {
            return false;
        }

        pActor->mModelManager->startBck(pBckName, pBrkName);
        changeBckForEffectKeeper(pActor);
        startBas(pActor, pBckName, false, 0.0f, 0.0f);
        return true;
    }

    bool tryStartBckAndBtp(const LiveActor* pActor, const char* pBckName, const char* pBtpName) {
        if (!tryStartBck(pActor, pBckName, pBtpName)) {
            return false;
        }

        if (isExistBtp(pActor, pBtpName)) {
            pActor->mModelManager->startBtp(pBtpName);
        }

        return true;
    }

    void setAllAnimFrame(const LiveActor* pActor, const char* pName, f32 frame) {
        if (isExistBck(pActor, pName)) {
            pActor->mModelManager->getBckCtrl()->mFrame = frame;
        }

        if (isExistBtk(pActor, pName)) {
            pActor->mModelManager->getBtkCtrl()->mFrame = frame;
        }

        if (isExistBpk(pActor, pName)) {
            pActor->mModelManager->getBpkCtrl()->mFrame = frame;
        }

        if (isExistBtp(pActor, pName)) {
            pActor->mModelManager->getBtpCtrl()->mFrame = frame;
        }

        if (isExistBrk(pActor, pName)) {
            pActor->mModelManager->getBrkCtrl()->mFrame = frame;
        }

        if (isExistBva(pActor, pName)) {
            pActor->mModelManager->getBvaCtrl()->mFrame = frame;
        }
    }

    void setAllAnimFrameAndStop(const LiveActor* pActor, const char* pName, f32 frame) {
        if (isExistBck(pActor, pName)) {
            setBckFrameAndStop(pActor, frame);
        }

        if (isExistBtk(pActor, pName)) {
            setBtkFrameAndStop(pActor, frame);
        }

        if (isExistBpk(pActor, pName)) {
            setBpkFrameAndStop(pActor, frame);
        }

        if (isExistBtp(pActor, pName)) {
            setBtpFrameAndStop(pActor, frame);
        }

        if (isExistBrk(pActor, pName)) {
            setBrkFrameAndStop(pActor, frame);
        }

        if (isExistBva(pActor, pName)) {
            setBvaFrameAndStop(pActor, frame);
        }
    }

    void setAllAnimFrameAtEnd(const LiveActor* pActor, const char* pName) {
        if (isExistBck(pActor, pName)) {
            setBckFrame(pActor, (f32)pActor->mModelManager->getBckCtrl()->mEnd);
        }

        if (isExistBtk(pActor, pName)) {
            setBtkFrame(pActor, (f32)pActor->mModelManager->getBtkCtrl()->mEnd);
        }

        if (isExistBpk(pActor, pName)) {
            setBpkFrame(pActor, (f32)pActor->mModelManager->getBpkCtrl()->mEnd);
        }

        if (isExistBtp(pActor, pName)) {
            setBtpFrame(pActor, (f32)pActor->mModelManager->getBtpCtrl()->mEnd);
        }

        if (isExistBrk(pActor, pName)) {
            setBrkFrame(pActor, (f32)pActor->mModelManager->getBrkCtrl()->mEnd);
        }

        if (isExistBva(pActor, pName)) {
            setBrkFrame(pActor, (f32)pActor->mModelManager->getBvaCtrl()->mEnd);
        }
    }

    bool isAnyAnimStopped(const LiveActor* pActor, const char* pName) {
        if (isExistBck(pActor, pName) && pActor->mModelManager->isBckStopped()) {
            return true;
        }

        if (isExistBtk(pActor, pName) && pActor->mModelManager->isBtkStopped()) {
            return true;
        }

        if (isExistBpk(pActor, pName) && pActor->mModelManager->isBpkStopped()) {
            return true;
        }

        if (isExistBtp(pActor, pName) && pActor->mModelManager->isBtpStopped()) {
            return true;
        }

        if (isExistBrk(pActor, pName) && pActor->mModelManager->isBrkStopped()) {
            return true;
        }

        if (isExistBva(pActor, pName) && pActor->mModelManager->isBvaStopped()) {
            return true;
        }

        return false;
    }

    bool isAnyAnimOneTimeAndStopped(const LiveActor* pActor, const char* pName) {
        if (isExistBck(pActor, pName) && pActor->mModelManager->isBckStopped()) {
            return true;
        }

        if (isExistBtk(pActor, pName) && pActor->mModelManager->isBtkStopped()) {
            return true;
        }

        if (isExistBpk(pActor, pName) && pActor->mModelManager->isBpkStopped()) {
            return true;
        }

        if (isExistBtp(pActor, pName) && pActor->mModelManager->isBtpStopped()) {
            return true;
        }

        if (isExistBrk(pActor, pName) && pActor->mModelManager->isBrkStopped()) {
            return true;
        }

        if (isExistBva(pActor, pName) && pActor->mModelManager->isBvaStopped()) {
            return true;
        }

        return false;
    }

    const char* getPlayingBckName(const LiveActor* pActor) {
        return pActor->mModelManager->getPlayingBckName();
    }

    void setBinderIgnoreMovingCollision(LiveActor* pActor) {
        pActor->mBinder->_1EC._0 = pActor->mBinder->_1EC._0;
        *(u8*)&pActor->mBinder->_1EC &= 0x7F;
    }

    void joinToGroup(LiveActor* pActor, const char* pGroupName) {
        LiveActorGroup* pGroup = (LiveActorGroup*)NameObjFinder::find(pGroupName);
        pGroup->registerActor(pActor);
    }

    MsgSharedGroup* joinToGroupArray(LiveActor* pActor, const JMapInfoIter& rIter, const char* pName, s32 maxCount) {
        if (!rIter.isValid()) {
            return nullptr;
        }

        s32 groupId = 0;
        if (!getJMapInfoGroupID(rIter, &groupId)) {
            return nullptr;
        }

        LiveActorGroupArray* pGroupArray = (LiveActorGroupArray*)getSceneObjHolder()->getObj(SceneObj_LiveActorGroupArray);
        return (MsgSharedGroup*)pGroupArray->entry(pActor, rIter, pName, maxCount);
    }

    LiveActorGroup* getGroupFromArray(const LiveActor* pActor) {
        LiveActorGroupArray* pGroupArray = (LiveActorGroupArray*)getSceneObjHolder()->getObj(SceneObj_LiveActorGroupArray);
        return pGroupArray->getLiveActorGroup(pActor);
    }

    LiveActor* getPairedGroupMember(const LiveActor* pActor) {
        LiveActorGroup* pGroup = getGroupFromArray(pActor);
        if (pGroup == nullptr) {
            return nullptr;
        }

        for (s32 i = 0; i < pGroup->getObjectCount(); i++) {
            LiveActor* pMember = pGroup->getActor(i);
            if (pMember != pActor) {
                return pMember;
            }
        }

        return nullptr;
    }

    void callMakeActorDeadAllGroupMember(const LiveActor* pActor) {
        callMethodAllGroupMember(pActor, &LiveActor::makeActorDead);
    }

    void callKillAllGroupMember(const LiveActor* pActor) {
        callMethodAllGroupMember(pActor, &LiveActor::kill);
    }

    void callMakeActorAppearedAllGroupMember(const LiveActor* pActor) {
        callMethodAllGroupMember(pActor, &LiveActor::makeActorAppeared);
    }

    void callAppearAllGroupMember(const LiveActor* pActor) {
        callMethodAllGroupMember(pActor, &LiveActor::appear);
    }

    void callRequestMovementOnAllGroupMember(const LiveActor* pActor) {
        callFuncAllGroupMember(pActor, requestMovementOn);
    }

    void callInvalidateClippingAllGroupMember(const LiveActor* pActor) {
        callFuncAllGroupMember(pActor, invalidateClipping);
    }

    void callValidateClippingAllGroupMember(const LiveActor* pActor) {
        callFuncAllGroupMember(pActor, validateClipping);
    }

    s32 countHideGroupMember(const LiveActor* pActor) {
        return countGroupMember(pActor, isHiddenModel);
    }

    s32 countShowGroupMember(const LiveActor* pActor) {
        return countGroupMember(pActor, isShowModel);
    }

    void addToAttributeGroupSearchTurtle(const LiveActor* pActor) {
        GroupCheckManager* pGroup = (GroupCheckManager*)getSceneObjHolder()->getObj(0x66);
        pGroup->add(pActor, 0);
    }

    void addToAttributeGroupReflectSpinningBox(const LiveActor* pActor) {
        GroupCheckManager* pGroup = (GroupCheckManager*)getSceneObjHolder()->getObj(0x66);
        pGroup->add(pActor, 1);
    }

    bool isExistInAttributeGroupSearchTurtle(const LiveActor* pActor) {
        GroupCheckManager* pGroup = (GroupCheckManager*)getSceneObjHolder()->getObj(0x66);
        return pGroup->isExist(pActor, 0);
    }

    bool isExistInAttributeGroupReflectSpinningBox(const LiveActor* pActor) {
        GroupCheckManager* pGroup = (GroupCheckManager*)getSceneObjHolder()->getObj(0x66);
        return pGroup->isExist(pActor, 1);
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

    bool isBckLooped(const LiveActor* pActor) {
        return (pActor->mModelManager->getBckCtrl()->mState & 1) != 0;
    }

    bool checkPassBckFrame(const LiveActor* pActor, f32 f) {
        return pActor->mModelManager->getBckCtrl()->checkPass(f) == 1;
    }

    void setBckFrameAtRandom(const LiveActor* pActor) {
        s16 actorEndFrame = pActor->mModelManager->getBckCtrl()->mEnd;
        s32 frameRand = (actorEndFrame * MR::getRandom());
        setBckFrame(pActor, frameRand);
    }

    void setBtkFrameAtRandom(const LiveActor* pActor) {
        s16 actorEndFrame = pActor->mModelManager->getBtkCtrl()->mEnd;
        s32 frameRand = (actorEndFrame * MR::getRandom());
        pActor->mModelManager->getBtkCtrl()->mFrame = frameRand;
    }

    void setBckFrameAndStop(const LiveActor* pActor, f32 frame) {
        J3DFrameCtrl* pBckCtrl;

        pBckCtrl = pActor->mModelManager->getBckCtrl();
        pBckCtrl->mFrame = frame;

        pBckCtrl = pActor->mModelManager->getBckCtrl();
        pBckCtrl->mRate = 0.0f;
    }

    void setBtkFrameAndStop(const LiveActor* pActor, f32 frame) {
        J3DFrameCtrl* pBtkCtrl;

        pBtkCtrl = pActor->mModelManager->getBtkCtrl();
        pBtkCtrl->mFrame = frame;

        pBtkCtrl = pActor->mModelManager->getBtkCtrl();
        pBtkCtrl->mRate = 0.0f;
    }

    void setBrkFrameAndStop(const LiveActor* pActor, f32 frame) {
        J3DFrameCtrl* pBrkCtrl;

        pBrkCtrl = pActor->mModelManager->getBrkCtrl();
        pBrkCtrl->mFrame = frame;

        pBrkCtrl = pActor->mModelManager->getBrkCtrl();
        pBrkCtrl->mRate = 0.0f;
    }

    void setBtpFrameAndStop(const LiveActor* pActor, f32 frame) {
        J3DFrameCtrl* pBtpCtrl;

        pBtpCtrl = pActor->mModelManager->getBtpCtrl();
        pBtpCtrl->mFrame = frame;

        pBtpCtrl = pActor->mModelManager->getBtpCtrl();
        pBtpCtrl->mRate = 0.0f;
    }

    void setBpkFrameAndStop(const LiveActor* pActor, f32 frame) {
        J3DFrameCtrl* pBpkCtrl;

        pBpkCtrl = pActor->mModelManager->getBpkCtrl();
        pBpkCtrl->mFrame = frame;

        pBpkCtrl = pActor->mModelManager->getBpkCtrl();
        pBpkCtrl->mRate = 0.0f;
    }

    void setBvaFrameAndStop(const LiveActor* pActor, f32 frame) {
        J3DFrameCtrl* pBvaCtrl;

        pBvaCtrl = pActor->mModelManager->getBvaCtrl();
        pBvaCtrl->mFrame = frame;

        pBvaCtrl = pActor->mModelManager->getBvaCtrl();
        pBvaCtrl->mRate = 0.0f;
    }

    void setBrkFrameEndAndStop(const LiveActor* pActor) {
        J3DFrameCtrl* pBrkCtrl;

        pBrkCtrl = pActor->mModelManager->getBrkCtrl();
        pBrkCtrl->mFrame = pBrkCtrl->mEnd;

        pBrkCtrl = pActor->mModelManager->getBrkCtrl();
        pBrkCtrl->mRate = 0.0f;
    }

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

    bool startBckIfExist(const LiveActor* pActor, const char* pBckName) {
        if (!getResourceHolder(pActor)->mMotionResTable->isExistRes(pBckName)) {
            return false;
        }

        pActor->mModelManager->startBck(pBckName, nullptr);
        changeBckForEffectKeeper(pActor);
        startBas(pActor, pBckName, false, 0.0f, 0.0f);
        return true;
    }

    bool startBtkIfExist(const LiveActor* pActor, const char* pBtkName) {
        if (!getResourceHolder(pActor)->mBtkResTable->isExistRes(pBtkName)) {
            return false;
        }

        pActor->mModelManager->startBtk(pBtkName);
        return true;
    }

    bool startBrkIfExist(const LiveActor* pActor, const char* pBrkName) {
        if (!getResourceHolder(pActor)->mBrkResTable->isExistRes(pBrkName)) {
            return false;
        }

        pActor->mModelManager->startBrk(pBrkName);
        return true;
    }

    bool startBtpIfExist(const LiveActor* pActor, const char* pBtpName) {
        if (!getResourceHolder(pActor)->mBtpResTable->isExistRes(pBtpName)) {
            return false;
        }

        pActor->mModelManager->startBtp(pBtpName);
        return true;
    }

    bool startBpkIfExist(const LiveActor* pActor, const char* pBpkName) {
        if (!getResourceHolder(pActor)->mBpkResTable->isExistRes(pBpkName)) {
            return false;
        }

        pActor->mModelManager->startBpk(pBpkName);
        return true;
    }

    bool startBvaIfExist(const LiveActor* pActor, const char* pBvaName) {
        if (!getResourceHolder(pActor)->mBvaResTable->isExistRes(pBvaName)) {
            return false;
        }

        pActor->mModelManager->startBva(pBvaName);
        return true;
    }

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

    void stopBck(const LiveActor* pActor) {
        pActor->mModelManager->getBckCtrl()->mRate = 0.0f;
    }

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

    void setBckRate(const LiveActor* pActor, f32 rate) {
        pActor->mModelManager->getBckCtrl()->mRate = rate * sAnimRateScale;
    }

    void setBtkRate(const LiveActor* pActor, f32 rate) {
        pActor->mModelManager->getBtkCtrl()->mRate = rate * sAnimRateScale;
    }

    void setBrkRate(const LiveActor* pActor, f32 rate) {
        pActor->mModelManager->getBrkCtrl()->mRate = rate * sAnimRateScale;
    }

    void setBvaRate(const LiveActor* pActor, f32 rate) {
        pActor->mModelManager->getBvaCtrl()->mRate = rate * sAnimRateScale;
    }

    void setBckFrame(const LiveActor* pActor, f32 frame) {
        pActor->mModelManager->getBckCtrl()->mFrame = frame;
    }

    void setBtkFrame(const LiveActor* pActor, f32 frame) {
        pActor->mModelManager->getBtkCtrl()->mFrame = frame;
    }

    void setBrkFrame(const LiveActor* pActor, f32 frame) {
        pActor->mModelManager->getBrkCtrl()->mFrame = frame;
    }

    void setBtpFrame(const LiveActor* pActor, f32 frame) {
        pActor->mModelManager->getBtpCtrl()->mFrame = frame;
    }

    void setBpkFrame(const LiveActor* pActor, f32 frame) {
        pActor->mModelManager->getBpkCtrl()->mFrame = frame;
    }

    void setBvaFrame(const LiveActor* pActor, f32 frame) {
        pActor->mModelManager->getBvaCtrl()->mFrame = frame;
    }

    bool isBckPlaying(const LiveActor* pActor, const char* pBckName) {
        return MR::isBckPlaying(pActor->mModelManager->mXanimePlayer, pBckName);
    }

    J3DFrameCtrl* getBckCtrl(const LiveActor* pActor) {
        return pActor->mModelManager->getBckCtrl();
    }

    J3DFrameCtrl* getBtkCtrl(const LiveActor* pActor) {
        return pActor->mModelManager->getBtkCtrl();
    }

    J3DFrameCtrl* getBrkCtrl(const LiveActor* pActor) {
        return pActor->mModelManager->getBrkCtrl();
    }

    J3DFrameCtrl* getBtpCtrl(const LiveActor* pActor) {
        return pActor->mModelManager->getBtpCtrl();
    }

    J3DFrameCtrl* getBpkCtrl(const LiveActor* pActor) {
        return pActor->mModelManager->getBpkCtrl();
    }

    J3DFrameCtrl* getBvaCtrl(const LiveActor* pActor) {
        return pActor->mModelManager->getBvaCtrl();
    }

    void updateMaterial(LiveActor* pActor) {
        pActor->mModelManager->updateDL(true);
    }

    void setMirrorReflectionInfoFromMtxYUp(const TPos3f& rMtx) {
        TVec3f up;
        TVec3f pos;

        up.set(rMtx.mMtx[0][1], rMtx.mMtx[1][1], rMtx.mMtx[2][1]);
        pos.set(rMtx.mMtx[0][3], rMtx.mMtx[1][3], rMtx.mMtx[2][3]);

        MR::getMirrorCamera()->setMirrorMapInfo(up, pos);
    }

    void setMirrorReflectionInfoFromModel(LiveActor* pActor) {
        J3DModelData* pModelData = MR::getJ3DModelData(pActor);
        MR::getMirrorCamera()->setMirrorMapInfo(pModelData);
    }

    void changeModelDataTexAll(LiveActor* pActor, const char* pTexName, const ResTIMG& rTimg) {
        J3DModelData* pModelData = MR::getJ3DModelData(pActor);
        DisplayListMaker* pDLMaker = pActor->mModelManager->mDisplayListMaker;
        J3DTexture* pTexture = pModelData->mMaterialTable.getTexture();

        for (u16 texIndex = 0; texIndex < pTexture->getNum(); texIndex++) {
            const char* name = pModelData->mMaterialTable.getTextureName()->getName(texIndex);
            if (strcmp(name, pTexName) != 0) {
                continue;
            }

            pTexture->setResTIMG(texIndex, rTimg);

            for (u16 matIndex = 0; matIndex < pModelData->mMaterialTable.getMaterialNum(); matIndex++) {
                J3DMaterial* material = pModelData->mMaterialTable.getMaterialNodePointer(matIndex);
                if (MR::isUseTex(material, texIndex)) {
                    pDLMaker->onCurFlag(matIndex, 0x4020000);
                }
            }
        }
    }

    void initJointTransform(const LiveActor* pActor) {
        pActor->mModelManager->initJointTransform();
    }

    XjointTransform* getJointTransform(const LiveActor* pActor, const char* pName) {
        return pActor->mModelManager->getJointTransform(pName);
    }

    void setJointTransformLocalMtx(const LiveActor* pActor, const char* pName, MtxPtr pMtx) {
        XjointTransform* pTransform = pActor->mModelManager->getJointTransform(pName);
        pTransform->_68 = (u32)pMtx;
    }

    f32 getBckFrame(const LiveActor* pActor) {
        return pActor->mModelManager->getBckCtrl()->mFrame;
    }

    f32 getBrkFrame(const LiveActor* pActor) {
        return pActor->mModelManager->getBrkCtrl()->mFrame;
    }

    f32 getBtpFrame(const LiveActor* pActor) {
        return pActor->mModelManager->getBtpCtrl()->mFrame;
    }

    f32 getBvaFrame(const LiveActor* pActor) {
        return pActor->mModelManager->getBvaCtrl()->mFrame;
    }

    f32 getBckRate(const LiveActor* pActor) {
        return pActor->mModelManager->getBckCtrl()->mRate;
    }

    f32 getBckFrameMax(const LiveActor* pActor) {
        return (f32)pActor->mModelManager->getBckCtrl()->mEnd;
    }

    f32 getBtkFrameMax(const LiveActor* pActor) {
        return (f32)pActor->mModelManager->getBtkCtrl()->mEnd;
    }

    f32 getBrkFrameMax(const LiveActor* pActor) {
        return (f32)pActor->mModelManager->getBrkCtrl()->mEnd;
    }

    void reflectBckCtrlData(LiveActor* pActor, const BckCtrlData& rBck) {
        BckCtrlFunction::reflectBckCtrlData(rBck, pActor->mModelManager->mXanimePlayer);

        AudAnmSoundObject* pSoundObj = pActor->mSoundObject;
        if (pSoundObj != nullptr && pSoundObj->hasAnimHandles()) {
            f32 endFrame = (f32)rBck.mEndFrame;
            if (rBck.mRepeatFrame < 0) {
                pSoundObj->setLoopFrame((f32)rBck.mStartFrame, endFrame);
            } else {
                pSoundObj->setLoopFrame((f32)rBck.mRepeatFrame, endFrame);
            }

            pSoundObj->setStartPos((f32)rBck.mStartFrame);
        }
    }

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
        return stepMax <= 0 ? 1.0f : clamp(static_cast< f32 >(pActor->getNerveStep()) / stepMax, 0.0f, 1.0f);
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
        f32 dot = grav->dot(*normal);
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

    void setBinderCollisionPartsFilter(LiveActor* pActor, CollisionPartsFilterBase* pFilter) {
        pActor->mBinder->setCollisionPartsFilter(pFilter);
    }

    void setBinderExceptActor(LiveActor* pActor, const LiveActor* pExcept) {
        CollisionPartsFilterActor* pFilter = new CollisionPartsFilterActor(pExcept);
        pActor->mBinder->setCollisionPartsFilter(pFilter);
    }

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

    void calcGravity(LiveActor* pActor) {
        TVec3f gravity;
        calcGravityVector(pActor, pActor->mPosition, &gravity, nullptr, 0);

        if (!isNearZero(gravity, 0.001f)) {
            pActor->mGravity = gravity;
        }
    }

    void calcGravity(LiveActor* pActor, const TVec3f& rPos) {
        TVec3f gravity;
        calcGravityVector(pActor, rPos, &gravity, nullptr, 0);

        if (!isNearZero(gravity, 0.001f)) {
            pActor->mGravity = gravity;
        }
    }

    void calcGravityOrZero(LiveActor* pActor) {
        calcGravityOrZero(pActor, pActor->mPosition);
    }

    void calcGravityOrZero(LiveActor* pActor, const TVec3f& rPos) {
        TVec3f gravity;
        calcGravityVectorOrZero(pActor, rPos, &gravity, nullptr, 0);
        if (!isNearZero(gravity, 0.001f)) {
            pActor->mGravity = gravity;
            return;
        }

        if (isBindedGround(pActor)) {
            pActor->mGravity = -(*pActor->mBinder->mGroundInfo.mParentTriangle.getNormal(0));
        }
    }

    void initFur(LiveActor* pActor) {
        initMultiFur(pActor, -1);
    }

    void initFurPlanet(LiveActor* pActor) {
        initMultiFur(pActor, 3);
    }

    void initFurPlayer(LiveActor* pActor) {
        initMultiFur(pActor, 0);
    }

    void initCollisionParts(LiveActor* pActor, const char* pName, HitSensor* pSensor, MtxPtr pMtx) {
        pActor->initActorCollisionParts(pName, pSensor, nullptr, pMtx, false, false);
    }

    void initCollisionPartsAutoEqualScale(LiveActor* pActor, const char* pName, HitSensor* pSensor, MtxPtr pMtx) {
        pActor->initActorCollisionParts(pName, pSensor, nullptr, pMtx, true, false);
    }

    void initCollisionPartsAutoEqualScaleOne(LiveActor* pActor, const char* pName, HitSensor* pSensor, MtxPtr pMtx) {
        pActor->initActorCollisionParts(pName, pSensor, nullptr, pMtx, true, true);
    }

    void initCollisionPartsFromResourceHolder(LiveActor* pActor, const char* pName, HitSensor* pSensor, ResourceHolder* pResHolder, MtxPtr pMtx) {
        pActor->initActorCollisionParts(pName, pSensor, pResHolder, pMtx, false, false);
    }

    CollisionParts* createCollisionPartsFromLiveActor(LiveActor* pActor, const char* pName, HitSensor* pSensor, CollisionScaleType scaleType) {
        TPos3f mtx;
        makeMtxTRS(mtx, pActor);
        return createCollisionParts(getResourceHolder(pActor), pName, pSensor, mtx, scaleType, 0);
    }

    CollisionParts* createCollisionPartsFromLiveActor(LiveActor* pActor, const char* pName, HitSensor* pSensor, MtxPtr pMtx, CollisionScaleType scaleType) {
        TPos3f mtx;
        mtx.set(pMtx);
        CollisionParts* pParts = createCollisionParts(getResourceHolder(pActor), pName, pSensor, mtx, scaleType, 0);
        pParts->_0 = (TMtx34f*)pMtx;
        return pParts;
    }

    CollisionParts* createCollisionPartsFromResourceHolder(
        ResourceHolder* pResHolder, const char* pName, HitSensor* pSensor, const TPos3f& rMtx, CollisionScaleType scaleType) {
        return createCollisionParts(pResHolder, pName, pSensor, rMtx, scaleType, 0);
    }

    CollisionParts* tryCreateCollisionMoveLimit(LiveActor* pActor, MtxPtr pMtx, HitSensor* pSensor) {
        char kcl[0x80];
        snprintf(kcl, sizeof(kcl), "%s.kcl", "MoveLimit");
        if (!getResourceHolder(pActor)->mFileInfoTable->isExistRes(kcl)) {
            return nullptr;
        }

        TPos3f mtx;
        mtx.set(pMtx);
        CollisionParts* pParts = createCollisionParts(getResourceHolder(pActor), "MoveLimit", pSensor, mtx, UNKNOWN_2, 3);
        pParts->_0 = (TMtx34f*)pMtx;

        if (pParts != nullptr) {
            validateCollisionParts(pParts);
        }

        return pParts;
    }

    CollisionParts* tryCreateCollisionWaterSurface(LiveActor* pActor, MtxPtr pMtx, HitSensor* pSensor) {
        char kcl[0x80];
        snprintf(kcl, sizeof(kcl), "%s.kcl", "WaterSurface");
        if (!getResourceHolder(pActor)->mFileInfoTable->isExistRes(kcl)) {
            return nullptr;
        }

        TPos3f mtx;
        mtx.set(pMtx);
        CollisionParts* pParts = createCollisionParts(getResourceHolder(pActor), "WaterSurface", pSensor, mtx, UNKNOWN_2, 2);
        pParts->_0 = (TMtx34f*)pMtx;

        if (pParts != nullptr) {
            validateCollisionParts(pParts);
        }

        return pParts;
    }

    CollisionParts* tryCreateCollisionSunshade(LiveActor* pActor, MtxPtr pMtx, HitSensor* pSensor) {
        char kcl[0x80];
        snprintf(kcl, sizeof(kcl), "%s.kcl", "Sunshade");
        if (!getResourceHolder(pActor)->mFileInfoTable->isExistRes(kcl)) {
            return nullptr;
        }

        TPos3f mtx;
        mtx.set(pMtx);
        CollisionParts* pParts = createCollisionParts(getResourceHolder(pActor), "Sunshade", pSensor, mtx, UNKNOWN_2, 1);
        pParts->_0 = (TMtx34f*)pMtx;

        if (pParts != nullptr) {
            validateCollisionParts(pParts);
        }

        return pParts;
    }

    bool tryCreateCollisionAllOtherCategory(LiveActor* pActor, HitSensor* pSensor, CollisionParts** pMoveLimit, CollisionParts** pWaterSurface,
                                            CollisionParts** pSunshade) {
        CollisionParts* moveLimit = tryCreateCollisionMoveLimit(pActor, pSensor);
        CollisionParts* waterSurface = tryCreateCollisionWaterSurface(pActor, pSensor);
        CollisionParts* sunshade = tryCreateCollisionSunshade(pActor, pSensor);
        if (pMoveLimit != nullptr) {
            *pMoveLimit = moveLimit;
        }

        if (pWaterSurface != nullptr) {
            *pWaterSurface = waterSurface;
        }

        if (pSunshade != nullptr) {
            *pSunshade = sunshade;
        }

        return sunshade != nullptr;
    }

    bool tryCreateCollisionAllOtherCategory(LiveActor* pActor, MtxPtr pMtx, HitSensor* pSensor, CollisionParts** pMoveLimit,
                                            CollisionParts** pWaterSurface, CollisionParts** pSunshade) {
        CollisionParts* moveLimit = tryCreateCollisionMoveLimit(pActor, pMtx, pSensor);
        CollisionParts* waterSurface = tryCreateCollisionWaterSurface(pActor, pMtx, pSensor);
        CollisionParts* sunshade = tryCreateCollisionSunshade(pActor, pMtx, pSensor);
        if (pMoveLimit != nullptr) {
            *pMoveLimit = moveLimit;
        }

        if (pWaterSurface != nullptr) {
            *pWaterSurface = waterSurface;
        }

        if (pSunshade != nullptr) {
            *pSunshade = sunshade;
        }

        return sunshade != nullptr;
    }

    bool isExistKcl(LiveActor* pActor, const char* pName) {
        char kclName[0x80];
        snprintf(kclName, sizeof(kclName), "%s.kcl", pName);
        return getResourceHolder(pActor)->mFileInfoTable->isExistRes(kclName);
    }

    f32 getCollisionBoundingSphereRange(const LiveActor* pActor) {
        return pActor->mCollisionParts->_D8;
    }

    bool isValidCollisionParts(LiveActor* pActor) {
        return pActor->mCollisionParts->_CC;
    }

    void validateCollisionParts(LiveActor* pActor) {
        CollisionParts* pParts = pActor->mCollisionParts;
        validateCollisionParts(pParts);

        if (pParts->_0 != nullptr) {
            pParts->updateBoundingSphereRange();
        } else {
            pParts->updateBoundingSphereRange(pActor->mPosition);
        }

        resetAllCollisionMtx(pActor);
    }

    void validateCollisionParts(CollisionParts* pParts) {
        if (!pParts->_CC) {
            pParts->addToBelongZone();
        }

        pParts->_CC = true;
    }

    void invalidateCollisionParts(LiveActor* pActor) {
        invalidateCollisionParts(pActor->mCollisionParts);
    }

    void invalidateCollisionParts(CollisionParts* pParts) {
        if (pParts->_CC) {
            pParts->removeFromBelongZone();
        }

        pParts->_CC = false;
    }

    void validateExCollisionParts(LiveActor* pActor) {
        *(u8*)&pActor->mBinder->_1EC |= 0x20;
    }

    void invalidateExCollisionParts(LiveActor* pActor) {
        *(u8*)&pActor->mBinder->_1EC &= ~0x20;
    }

    void onUpdateCollisionParts(LiveActor* pActor) {
        CollisionParts* pParts = pActor->mCollisionParts;
        if (!pParts->_CC) {
            validateCollisionParts(pActor);
        }

        pParts->_CD = true;
    }

    void onUpdateCollisionPartsOnetimeImmediately(LiveActor* pActor) {
        CollisionParts* pParts = pActor->mCollisionParts;
        if (!pParts->_CC) {
            validateCollisionParts(pActor);
        }

        pParts->_CE = true;
        pActor->calcAndSetBaseMtx();

        TPos3f mtx;
        makeMtxTR((MtxPtr)mtx.toMtxPtr(), pActor);
        setCollisionMtx(pActor, pParts);
        pParts->updateMtx();
    }

    void offUpdateCollisionParts(LiveActor* pActor) {
        CollisionParts* pParts = pActor->mCollisionParts;
        if (!pParts->_CC) {
            validateCollisionParts(pActor);
        }

        pParts->_CD = false;
    }

    void resetAllCollisionMtx(LiveActor* pActor) {
        CollisionParts* pParts = pActor->mCollisionParts;
        if (pParts->_0 != nullptr) {
            pParts->resetAllMtx();
            return;
        }

        TPos3f mtx;
        calcCollisionMtx(&mtx, pActor);
        pParts->resetAllMtx(mtx);
    }

    u32 getCollisionSensorType(const CollisionParts* pParts) {
        return pParts->mHitSensor->mType;
    }

    ModelObj* createModelObjMapObj(const char* pName, const char* pModelName, MtxPtr pMtx) {
        ModelObj* pObj = new ModelObj(pName, pModelName, pMtx, 8, -2, -2, false);
        pObj->initWithoutIter();
        return pObj;
    }

    ModelObj* createModelObjMapObjStrongLight(const char* pName, const char* pModelName, MtxPtr pMtx) {
        ModelObj* pObj = new ModelObj(pName, pModelName, pMtx, 10, -2, -2, false);
        pObj->initWithoutIter();
        return pObj;
    }

    ModelObj* createModelObjNoSilhouettedMapObj(const char* pName, const char* pModelName, MtxPtr pMtx) {
        ModelObj* pObj = new ModelObj(pName, pModelName, pMtx, 13, -2, -2, false);
        pObj->initWithoutIter();
        return pObj;
    }

    ModelObj* createModelObjNoSilhouettedMapObjStrongLight(const char* pName, const char* pModelName, MtxPtr pMtx) {
        ModelObj* pObj = new ModelObj(pName, pModelName, pMtx, 15, -2, -2, false);
        pObj->initWithoutIter();
        return pObj;
    }

    ModelObj* createModelObjIndirectMapObj(const char* pName, const char* pModelName, MtxPtr pMtx) {
        ModelObj* pObj = new ModelObj(pName, pModelName, pMtx, 25, -2, -2, false);
        pObj->initWithoutIter();
        return pObj;
    }

    ModelObj* createModelObjPlayerDecoration(const char* pName, const char* pModelName, MtxPtr pMtx) {
        ModelObj* pObj = new ModelObj(pName, pModelName, pMtx, 21, -2, -2, false);
        pObj->initWithoutIter();
        return pObj;
    }

    ModelObj* createModelObjEnemy(const char* pName, const char* pModelName, MtxPtr pMtx) {
        ModelObj* pObj = new ModelObj(pName, pModelName, pMtx, 18, 43, -2, false);
        pObj->initWithoutIter();
        return pObj;
    }

    ModelObj* createModelObjNpc(const char* pName, const char* pModelName, MtxPtr pMtx) {
        ModelObj* pObj = new ModelObj(pName, pModelName, pMtx, 16, -2, -2, false);
        pObj->initWithoutIter();
        return pObj;
    }

    ModelObj* createModelObjPlanetLow(const char* pName, const char* pModelName, MtxPtr pMtx) {
        ModelObj* pObj = new ModelObj(pName, pModelName, pMtx, 5, -2, -2, false);
        pObj->initWithoutIter();
        return pObj;
    }

    ModelObj* createModelObjBloomModel(const char* pName, const char* pModelName, MtxPtr pMtx) {
        ModelObj* pObj = new ModelObj(pName, pModelName, pMtx, 30, -2, -2, false);
        pObj->initWithoutIter();
        registerDemoSimpleCastAll(pObj);
        return pObj;
    }

    PartsModel* createPartsModelMapObj(LiveActor* pHost, const char* pName, const char* pModelName, MtxPtr pMtx) {
        PartsModel* pModel = new PartsModel(pHost, pName, pModelName, pMtx, 8, false);
        pModel->initWithoutIter();
        return pModel;
    }

    PartsModel* createPartsModelMapObjStrongLight(LiveActor* pHost, const char* pName, const char* pModelName, MtxPtr pMtx) {
        PartsModel* pModel = new PartsModel(pHost, pName, pModelName, pMtx, 10, false);
        pModel->initWithoutIter();
        return pModel;
    }

    PartsModel* createPartsModelNoSilhouettedMapObj(LiveActor* pHost, const char* pName, const char* pModelName, MtxPtr pMtx) {
        PartsModel* pModel = new PartsModel(pHost, pName, pModelName, pMtx, 13, false);
        pModel->initWithoutIter();
        return pModel;
    }

    PartsModel* createPartsModelEnemy(LiveActor* pHost, const char* pName, const char* pModelName, MtxPtr pMtx) {
        PartsModel* pModel = new PartsModel(pHost, pName, pModelName, pMtx, 18, false);
        pModel->initWithoutIter();
        return pModel;
    }

    PartsModel* createPartsModelNpc(LiveActor* pHost, const char* pName, const char* pModelName, MtxPtr pMtx) {
        PartsModel* pModel = new PartsModel(pHost, pName, pModelName, pMtx, 16, false);
        pModel->initWithoutIter();
        pModel->_99 = true;
        return pModel;
    }

    PartsModel* createPartsModelIndirectNpc(LiveActor* pHost, const char* pName, const char* pModelName, MtxPtr pMtx) {
        PartsModel* pModel = new PartsModel(pHost, pName, pModelName, pMtx, 27, false);
        pModel->initWithoutIter();
        pModel->_99 = true;
        return pModel;
    }

    PartsModel* createPartsModelEnemyAndFix(LiveActor* pHost, const char* pName, const char* pModelName, MtxPtr pMtx, const TVec3f& rPos,
                                            const TVec3f& rRot, const char* pJointName) {
        PartsModel* pModel = new PartsModel(pHost, pName, pModelName, pMtx, 18, false);
        pModel->initWithoutIter();
        pModel->initFixedPosition(rPos, rRot, pJointName);
        return pModel;
    }

    PartsModel* createPartsModelNpcAndFix(LiveActor* pHost, const char* pName, const char* pModelName, const char* pJointName) {
        PartsModel* pModel = new PartsModel(pHost, pName, pModelName, nullptr, 16, false);
        pModel->initWithoutIter();
        pModel->initFixedPosition(pJointName);
        pModel->_99 = true;
        return pModel;
    }

    LodCtrl* createLodCtrlNPC(LiveActor* pActor, const JMapInfoIter& rIter) {
        LodCtrl* pLod = new LodCtrl(pActor, rIter);
        pLod->createLodModel(16, 40, -1);
        pLod->syncMaterialAnimation();
        pLod->syncJointAnimation();
        pLod->initLightCtrl();
        pLod->offSyncShadowHost();
        pLod->_1B = true;
        return pLod;
    }

    LodCtrl* createLodCtrlPlanet(LiveActor* pActor, const JMapInfoIter& rIter, f32 farClip, s32 lowModelType) {
        LodCtrl* pLod = new LodCtrl(pActor, rIter);
        pLod->createLodModel(5, lowModelType, 1);
        pLod->setDistanceToMiddleAndLow(5000.0f, 10000.0f);
        pLod->setFarClipping(farClip);

        const char* pResName = getModelResourceHolder(pActor)->mModelResTable->getResName(0UL);
        if (pLod->_10 != nullptr) {
            tryStartAllAnim(pLod->_10, pResName);
        }

        if (pLod->_14 != nullptr) {
            tryStartAllAnim(pLod->_14, pResName);
        }

        return pLod;
    }

    LodCtrl* createLodCtrlMapObj(LiveActor* pActor, const JMapInfoIter& rIter, f32 farClip) {
        LodCtrl* pLod = new LodCtrl(pActor, rIter);
        pLod->createLodModel(8, -1, 5);
        pLod->setDistanceToMiddleAndLow(5000.0f, 10000.0f);
        pLod->setFarClipping(farClip);

        const char* pResName = getModelResourceHolder(pActor)->mModelResTable->getResName(0UL);
        if (pLod->_10 != nullptr) {
            tryStartAllAnim(pLod->_10, pResName);
        }

        if (pLod->_14 != nullptr) {
            tryStartAllAnim(pLod->_14, pResName);
        }

        return pLod;
    }

    Flag* createMapFlag(const char* pName, const char* pInfoName, const TVec3f* pPos, const TVec3f& rRot, f32 a5, f32 a6, f32 a7, s32 a8, s32 a9,
                        f32 a10) {
        void* pAlloc = operator new(0x134);
        Flag* pFlag = pAlloc != nullptr ? new (pAlloc) Flag(pName) : nullptr;
        if (pFlag != nullptr) {
            pFlag->setInfoPos(pInfoName, pPos, rRot, a5, a6, a7, a8, a9, a10);
            pFlag->initWithoutIter();
        }

        return pFlag;
    }

    void stopSceneAtStep(const LiveActor* pActor, s32 step, s32 sceneStep) {
        if (isStep(pActor, step)) {
            stopScene(sceneStep);
        }
    }

    void tryRumblePadAndCameraDistanceVeryStrong(const LiveActor* pActor, f32 near, f32 middle, f32 far) {
        if (isNearPlayerAnyTime(pActor, near)) {
            tryRumblePadVeryStrong(pActor, 0);
            shakeCameraStrong();
        } else if (isNearPlayerAnyTime(pActor, middle)) {
            tryRumblePadStrong(pActor, 0);
            shakeCameraNormal();
        } else if (isNearPlayerAnyTime(pActor, far)) {
            tryRumblePadMiddle(pActor, 0);
            shakeCameraNormalWeak();
        }
    }

    void tryRumblePadAndCameraDistanceStrong(const LiveActor* pActor, f32 near, f32 middle, f32 far) {
        if (isNearPlayerAnyTime(pActor, near)) {
            tryRumblePadStrong(pActor, 0);
            shakeCameraNormal();
        } else if (isNearPlayerAnyTime(pActor, middle)) {
            tryRumblePadMiddle(pActor, 0);
            shakeCameraNormalWeak();
        } else if (isNearPlayerAnyTime(pActor, far)) {
            tryRumblePadWeak(pActor, 0);
            shakeCameraWeak();
        }
    }

    void tryRumblePadAndCameraDistanceMiddle(const LiveActor* pActor, f32 near, f32 middle, f32 far) {
        if (isNearPlayerAnyTime(pActor, near)) {
            tryRumblePadMiddle(pActor, 0);
            shakeCameraNormalWeak();
        } else if (isNearPlayerAnyTime(pActor, middle)) {
            tryRumblePadWeak(pActor, 0);
            shakeCameraWeak();
        } else if (isNearPlayerAnyTime(pActor, far)) {
            tryRumblePadVeryWeak(pActor, 0);
            shakeCameraVeryWeak();
        }
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

    void setCollisionMtx(LiveActor* pActor, CollisionParts* pParts) {
        if (!pParts->_CC) {
            return;
        }

        if (pParts->_0 != nullptr) {
            pParts->setMtx();
            return;
        }

        TPos3f mtx;
        calcCollisionMtx(&mtx, pActor);
        pParts->setMtx(mtx);
    }

    CollisionParts* getCollisionParts(const LiveActor* pActor) {
        return pActor->mCollisionParts;
    }

    bool isExistCollisionParts(const LiveActor* pActor) {
        return pActor->mCollisionParts != nullptr;
    }
};  // namespace MR

void JAUSoundAnimator::setLoopFrame(f32 start, f32 end) {
    JAUSoundAnimation* pAnim = reinterpret_cast< JAUSoundAnimation* >(this->_8);
    *reinterpret_cast< f32* >(reinterpret_cast< u8* >(this) + 0x20) = start;
    *reinterpret_cast< u32* >(reinterpret_cast< u8* >(this) + 0x18) = pAnim->getStartSoundIndex(start);
    *reinterpret_cast< f32* >(reinterpret_cast< u8* >(this) + 0x24) = end;
    *reinterpret_cast< u32* >(reinterpret_cast< u8* >(this) + 0x1C) = pAnim->getEndSoundIndex(end);
}
