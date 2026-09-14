#include "Game/Map/CollisionCategorizedKeeper.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/CollisionDirector.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/CollisionPartsFilter.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

CollisionCategorizedKeeper::CollisionCategorizedKeeper(s32 category)
    : NameObj("地形コリジョンカテゴリキーパー"), mHitInfoArray(), _10(), mPartsCount(), mCategory(category), mZonesInitialized(),
      mUpdateZoneBounds(true) {
    mHitInfoArray = new HitInfo[32];
}

void CollisionCategorizedKeeper::movement() {
    CollisionParts* pParts;

    for (CollisionZone** pZone = mZones.begin(); pZone != mZones.end(); pZone++) {
        s32 count = (*pZone)->mParts.size();

        for (int i = 0; i < count; i++) {
            pParts = (*pZone)->mParts[i];

            if (pParts->_CC) {
                if (mCategory == pParts->mKeeperIndex) {
                    pParts->updateMtx();
                }

                if (mUpdateZoneBounds) {
                    (*pZone)->calcMinMaxAndRadius();
                } else if (pParts->_D4 == 0) {
                    (*pZone)->calcMinMaxAndRadiusIfMoveOuter(pParts);
                }
            }
        }
    }

    mUpdateZoneBounds = false;
}

void CollisionCategorizedKeeper::addToZone(CollisionParts* pParts, s32 zone) {
    mZones[zone]->addParts(pParts);
    mPartsCount++;
}

void CollisionCategorizedKeeper::removeFromZone(CollisionParts* pParts, s32 zone) {
    mZones[zone]->eraseParts(pParts);
    mPartsCount--;
}

void CollisionCategorizedKeeper::addToGlobal(CollisionParts* pParts) {
    mZones[0]->addParts(pParts);
    mPartsCount++;
}

void CollisionCategorizedKeeper::removeFromGlobal(CollisionParts* pParts) {
    mZones[0]->eraseParts(pParts);
    mPartsCount--;
}

s32 CollisionCategorizedKeeper::checkStrikePoint(const TVec3f& rPosition, HitInfo* pHitInfo) {
    MR::getCollisionDirector();
    _10 = 0;

    for (CollisionZone** pZone = mZones.begin(); pZone != mZones.end(); pZone++) {
        if (pZone != mZones.begin()) {
            if (!isSphereOverlappingWithBox((*pZone)->mMin, (*pZone)->mMax, rPosition, 0.0f)) {
                continue;
            }

            f32 radius = (*pZone)->mRadius;
            TVec3f offset((*pZone)->mCenter);
            offset -= rPosition;

            if (radius * radius < offset.squared()) {
                continue;
            }
        }

        int count = (*pZone)->mParts.size();
        CollisionParts* pParts;

        for (int i = 0; i < count; i++) {
            pParts = (*pZone)->mParts[i];

            if (!pParts->_CC) {
                continue;
            }

            f32 radius = pParts->_D8;
            TVec3f offset;
            offset.x = MR::abs(pParts->getTrans().x - rPosition.x);

            if (radius < offset.x) {
                continue;
            }

            offset.y = MR::abs(pParts->getTrans().y - rPosition.y);

            if (radius < offset.y) {
                continue;
            }

            offset.z = MR::abs(pParts->getTrans().z - rPosition.z);

            if (radius < offset.z) {
                continue;
            }

            if (offset.squared() > radius * radius) {
                continue;
            }

            if (pParts->checkStrikePoint(pHitInfo, rPosition)) {
                _10 = 1;
                return 1;
            }
        }
    }

    return _10;
}

