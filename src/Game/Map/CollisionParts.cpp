#include "Game/Map/CollisionParts.hpp"
#include "Game/Camera/CameraPolygonCodeUtil.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/CollisionCategorizedKeeper.hpp"
#include "Game/Map/CollisionDirector.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Map/KCollision.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/TriangleFilter.hpp"

void CollisionParts_FORCE_MATCH_SDATA2() {
    1.0f;
    0.0f;
    3.0f;
    -1.0f;
    2.0f;
    0.001f;
    20.0f;
    1.0f / 35.0f;
}

void CollisionParts_FORCE_MATCH_GET_SCALE(const TPos3f& rMatrix, TVec3f& rScale) {
    rMatrix.getScale(rScale);
}

CollisionParts::CollisionParts()
    : _0(), mHitSensor(), _CC(), _CD(true), _CE(), _CF(), _D0(), _D4(), _D8(-1.0f), _DC(1.0f), mKeeperIndex(-1), mZone() {
    mServer = new KCollisionServer();

    mPrevBaseMatrix.identity();
    mBaseMatrix.identity();
    mMatrix.identity();
    PSMTXInverse(mBaseMatrix.toMtxPtr(), mInvBaseMatrix.toMtxPtr());
}

void CollisionParts::init(const TPos3f& rMatrix, HitSensor* pHitSensor, const void* pKclData, const void* pMapInfo, s32 keeperIndex,
                          bool useAutoScale) {
    mServer->init(const_cast< void* >(pKclData), pMapInfo);
    mHitSensor = pHitSensor;

    resetAllMtx(rMatrix);

    TVec3f scale;
    mBaseMatrix.getScale(scale);

    mZone = MR::getCollisionDirector()->getCategoryKeeper(keeperIndex)->getZone(MR::getCurrentPlacementZoneId());

    MR::initCameraCodeCollection(pHitSensor->mHost->mName, mZone->mZoneID);
    mServer->calcFarthestVertexDistance();
    MR::termCameraCodeCollection();

    updateBoundingSphereRange(scale);
    mKeeperIndex = keeperIndex;
}

void CollisionParts::addToBelongZone() {
    s32 zoneID = mZone->mZoneID;

    MR::getCollisionDirector()->getCategoryKeeper(mKeeperIndex)->addToZone(this, zoneID);
}

void CollisionParts::removeFromBelongZone() {
    s32 zoneID = mZone->mZoneID;

    MR::getCollisionDirector()->getCategoryKeeper(mKeeperIndex)->removeFromZone(this, zoneID);
}

void CollisionParts::initWithAutoEqualScale(const TPos3f& rMatrix, HitSensor* pHitSensor, const void* pKclData, const void* pMapInfo, s32 keeperIndex,
                                            bool useAutoScale) {
    _CF = true;
    _D0 = false;

    init(rMatrix, pHitSensor, pKclData, pMapInfo, keeperIndex, useAutoScale);
}

void CollisionParts::initWithNotUsingScale(const TPos3f& rMatrix, HitSensor* pHitSensor, const void* pKclData, const void* pMapInfo, s32 keeperIndex,
                                           bool useAutoScale) {
    _CF = false;
    _D0 = true;

    init(rMatrix, pHitSensor, pKclData, pMapInfo, keeperIndex, useAutoScale);
}

void CollisionParts::resetAllMtx(const TPos3f& rMatrix) {
    bool reset = false;

    if (_CD || _CE) {
        reset = true;
    }

    if (!reset) {
        return;
    }

    resetAllMtxPrivate(rMatrix);
}

void CollisionParts::resetAllMtx() {
    bool reset = false;

    if (_CD || _CE) {
        reset = true;
    }

    if (reset) {
        TPos3f matrix(_0);
        makeEqualScale(matrix.toMtxPtr());

        resetAllMtxPrivate(matrix);
    }
}

void CollisionParts::forceResetAllMtxAndSetUpdateMtxOneTime() {
    TPos3f matrix(_0);
    makeEqualScale(matrix.toMtxPtr());
    resetAllMtxPrivate(matrix);

    _CE = true;
}

