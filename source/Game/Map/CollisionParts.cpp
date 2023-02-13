#include "Game/Boss/BossStinkBug.h"
#include "Game/Camera/CameraPolygonCodeUtil.h"
#include "Game/Map/CollisionCategorizedKeeper.h"
#include "Game/Map/CollisionDirector.h"
#include "Game/Map/CollisionParts.h"
#include "Game/Map/KCollision.h"
#include "Game/LiveActor/HitSensor.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Util/SceneUtil.h"

CollisionParts::CollisionParts() {
    _0 = nullptr;
    mHitSensor = nullptr;
    _CC = false;
    _CD = true;
    _CE = false;
    _CF = false;
    _D0 = false;
    _D4 = 0;
    _D8 = -1.0f;
    _DC = 1.0f;
    mKeeperIndex = -1;
    mZone = nullptr;

    mServer = new KCollisionServer();

    mPrevBaseMatrix.identity();
    mBaseMatrix.identity();
    mMatrix.identity();
    PSMTXInverse(reinterpret_cast<MtxPtr>(&mBaseMatrix), reinterpret_cast<MtxPtr>(&mInvBaseMatrix));
}

void CollisionParts::init(const TPos3f &a1, HitSensor *pHitSensor, const void *pKclData, const void *pMapInfo, long keeperIndex, bool a6) {
    mServer->init(const_cast<void *>(pKclData), pMapInfo);
    mHitSensor = pHitSensor;

    resetAllMtx(a1);

    TVec3f scale;
    mBaseMatrix.getScale(scale);

    CollisionDirector *director = MR::getCollisionDirector();
    CollisionCategorizedKeeper *keeper = director->mKeepers[keeperIndex];
    s32 zoneID = MR::getCurrentPlacementZoneId();

    mZone = keeper->getZone(zoneID);

    MR::initCameraCodeCollection(pHitSensor->mActor->mName, mZone->mZoneID);
    mServer->calcFarthestVertexDistance();
    MR::termCameraCodeCollection();

    updateBoundingSphereRange(scale);
    mKeeperIndex = keeperIndex;
}

void CollisionParts::addToBelongZone() {
    s32 index = mKeeperIndex;
    s32 zoneID = mZone->mZoneID;

    CollisionDirector *director = MR::getCollisionDirector();
    director->mKeepers[index]->addToZone(this, zoneID);
}

void CollisionParts::removeFromBelongZone() {
    s32 index = mKeeperIndex;
    s32 zoneID = mZone->mZoneID;

    CollisionDirector *director = MR::getCollisionDirector();
    director->mKeepers[index]->removeFromZone(this, zoneID);
}

void CollisionParts::initWithAutoEqualScale(const TPos3f &a1, HitSensor *pHitSensor, const void *pKclData, const void *pMapInfo, long keeperIndex, bool a6) {
    _CF = true;
    _D0 = false;

    init(a1, pHitSensor, pKclData, pMapInfo, keeperIndex, a6);
}

void CollisionParts::initWithNotUsingScale(const TPos3f &a1, HitSensor *pHitSensor, const void *pKclData, const void *pMapInfo, long keeperIndex, bool a6) {
    _CF = false;
    _D0 = true;

    init(a1, pHitSensor, pKclData, pMapInfo, keeperIndex, a6);
}

void CollisionParts::resetAllMtx(const TPos3f &a1) {
    bool reset = false;

    if (_CD || _CE) {
        reset = true;
    }

    if (!reset) {
        return;
    }

    resetAllMtxPrivate(a1);
}

void CollisionParts::resetAllMtx() {
    bool reset = false;

    if (_CD || _CE) {
        reset = true;
    }

    if (reset) {
        TPos3f matrix;
        JMath::gekko_ps_copy12(&matrix, _0);
        makeEqualScale(reinterpret_cast<MtxPtr>(&matrix));

        resetAllMtxPrivate(matrix);
    }
}

void CollisionParts::forceResetAllMtxAndSetUpdateMtxOneTime() {
    TPos3f matrix;
    JMath::gekko_ps_copy12(&matrix, _0);
    makeEqualScale(reinterpret_cast<MtxPtr>(&matrix));
    resetAllMtxPrivate(matrix);

    _CE = true;
}

void CollisionParts::resetAllMtxPrivate(const TPos3f &a1) {
    JMath::gekko_ps_copy12(&mPrevBaseMatrix, &a1);
    JMath::gekko_ps_copy12(&mBaseMatrix, &a1);
    JMath::gekko_ps_copy12(&mMatrix, &a1);
    PSMTXInverse(reinterpret_cast<MtxPtr>(&mBaseMatrix), reinterpret_cast<MtxPtr>(&mInvBaseMatrix));
}

void CollisionParts::setMtx(const TPos3f &matrix) {
    JMath::gekko_ps_copy12(&mMatrix, &matrix);
}

void CollisionParts::setMtx() {
    JMath::gekko_ps_copy12(&mMatrix, _0);
}