s32 CollisionCategorizedKeeper::checkStrikeBall(const TVec3f& rPosition, f32 ballRadius, bool moving, const CollisionPartsFilterBase* pPartsFilter,
                                                const TriangleFilterBase* pTriangleFilter) {
    MR::getCollisionDirector();
    _10 = 0;
    s32 hitCount = 0;

    for (CollisionZone** pZone = mZones.begin(); pZone != mZones.end(); pZone++) {
        if (pZone != mZones.begin()) {
            if (!isSphereOverlappingWithBox((*pZone)->mMin, (*pZone)->mMax, rPosition, ballRadius)) {
                continue;
            }

            f32 radius = ballRadius + (*pZone)->mRadius;
            TVec3f offset((*pZone)->mCenter);
            offset -= rPosition;

            if (radius * radius < offset.squared()) {
                continue;
            }
        }

        s32 count = (*pZone)->mParts.size();
        CollisionParts* pParts;

        for (int i = 0; i < count; i++) {
            pParts = (*pZone)->mParts[i];

            if (!pParts->_CC) {
                continue;
            }

            if (pPartsFilter != nullptr && pPartsFilter->isInvalidParts(pParts)) {
                continue;
            }

            f32 radius = ballRadius + pParts->_D8;
            TVec3f offset;
            offset.x = MR::abs(pParts->getTrans().x - rPosition.x);

            if (radius < offset.x) {
                continue;
            }

            offset.y = MR::abs(pParts->getTrans().y - rPosition.y);

            if (radius < offset.y) {
                continue;
            }

            offset.z = MR::abs(pParts->getTrans().z - rPosition.z);

            if (radius < offset.z) {
                continue;
            }

            if (offset.squared() > radius * radius) {
                continue;
            }

            hitCount += pParts->checkStrikeBall(&mHitInfoArray[hitCount], 32 - hitCount, rPosition, ballRadius, moving, pTriangleFilter);

            if (hitCount >= 32) {
                _10 = hitCount;
                return hitCount;
            }
        }
    }

    _10 = hitCount;
    return hitCount;
}

s32 CollisionCategorizedKeeper::checkStrikeBallWithThickness(const TVec3f& rPosition, f32 ballRadius, f32 thickness,
                                                             const CollisionPartsFilterBase* pPartsFilter,
                                                             const TriangleFilterBase* pTriangleFilter) {
    MR::getCollisionDirector();
    _10 = 0;
    s32 hitCount = 0;

    for (CollisionZone** pZone = mZones.begin(); pZone != mZones.end(); pZone++) {
        if (pZone != mZones.begin()) {
            if (!isSphereOverlappingWithBox((*pZone)->mMin, (*pZone)->mMax, rPosition, ballRadius)) {
                continue;
            }

            f32 radius = ballRadius + (*pZone)->mRadius;
            TVec3f offset((*pZone)->mCenter);
            offset -= rPosition;

            if (radius * radius < offset.squared()) {
                continue;
            }
        }

        s32 count = (*pZone)->mParts.size();
        CollisionParts* pParts;

        for (int i = 0; i < count; i++) {
            pParts = (*pZone)->mParts[i];

            if (!pParts->_CC) {
                continue;
            }

            if (pPartsFilter != nullptr && pPartsFilter->isInvalidParts(pParts)) {
                continue;
            }

            f32 radius = ballRadius + pParts->_D8;
            TVec3f offset;
            offset.x = MR::abs(pParts->getTrans().x - rPosition.x);

            if (radius < offset.x) {
                continue;
            }

            offset.y = MR::abs(pParts->getTrans().y - rPosition.y);

            if (radius < offset.y) {
                continue;
            }

            offset.z = MR::abs(pParts->getTrans().z - rPosition.z);

            if (radius < offset.z) {
                continue;
            }

            if (offset.squared() > radius * radius) {
                continue;
            }

            hitCount +=
                pParts->checkStrikeBallWithThickness(&mHitInfoArray[hitCount], 32 - hitCount, rPosition, ballRadius, thickness, pTriangleFilter);

            if (hitCount >= 32) {
                _10 = hitCount;
                return hitCount;
            }
        }
    }

    _10 = hitCount;
    return hitCount;
}

s32 CollisionCategorizedKeeper::checkStrikeLine(const TVec3f& rStart, const TVec3f& rDirection, s32 maxHits,
                                                const CollisionPartsFilterBase* pPartsFilter, const TriangleFilterBase* pTriangleFilter) {
    MR::getCollisionDirector();

    if (maxHits == 0) {
        maxHits = 32;
    }

    _10 = 0;
    s32 hitCount = 0;
    TVec3f min(rStart);
    TVec3f max(rStart);

    if (rDirection.x < 0.0f) {
        min.x += rDirection.x;
    } else {
        max.x += rDirection.x;
    }

    if (rDirection.y < 0.0f) {
        min.y += rDirection.y;
    } else {
        max.y += rDirection.y;
    }

    if (rDirection.z < 0.0f) {
        min.z += rDirection.z;
    } else {
        max.z += rDirection.z;
    }

    for (CollisionZone** pZone = mZones.begin(); pZone != mZones.end(); pZone++) {
        if (pZone != mZones.begin()) {
            TVec3f position((*pZone)->mCenter);
            f32 radius = (*pZone)->mRadius;

            if (!isSphereOverlappingWithBox(min, max, position, radius)) {
                continue;
            }

            if (!MR::checkHitSegmentSphere(position, rStart, rStart + rDirection, radius, nullptr)) {
                continue;
            }
        }

        s32 count = (*pZone)->mParts.size();
        CollisionParts* pParts;

        for (int i = 0; i < count; i++) {
            pParts = (*pZone)->mParts[i];

            if (!pParts->_CC) {
                continue;
            }

            if (pPartsFilter != nullptr && pPartsFilter->isInvalidParts(pParts)) {
                continue;
            }

            TVec3f position(pParts->getTrans());
            f32 radius = pParts->_D8;

            if (!isSphereOverlappingWithBox(min, max, position, radius)) {
                continue;
            }

            if (!MR::checkHitSegmentSphere(position, rStart, rStart + rDirection, radius, nullptr)) {
                continue;
            }

            hitCount += pParts->checkStrikeLine(&mHitInfoArray[hitCount], maxHits - hitCount, rStart, rDirection, pTriangleFilter);

            if (maxHits <= hitCount) {
                _10 = hitCount;
                return hitCount;
            }
        }
    }

    _10 = hitCount;
    return hitCount;
}