void CollisionParts::resetAllMtxPrivate(const TPos3f& rMatrix) {
    mPrevBaseMatrix.setInline(rMatrix);
    mBaseMatrix.setInline(rMatrix);
    mMatrix.setInline(rMatrix);
    PSMTXInverse(mBaseMatrix.toMtxPtr(), mInvBaseMatrix.toMtxPtr());
}

void CollisionParts::setMtx(const TPos3f& rMatrix) {
    mMatrix.setInline(rMatrix);
}

void CollisionParts::setMtx() {
    mMatrix.setInline(_0);
}

void CollisionParts::updateMtx() {
    bool update = false;

    if (_CD || _CE) {
        update = true;
    }

    if (!update) {
        if (MR::isSameMtx(mMatrix.toMtxPtr(), mBaseMatrix.toMtxPtr())) {
            _D4++;
        }
    } else {
        if (MR::isSameMtx(mMatrix.toMtxPtr(), mBaseMatrix.toMtxPtr())) {
            _D4++;
        } else {
            if (_CE) {
                _D4 = 1;
            } else {
                _D4 = 0;
            }

            f32 scale = makeEqualScale(mMatrix.toMtxPtr());
            _E8 = scale;
            f32 scaleChange = scale - _DC;
            _EC = scale;
            _F0 = scale;

            if (!MR::isNearZero(scaleChange)) {
                updateBoundingSphereRangePrivate(scale);
            }
        }

        _CE = false;

        if (_D4 < 2) {
            mPrevBaseMatrix.setInline(mBaseMatrix);
            mBaseMatrix.setInline(mMatrix);
            PSMTXInverse(mBaseMatrix.toMtxPtr(), mInvBaseMatrix.toMtxPtr());
        }
    }
}