void CollisionParts::updateMtx() {
    bool bVar1 = false;

    if (_CD || _CE) {
        bVar1 = true;
    }

    if (!bVar1) {
        if (MR::isSameMtx(reinterpret_cast<MtxPtr>(&mMatrix), reinterpret_cast<MtxPtr>(&mBaseMatrix))) {
            _D4++;
        }
    }
    else {
        if (MR::isSameMtx(reinterpret_cast<MtxPtr>(&mMatrix), reinterpret_cast<MtxPtr>(&mBaseMatrix))) {
            _D4++;
        }
        else {
            if (_CE) {
                _D4 = 1;
            }
            else {
                _D4 = 0;
            }

            f32 dVar4 = makeEqualScale(reinterpret_cast<MtxPtr>(&mMatrix));
            _E8 = dVar4;
            f32 var = dVar4 - _DC;
            _EC = dVar4;
            _F0 = dVar4;

            if (!MR::isNearZero(var, 0.001f)) {
                updateBoundingSphereRangePrivate(dVar4);
            }
        }

        _CE = false;

        if (_D4 < 2) {
            JMath::gekko_ps_copy12(&mPrevBaseMatrix, &mBaseMatrix);
            JMath::gekko_ps_copy12(&mBaseMatrix, &mMatrix);
            PSMTXInverse(reinterpret_cast<MtxPtr>(&mBaseMatrix), reinterpret_cast<MtxPtr>(&mInvBaseMatrix));
        }
    }
}

#ifdef NON_MATCHING
// Issues with assignments of scaleDiff
f32 CollisionParts::makeEqualScale(MtxPtr matrix) {
    TPos3f &mtx = *reinterpret_cast<TPos3f *>(matrix);

    TVec3f scale;
    mtx.getScale(scale);

    TVec3f scaleDiff;
    scaleDiff.x = scale.z - scale.x;
    scaleDiff.y = scale.y - scale.z;
    scaleDiff.z = scale.x - scale.y;

    if (MR::isNearZero(scaleDiff.x, 0.001f) && MR::isNearZero(scaleDiff.y, 0.001f) && MR::isNearZero(scaleDiff.z, 0.001f)) {
        return scale.x;
    }

    f32 uniformScale = 1.0f;
    TVec3f invScale;

    if (_D0) {
        invScale.set(uniformScale / scale.x, uniformScale / scale.y, uniformScale / scale.z);
        uniformScale = 1.0f;
    }
    else if (_CF) {
        uniformScale = (scale.x + scale.y + scale.z) / 3.0f;
        invScale.set(uniformScale / scale.x, uniformScale / scale.y, uniformScale / scale.z);
    }

    mtx.mMtx[0][0] *= invScale.x;
    mtx.mMtx[1][0] *= invScale.x;
    mtx.mMtx[2][0] *= invScale.x;

    mtx.mMtx[0][1] *= invScale.y;
    mtx.mMtx[1][1] *= invScale.y;
    mtx.mMtx[2][1] *= invScale.y;

    mtx.mMtx[0][2] *= invScale.z;
    mtx.mMtx[1][2] *= invScale.z;
    mtx.mMtx[2][2] *= invScale.z;

    return uniformScale;
}
#endif

void CollisionParts::updateBoundingSphereRange() {
    TMtx34f matrix;
    JMath::gekko_ps_copy12(&matrix, _0);
    f32 scale = makeEqualScale(reinterpret_cast<MtxPtr>(&matrix));
    updateBoundingSphereRangePrivate(scale);
}

void CollisionParts::updateBoundingSphereRange(TVec3f a1) {
    f32 range = (a1.x + a1.y + a1.z) / 3.0f;
    updateBoundingSphereRangePrivate(range);
}

void CollisionParts::updateBoundingSphereRangePrivate(float scale) {
    _DC = scale;
    _D8 = scale * mServer->mMaxVertexDistance;
}

const char *CollisionParts::getHostName() const {
    if (mHitSensor == nullptr) {
        return nullptr;
    }

    LiveActor *actor = mHitSensor->mActor;

    if (actor == nullptr) {
        return nullptr;
    }

    return actor->mName;
}

s32 CollisionParts::getPlacementZoneID() const {
    return mZone->mZoneID;
}

#ifdef NON_MATCHING
// Instruction order
void CollisionParts::projectToPlane(TVec3f *pProjected, const TVec3f &rPos, const TVec3f &rOrigin, const TVec3f &rNormal) {
    TVec3f projected = rPos;

    TVec3f relative = rPos;
    relative.sub(rOrigin);

    f32 distance = relative.dot(rNormal);

    TVec3f negNormal = TVec3f(-rNormal);
    negNormal.scale(distance);
    projected.add(negNormal);
    pProjected->set(projected);
}
#endif

void CollisionParts::calcForceMovePower(TVec3f *a1, const TVec3f &a2) const {
    TVec3f tStack88 = a2;
    TMtx34f auStack76;
    PSMTXInverse((MtxPtr)&mPrevBaseMatrix, reinterpret_cast<MtxPtr>(&auStack76));

    auStack76.mult(tStack88, tStack88);
    mBaseMatrix.mult(tStack88, tStack88);

    tStack88.sub(a2);
    *a1 = tStack88;
}