u32 CollisionCategorizedKeeper::createAreaPolygonList(Triangle* pTriangles, u32 maxTriangles, const TVec3f& rStart, const TVec3f& rEnd) {
    MR::getCollisionDirector();
    u32 triangleCount = 0;
    TVec3f min;
    TVec3f max;

    if (rStart.x < rEnd.x) {
        min.x = rStart.x;
        max.x = rEnd.x;
    } else {
        min.x = rEnd.x;
        max.x = rStart.x;
    }

    if (rStart.y < rEnd.y) {
        min.y = rStart.y;
        max.y = rEnd.y;
    } else {
        min.y = rEnd.y;
        max.y = rStart.y;
    }

    if (rStart.z < rEnd.z) {
        min.z = rStart.z;
        max.z = rEnd.z;
    } else {
        min.z = rEnd.z;
        max.z = rStart.z;
    }

    for (CollisionZone** pZone = mZones.begin(); pZone != mZones.end(); pZone++) {
        if (pZone != mZones.begin() && !isSphereOverlappingWithBox(min, max, (*pZone)->mCenter, (*pZone)->mRadius)) {
            continue;
        }

        s32 count = (*pZone)->mParts.size();
        CollisionParts* pParts;

        for (int i = 0; i < count; i++) {
            pParts = (*pZone)->mParts[i];

            if (!pParts->_CC) {
                continue;
            }

            f32 radius = pParts->_D8;
            TVec3f position(pParts->getTrans());

            if (!isSphereOverlappingWithBox(min, max, position, radius)) {
                continue;
            }

            triangleCount += pParts->createAreaPolygonList(pTriangles + triangleCount, maxTriangles - triangleCount, rStart, rEnd);

            if (maxTriangles <= triangleCount) {
                return triangleCount;
            }
        }
    }

    return triangleCount;
}

u32 CollisionCategorizedKeeper::createAreaPolygonListArray(Triangle* pTriangles, u32 maxTriangles, TVec3f* pVertices, u32 vertexCount) {
    MR::getCollisionDirector();
    u32 triangleCount = 0;
    TVec3f min;
    TVec3f max;
    MR::createBoundingBox(pVertices, vertexCount, &min, &max);

    for (CollisionZone** pZone = mZones.begin(); pZone != mZones.end(); pZone++) {
        if (pZone != mZones.begin() && !isSphereOverlappingWithBox(min, max, (*pZone)->mCenter, (*pZone)->mRadius)) {
            continue;
        }

        s32 count = (*pZone)->mParts.size();
        CollisionParts* pParts;

        for (int i = 0; i < count; i++) {
            pParts = (*pZone)->mParts[i];

            if (!pParts->_CC) {
                continue;
            }

            f32 radius = pParts->_D8;
            TVec3f position(pParts->getTrans());

            if (!isSphereOverlappingWithBox(min, max, position, radius)) {
                continue;
            }

            triangleCount += pParts->createAreaPolygonListArray(pTriangles + triangleCount, maxTriangles - triangleCount, pVertices, vertexCount);

            if (maxTriangles <= triangleCount) {
                return triangleCount;
            }
        }
    }

    return triangleCount;
}

bool CollisionCategorizedKeeper::isSphereOverlappingWithBox(const TVec3f& rMin, const TVec3f& rMax, const TVec3f& rCenter, f32 radius) {
    if (rCenter.x < rMin.x - radius || rMax.x + radius < rCenter.x) {
        return false;
    }

    if (rCenter.y < rMin.y - radius || rMax.y + radius < rCenter.y) {
        return false;
    }

    if (rCenter.z < rMin.z - radius || rMax.z + radius < rCenter.z) {
        return false;
    }

    return true;
}