f32 CollisionParts::makeEqualScale(MtxPtr pMatrix) {
    TPos3f& mtx = *reinterpret_cast< TPos3f* >(pMatrix);

    TVec3f scale;
    mtx.getScale(scale);

    TVec3f scaleDiff(scale.x - scale.y, scale.y - scale.z, scale.z - scale.x);

    if (MR::isNearZero(scaleDiff.x) && MR::isNearZero(scaleDiff.y) && MR::isNearZero(scaleDiff.z)) {
        return scale.x;
    }

    f32 uniformScale = 1.0f;
    TVec3f invScale;

    if (_D0) {
        invScale.set< f32 >(uniformScale / scale.x, uniformScale / scale.y, uniformScale / scale.z);
        uniformScale = 1.0f;
    } else if (_CF) {
        uniformScale = (scale.x + scale.y + scale.z) / 3.0f;
        invScale.set< f32 >(uniformScale / scale.x, uniformScale / scale.y, uniformScale / scale.z);
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

void CollisionParts::updateBoundingSphereRange() {
    TPos3f matrix(_0);
    f32 scale = makeEqualScale(matrix.toMtxPtr());
    updateBoundingSphereRangePrivate(scale);
}

void CollisionParts::updateBoundingSphereRange(TVec3f scale) {
    f32 range = (scale.x + scale.y + scale.z) / 3.0f;
    updateBoundingSphereRangePrivate(range);
}

void CollisionParts::updateBoundingSphereRangePrivate(f32 scale) {
    _DC = scale;
    _D8 = scale * mServer->mMaxVertexDistance;
}

const char* CollisionParts::getHostName() const {
    if (mHitSensor == nullptr) {
        return nullptr;
    }

    LiveActor* actor = mHitSensor->mHost;

    if (actor == nullptr) {
        return nullptr;
    }

    return actor->mName;
}

s32 CollisionParts::getPlacementZoneID() const {
    return mZone->mZoneID;
}

bool CollisionParts::checkStrikePoint(HitInfo* pHitInfo, const TVec3f& rPos) {
    TVec3f localPos;
    mInvBaseMatrix.mult(rPos, localPos);
    TVec3f scale;
    mInvBaseMatrix.getScale(scale);
    f32 averageScale = (scale.x + scale.y + scale.z) / 3.0f;
    Fxyz point;
    point.x = localPos.x;
    point.y = localPos.y;
    point.z = localPos.z;
    f32 distance;
    KC_PrismData* prism = nullptr;
    u8 classification;

    if (1.0f < averageScale) {
        f32 radius = 20.0f * averageScale;
        mServer->checkSphereWithThickness(&point, radius, averageScale, 1, &prism, &distance, &classification, 2.0f * radius);
        if (prism == nullptr) {
            return false;
        }

        TVec3f offset(localPos);
        offset -= mServer->getPos(prism, 0);
        TVec3f normal(*mServer->getFaceNormal(prism));
        distance = -offset.x * normal.x - offset.y * normal.y - offset.z * normal.z;
    } else {
        prism = mServer->checkPoint(&point, averageScale, &distance);
        if (prism == nullptr) {
            return false;
        }
    }

    if (pHitInfo != nullptr) {
        pHitInfo->mParentTriangle.fillData(this, mServer->toIndex(prism), mHitSensor);
        f32 worldDistance = distance / averageScale;
        pHitInfo->_60 = worldDistance;
        pHitInfo->mHitPos = rPos + *pHitInfo->mParentTriangle.getNormal(0) * worldDistance;
    }

    return true;
}

u32 CollisionParts::checkStrikeBall(HitInfo* pHitInfo, u32 maxHits, const TVec3f& rPos, f32 radius, bool checkMovement,
                                    const TriangleFilterBase* pFilter) {
    KC_PrismData* prisms[64];
    f32 distances[64];
    u8 classifications[64];
    TPos3f inversePrev;
    TVec3f localPos;
    mInvBaseMatrix.mult(rPos, localPos);
    TVec3f scale;
    mInvBaseMatrix.getScale(scale);
    f32 averageScale = (scale.x + scale.y + scale.z) / 3.0f;
    f32 inverseScale = 1.0f / averageScale;
    radius *= averageScale;
    TVec3f movement(0, 0, 0);

    if (checkMovement && _D4 == 0) {
        PSMTXInverse(mPrevBaseMatrix.toMtxPtr(), inversePrev.toMtxPtr());
        TVec3f prevPos;
        inversePrev.mult(rPos, prevPos);
        TVec3f localMovement(localPos - prevPos);
        TVec3f worldMovement(localMovement);
        mBaseMatrix.mult33(worldMovement, worldMovement);
        s32 steps = static_cast< s32 >((1.0f / 35.0f) * localMovement.length()) + 1;
        TVec3f step(localMovement);

        if (steps > 1) {
            step /= steps;
        }

        TVec3f offset;
        offset.zero();

        for (s32 i = 0; i <= steps; i++) {
            movement.set(-(localMovement - offset));
            mBaseMatrix.mult33(movement, movement);
            const TVec3f* direction = i == steps ? nullptr : &worldMovement;

            u32 count = checkStrikeBallCore(pHitInfo, maxHits, prevPos + offset, movement, radius, averageScale, inverseScale, prisms, distances,
                                            classifications, pFilter, direction);
            if (count != 0) {
                return count;
            }

            offset += step;
        }

        return 0;
    } else {
        return checkStrikeBallCore(pHitInfo, maxHits, localPos, TVec3f(0, 0, 0), radius, averageScale, inverseScale, prisms, distances,
                                   classifications, pFilter, nullptr);
    }
}

u32 CollisionParts::checkStrikeBallCore(HitInfo* pHitInfo, u32 maxHits, const TVec3f& rPos, const TVec3f& rMovement, f32 radius, f32 scale,
                                        f32 inverseScale, KC_PrismData** pPrisms, f32* pDistances, u8* pClassifications,
                                        const TriangleFilterBase* pFilter, const TVec3f* pDirection) {
    const TVec3f* normal;
    u32 count =
        mServer->checkSphere(reinterpret_cast< Fxyz* >(const_cast< TVec3f* >(&rPos)), radius, scale, maxHits, pPrisms, pDistances, pClassifications);
    u32 hitCount = 0;

    for (u32 i = 0; i < count; i++) {
        HitInfo& hit = pHitInfo[hitCount];
        TVec3f position(rPos);
        calcCollidePosition(&position, *pPrisms[i], pClassifications[i]);
        mBaseMatrix.mult(position, hit.mHitPos);
        hit.mParentTriangle.fillData(this, mServer->toIndex(pPrisms[i]), mHitSensor);

        if (pFilter != nullptr && pFilter->isInvalidTriangle(&hit.mParentTriangle)) {
            continue;
        }

        if (pDirection != nullptr && 0.0f < pDirection->dot(*hit.mParentTriangle.getFaceNormal())) {
            continue;
        }

        hit._60 = inverseScale * pDistances[i];
        hit._88 = pClassifications[i];
        hit._7C.set(rMovement);
        normal = hit.mParentTriangle.getFaceNormal();
        hit._7C.scale(normal->dot(hit._7C), *normal);
        hitCount++;
    }

    return hitCount;
}

u32 CollisionParts::checkStrikeBallWithThickness(HitInfo* pHitInfo, u32 maxHits, const TVec3f& rPos, f32 radius, f32 thickness,
                                                 const TriangleFilterBase* pFilter) {
    KC_PrismData* prisms[64];
    f32 distances[64];
    u8 classifications[64];
    TVec3f localPos;
    mInvBaseMatrix.mult(rPos, localPos);
    TVec3f scale;
    mInvBaseMatrix.getScale(scale);
    f32 averageScale = (scale.x + scale.y + scale.z) / 3.0f;
    Fxyz point;
    point.x = localPos.x;
    point.y = localPos.y;
    point.z = localPos.z;
    f32 scaledRadius = radius * averageScale;
    u32 count = mServer->checkSphereWithThickness(&point, scaledRadius, averageScale, maxHits, prisms, distances, classifications, thickness);
    f32 inverseScale = 1.0f / averageScale;
    u32 hitCount = 0;

    for (u32 i = 0; i < count; i++) {
        HitInfo& hit = pHitInfo[hitCount];
        TVec3f position(localPos);
        calcCollidePosition(&position, *prisms[i], classifications[i]);
        mBaseMatrix.mult(position, hit.mHitPos);
        hit.mParentTriangle.fillData(this, mServer->toIndex(prisms[i]), mHitSensor);

        if (pFilter != nullptr && pFilter->isInvalidTriangle(&hit.mParentTriangle)) {
            continue;
        }

        hit._60 = inverseScale * distances[i];
        hit._88 = classifications[i];
        hitCount++;
    }

    return hitCount;
}

void CollisionParts::calcCollidePosition(TVec3f* pPos, const KC_PrismData& rPrism, u8 classification) {
    TVec3f offset;
    TVec3f normal;

    switch (classification) {
    // face, project into the triangle's plane
    case 1:
        projectToPlane(pPos, *pPos, mServer->getPos(&rPrism, 0), *mServer->getNormal(rPrism.mNormalIndex));
        break;
    // edge 0, between verts 0-2
    case 2:
        projectToPlane(pPos, *pPos, mServer->getPos(&rPrism, 0), *mServer->getNormal(rPrism.mNormalIndex));
        normal.set(*mServer->getNormal(rPrism.mEdgeIndices[0]));
        offset.set(*pPos);
        offset -= mServer->getPos(&rPrism, 0);
        *pPos += -normal * offset.dot(normal);
        break;
    // edge 1, between verts 0-1
    case 3:
        projectToPlane(pPos, *pPos, mServer->getPos(&rPrism, 0), *mServer->getNormal(rPrism.mNormalIndex));
        normal.set(*mServer->getNormal(rPrism.mEdgeIndices[1]));
        offset.set(*pPos);
        offset -= mServer->getPos(&rPrism, 0);
        *pPos += -normal * offset.dot(normal);
        break;
    // edge 2, between verts 1-2
    case 4:
        projectToPlane(pPos, *pPos, mServer->getPos(&rPrism, 0), *mServer->getNormal(rPrism.mNormalIndex));
        normal.set(*mServer->getNormal(rPrism.mEdgeIndices[2]));
        offset.set(*pPos);
        offset -= mServer->getPos(&rPrism, 1);
        *pPos += -normal * offset.dot(normal);
        break;
    // vertex 0
    case 5:
        pPos->set(mServer->getPos(&rPrism, 0));
        break;
    // vertex 1
    case 6:
        pPos->set(mServer->getPos(&rPrism, 1));
        break;
    // vertex 2
    case 7:
        pPos->set(mServer->getPos(&rPrism, 2));
        break;
    }
}

void CollisionParts::projectToPlane(TVec3f* pProjected, const TVec3f& rPos, const TVec3f& rOrigin, const TVec3f& rNormal) {
    TVec3f projected = rPos;

    f32 distance = (rPos - rOrigin).dot(rNormal);

    projected.add(-rNormal * distance);
    pProjected->set(projected);
}

u32 CollisionParts::checkStrikeLine(HitInfo* pHitInfo, u32 maxHits, const TVec3f& rPos, const TVec3f& rDirection, const TriangleFilterBase* pFilter) {
    f32 distances[64];
    KC_PrismData* prisms[64];
    u8 classifications[64];
    f32 length = rDirection.length();
    TVec3f localPos;
    TVec3f localDirection;
    mInvBaseMatrix.mult(rPos, localPos);
    mInvBaseMatrix.mult(rPos + rDirection, localDirection);
    localDirection = localDirection - localPos;
    u32 count = 0;
    mServer->checkArrow(localPos, localDirection, distances, classifications, &count, prisms, maxHits);
    u32 hitCount = 0;

    for (u32 i = 0; i < count; i++) {
        HitInfo& hit = pHitInfo[hitCount];
        TVec3f position(localPos + localDirection * distances[i]);
        mBaseMatrix.mult(position, position);
        hit.mParentTriangle.fillData(this, mServer->toIndex(prisms[i]), mHitSensor);

        if (pFilter != nullptr && pFilter->isInvalidTriangle(&hit.mParentTriangle)) {
            continue;
        }

        hit._60 = length * distances[i];
        hit.mHitPos = position;
        hit._88 = classifications[i];
        hitCount++;
    }

    return hitCount;
}

u32 CollisionParts::createAreaPolygonList(Triangle* pTriangles, u32 maxTriangles, const TVec3f& rMin, const TVec3f& rMax) {
    KC_PrismData* prisms[512];
    TPos3f rotation;
    PSMTXCopy(mInvBaseMatrix.toMtxPtr(), rotation.toMtxPtr());
    rotation.zeroTrans();
    TVec3f min;
    TVec3f max;
    mInvBaseMatrix.mult(rMin, min);
    mInvBaseMatrix.mult(rMax, max);
    u32 count = mServer->checkArea3D(reinterpret_cast< Fxyz* >(&min), reinterpret_cast< Fxyz* >(&max), prisms, maxTriangles);
    if (count == 0) {
        return 0;
    }

    for (u32 i = 0; i < count; i++) {
        pTriangles[i].fillData(this, mServer->toIndex(prisms[i]), mHitSensor);
    }

    return count;
}

u32 CollisionParts::createAreaPolygonListArray(Triangle* pTriangles, u32 maxTriangles, TVec3f* pPoints, u32 numPoints) {
    KC_PrismData* prisms[512];
    TVec3f localPoints[32];
    TPos3f rotation;
    PSMTXCopy(mInvBaseMatrix.toMtxPtr(), rotation.toMtxPtr());
    rotation.zeroTrans();

    for (u32 i = 0; i < numPoints; i++) {
        mInvBaseMatrix.mult(pPoints[i], localPoints[i]);
    }

    TVec3f min;
    TVec3f max;
    MR::createBoundingBox(localPoints, numPoints, &min, &max);
    u32 count = mServer->checkArea3D(reinterpret_cast< Fxyz* >(&min), reinterpret_cast< Fxyz* >(&max), prisms, maxTriangles);
    if (count == 0) {
        return 0;
    }

    for (u32 i = 0; i < count; i++) {
        pTriangles[i].fillData(this, mServer->toIndex(prisms[i]), mHitSensor);
    }

    return count;
}

void CollisionParts::calcForceMovePower(TVec3f* pMovement, const TVec3f& rPos) const {
    TVec3f position(rPos);
    TMtx34f inversePrev;
    PSMTXInverse(mPrevBaseMatrix, inversePrev);

    inversePrev.mult(position, position);
    mBaseMatrix.mult(position, position);

    position.sub(rPos);
    *pMovement = position;
}