bool CollisionCategorizedKeeper::searchSameHostParts(CollisionParts** pResult, CollisionParts* pParts) const {
    for (CollisionZone* const* pZone = mZones.begin(); pZone != mZones.end(); pZone++) {
        s32 count = (*pZone)->mParts.size();

        for (int i = 0; i < count; i++) {
            CollisionParts* pCandidate = (*pZone)->mParts[i];

            if (pCandidate->mHitSensor->mHost == pParts->mHitSensor->mHost) {
                *pResult = pCandidate;
                return true;
            }
        }
    }

    return false;
}

HitInfo* CollisionCategorizedKeeper::getStrikeInfo(u32 index) {
    return &mHitInfoArray[index];
}

CollisionZone* CollisionCategorizedKeeper::getZone(int zone) {
    if (!mZonesInitialized) {
        s32 count = MR::getZoneNum();

        for (int i = 0; i < count; i++) {
            mZones.push_back(new CollisionZone(i));
        }

        mZonesInitialized = true;
    }

    return mZones[zone];
}

CollisionZone::CollisionZone(s32 zoneID) : mZoneID(zoneID), mCenter(0, 0, 0), mRadius(), mMin(0, 0, 0), mMax(0, 0, 0) {
}

void CollisionZone::addParts(CollisionParts* pParts) {
    mParts.push_back(pParts);

    if (mZoneID != 0) {
        calcMinMaxAndRadius();
    }
}

void CollisionZone::calcMinMaxAndRadius() {
    CollisionParts** pCurrent;
    mMin.zero();
    mMax.zero();
    mRadius = 0.0f;

    for (CollisionParts** pParts = mParts.begin(); pParts != mParts.end(); pParts++) {
        TVec3f min((*pParts)->getTrans());
        TVec3f max((*pParts)->getTrans());
        f32 radius = (*pParts)->_D8;
        min -= TVec3f(radius, radius, radius);
        max += TVec3f(radius, radius, radius);
        addAndUpdateMinMax(min, max);
    }

    mCenter = (mMax + mMin) / 2.0f;
    f32 radius = 0.0f;

    for (pCurrent = mParts.begin(); pCurrent != mParts.end(); pCurrent++) {
        TVec3f position((*pCurrent)->getTrans());
        position -= mCenter;
        f32 outer = position.length();
        outer += (*pCurrent)->_D8;

        if (radius < outer) {
            radius = outer;
        }
    }

    mRadius = radius;
}

void CollisionZone::calcMinMaxAndRadiusIfMoveOuter(CollisionParts* pParts) {
    f32 radius = pParts->_D8;
    TVec3f position(pParts->getTrans());
    TVec3f min(mMin);
    TVec3f max(mMax);
    min.x += radius;
    min.y += radius;
    min.z += radius;
    max.x -= radius;
    max.y -= radius;
    max.z -= radius;

    if (!MR::isInRange(position.x, min.x, max.x) || !MR::isInRange(position.y, min.y, max.y) || !MR::isInRange(position.z, min.z, max.z)) {
        calcMinMaxAndRadius();
    }
}

void CollisionZone::addAndUpdateMinMax(TVec3f min, TVec3f max) {
    if (mRadius == 0.0f) {
        mRadius = 0.1f;
        mMin.set(min);
        mMax.set(max);
    } else {
        if (min.x < mMin.x) {
            mMin.x = min.x;
        }

        if (min.y < mMin.y) {
            mMin.y = min.y;
        }

        if (min.z < mMin.z) {
            mMin.z = min.z;
        }

        if (mMax.x < max.x) {
            mMax.x = max.x;
        }

        if (mMax.y < max.y) {
            mMax.y = max.y;
        }

        if (mMax.z < max.z) {
            mMax.z = max.z;
        }
    }
}

void CollisionZone::eraseParts(CollisionParts* pParts) {
    CollisionParts** pFound = mParts.begin();
    s32 count = mParts.size();
    CollisionParts** pEnd = mParts.begin() + count;
    while (pFound != pEnd && *pFound != pParts) {
        pFound++;
    }

    if (pFound != pEnd) {
        mParts[pFound - mParts.begin()] = mParts[count - 1];
        mParts.pop_back();
    }
}

CollisionCategorizedKeeper::~CollisionCategorizedKeeper() {
}

TVec3f CollisionParts::getTrans() {
    TVec3f translation;
    mBaseMatrix.getTrans(translation);
    return translation;
}
