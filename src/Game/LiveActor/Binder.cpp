#include "Game/LiveActor/Binder.hpp"
#include "Game/Map/CollisionCategorizedKeeper.hpp"
#include "Game/Map/CollisionDirector.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <algorithm>

void Binder_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    // hPrintFixReactionLength
    const f32 hOverlapAddValue = 1.2f;
}  // namespace

Binder::Binder(MtxPtr pMtx, const TVec3f* pPosition, const TVec3f* pGravity, f32 radius, f32 offsetY, u32 planeNum)
    : mTriangleFilter(), mCollisionPartsFilter(), mExCollisionParts(), _C(pMtx), _10(pPosition), _14(pGravity), mRadius(radius), mOffsetY(offsetY),
      mOffsetVec(), _24(planeNum), mPlaneNum(), mPlane(), mFixReactionVector(0, 0, 0), mGroundInfo(), _C8(), mWallInfo(), _158(), mRoofInfo(),
      _1E8() {
    if (_24 == 0) {
        mPlane = nullptr;
    } else {
        mPlane = new HitInfo[_24];
    }

    clear();
    _1EC._0 = true;
    _1EC._1 = true;
    _1EC._2 = false;
    _1EC._3 = false;
    _1EC._4 = false;
    _1EC._5 = false;
}

void Binder::setTriangleFilter(TriangleFilterBase* pFilter) {
    mTriangleFilter = pFilter;
}

void Binder::setCollisionPartsFilter(CollisionPartsFilterBase* pFilter) {
    mCollisionPartsFilter = pFilter;
}

void Binder::clear() {
    mPlaneNum = 0;
    _C8 = -99999.0f;
    _158 = -99999.0f;
    _1E8 = -99999.0f;
    mFixReactionVector.zero();
}

const HitInfo* Binder::getPlane(int index) const {
    return &mPlane[index];
}

u32 Binder::copyPlaneArrayAndSortingSensor(HitInfo** pPlanes, u32 capacity) {
    if (_24 == 0) {
        u32 count = 0;
        if (isBindedGround()) {
            pPlanes[count++] = &mGroundInfo;
        }

        if (isBindedWall()) {
            pPlanes[count++] = &mWallInfo;
        }

        if (isBindedRoof()) {
            pPlanes[count++] = &mRoofInfo;
        }

        std::sort(pPlanes, pPlanes + count, compSensor);
        return count;
    }

    for (u32 i = 0; i < mPlaneNum; i++) {
        pPlanes[i] = &mPlane[i];
    }

    std::sort(pPlanes, pPlanes + mPlaneNum, compSensor);
    return mPlaneNum;
}

bool Binder::compSensor(const HitInfo* pPlane1, const HitInfo* pPlane2) {
    return pPlane1->mParentTriangle.mSensor > pPlane2->mParentTriangle.mSensor;
}

// takes the desired movement and returns the collision-adjusted displacement
// collision sphere starts from the host position + binder offset, transformed by the host matrix when applicable
// checks our movement in small steps, pushes the sphere out of overlapping geometry, then slides the remaining movement along the contact surface
// records ground, wall, and roof contacts, stores our push-out vector, and optionally follows moving ground before returning the result
const TVec3f Binder::bind(const TVec3f& rVelocity) {
    const bool stopAtFirstHit = _1EC._5 != 0;
    _1EC._5 = false;
    clear();

    TVec3f position(*_10);
    TVec3f movement(rVelocity);
    TVec3f result;

    if (_1EC._2 && mExCollisionParts) {
        MR::getCollisionDirector()->getCategoryKeeper(0)->addToGlobal(mExCollisionParts);
    }

    if (mOffsetVec) {
        if (_1EC._4 && _C) {
            position.x += _C[0][0] * mOffsetVec->x;
            position.y += _C[1][0] * mOffsetVec->x;
            position.z += _C[2][0] * mOffsetVec->x;
            position.x += _C[0][1] * mOffsetVec->y;
            position.y += _C[1][1] * mOffsetVec->y;
            position.z += _C[2][1] * mOffsetVec->y;
            position.x += _C[0][2] * mOffsetVec->z;
            position.y += _C[1][2] * mOffsetVec->z;
            position.z += _C[2][2] * mOffsetVec->z;
        } else {
            position.x += mOffsetVec->x;
            position.y += mOffsetVec->y;
            position.z += mOffsetVec->z;
        }
    } else if (_C) {
        position.x += _C[0][1] * mOffsetY;
        position.y += _C[1][1] * mOffsetY;
        position.z += _C[2][1] * mOffsetY;
    } else {
        position.y += mOffsetY;
    }

    TVec3f startPosition(position);
    HitInfo localPlanes[32];
    u32 capacity = _24;
    HitInfo* planes;
    if (capacity == 0) {
        planes = localPlanes;
        capacity = 32;
    } else {
        planes = mPlane;
    }

    bool hasRemainingMovement;
    u32 firstHitCount = findBindedPos(&position, &movement, &hasRemainingMovement, planes, capacity, false, stopAtFirstHit);
    if (firstHitCount == 0) {
        result = movement;
    } else {
        TVec3f firstReaction(0, 0, 0);
        TVec3f reaction(0, 0, 0);
        TVec3f slideReaction(0, 0, 0);
        obtainMomentFixReaction(planes, capacity, &firstReaction, 0);
        position.add(firstReaction);
        TVec3f previousReaction(firstReaction);

        while (!stopAtFirstHit && hasRemainingMovement == true) {
            TVec3f remaining(rVelocity - movement);
            moveAlongHittedPlanes(&movement, &position, &remaining, rVelocity, previousReaction, planes, capacity, &hasRemainingMovement);
            obtainMomentFixReaction(planes, capacity, &reaction, firstHitCount);
            position.add(reaction);
            previousReaction.set(reaction);
            slideReaction.add(reaction);
            hasRemainingMovement = false;
        }

        storeContactPlane(planes, capacity);
        if (_1EC._0) {
            moveWithCollisionParts(&position, &movement);
        }

        mFixReactionVector = firstReaction + slideReaction;
        result = position - startPosition;
    }

    if (_1EC._2 && mExCollisionParts) {
        MR::getCollisionDirector()->getCategoryKeeper(0)->removeFromGlobal(mExCollisionParts);
    }

    return result;
}

void Binder::moveAlongHittedPlanes(TVec3f* pMovement, TVec3f* pPosition, TVec3f* pRemaining, const TVec3f& rVelocity, const TVec3f& rReaction,
                                   HitInfo* pPlanes, u32 capacity, bool* pHasRemainingMovement) {
    TVec3f normal(rReaction);
    MR::normalizeOrZero(&normal);
    f32 dot = pRemaining->dot(normal);
    if (dot < 0.0f) {
        pRemaining->sub(normal * dot);
    }

    if (rVelocity.dot(*pRemaining) < 0.0f) {
        *pHasRemainingMovement = false;
    } else {
        findBindedPos(pPosition, pRemaining, pHasRemainingMovement, pPlanes, capacity, true, false);
        pMovement->add(*pRemaining);
    }
}

u32 Binder::findBindedPos(TVec3f* pPosition, TVec3f* pMovement, bool* pHasRemainingMovement, HitInfo* pPlanes, u32 capacity, bool skipInitial,
                          bool stopAtFirstHit) {
    s32 steps = static_cast< s32 >((1.0f / 35.0f) * pMovement->length()) + 1;
    TVec3f step(*pMovement);
    if (steps > 1) {
        step /= steps;
    }

    pMovement->zero();
    for (s32 i = 0; i <= steps; i++) {
        if (i != 0) {
            pPosition->add(step);
            pMovement->add(step);
        } else if (_1EC._3 || skipInitial) {
            continue;
        }

        s32 hitCount;
        if (_1EC._1) {
            hitCount = Collision::checkStrikeBallToMapWithMovingReaction(*pPosition, mRadius, mCollisionPartsFilter, mTriangleFilter);
        } else {
            hitCount = Collision::checkStrikeBallToMap(*pPosition, mRadius, mCollisionPartsFilter, mTriangleFilter);
        }

        if (hitCount != 0) {
            if (i == steps) {
                *pHasRemainingMovement = false;
            } else {
                *pHasRemainingMovement = true;
            }

            return storeCurrentHitInfo(pPlanes, capacity, stopAtFirstHit);
        }
    }

    *pHasRemainingMovement = false;
    return 0;
}

bool Binder::moveWithCollisionParts(TVec3f* pPosition, TVec3f* pMovement) {
    if (_C8 < 0.0f) {
        return false;
    }

    if (!mGroundInfo.mParentTriangle.isHostMoved()) {
        return false;
    }

    TVec3f force;
    mGroundInfo.mParentTriangle.calcForceMovePower(&force, *pPosition);
    pMovement->add(force);
    pPosition->add(force);
    return true;
}

u32 Binder::storeCurrentHitInfo(HitInfo* pPlanes, u32 capacity, bool stopAtFirstHit) {
    u32 hitCount = Collision::getStrikeInfoNumMap();
    u32 count = 0;
    for (u32 i = 0; i < hitCount; i++) {
        if (capacity <= count + mPlaneNum) {
            u32 stored = capacity - mPlaneNum;
            mPlaneNum = capacity;
            return stored;
        }

        pPlanes[count + mPlaneNum] = *Collision::getStrikeInfoMap(i);
        if (!stopAtFirstHit) {
            pPlanes[i + mPlaneNum]._60 += hOverlapAddValue;
        }

        count++;
    }

    mPlaneNum += count;
    return count;
}

HitInfo& HitInfo::operator=(const HitInfo& rOther) {
    mParentTriangle.mParts = rOther.mParentTriangle.mParts;
    mParentTriangle.mIdx = rOther.mParentTriangle.mIdx;
    mParentTriangle.mSensor = rOther.mParentTriangle.mSensor;
    mParentTriangle.mNormals[0] = rOther.mParentTriangle.mNormals[0];
    mParentTriangle.mNormals[1] = rOther.mParentTriangle.mNormals[1];
    mParentTriangle.mNormals[2] = rOther.mParentTriangle.mNormals[2];
    mParentTriangle.mNormals[3] = rOther.mParentTriangle.mNormals[3];
    mParentTriangle.mPos[0] = rOther.mParentTriangle.mPos[0];
    mParentTriangle.mPos[1] = rOther.mParentTriangle.mPos[1];
    mParentTriangle.mPos[2] = rOther.mParentTriangle.mPos[2];
    _60 = rOther._60;
    mHitPos = rOther.mHitPos;
    _70 = rOther._70;
    _7C = rOther._7C;
    _88 = rOther._88;

    return *this;
}

void Binder::obtainMomentFixReaction(HitInfo* pPlanes, u32 capacity, TVec3f* pReaction, u32 start) {
    TVec3f positive(0, 0, 0);
    TVec3f negative(0, 0, 0);
    for (u32 i = start; i < mPlaneNum; i++) {
        HitInfo& plane = pPlanes[i];
        TVec3f normal(*plane.mParentTriangle.getNormal(0));
        f32 x = normal.x * plane._60;
        if (positive.x < x) {
            positive.x = x;
        } else if (x < negative.x) {
            negative.x = x;
        }

        f32 y = normal.y * plane._60;
        if (positive.y < y) {
            positive.y = y;
        } else if (y < negative.y) {
            negative.y = y;
        }

        f32 z = normal.z * plane._60;
        if (positive.z < z) {
            positive.z = z;
        } else if (z < negative.z) {
            negative.z = z;
        }

        if (_1EC._1 && !MR::isNearZero(plane._7C)) {
            x = plane._7C.x;
            if (positive.x < x) {
                positive.x = x;
            } else if (x < negative.x) {
                negative.x = x;
            }

            y = plane._7C.y;
            if (positive.y < y) {
                positive.y = y;
            } else if (y < negative.y) {
                negative.y = y;
            }

            z = plane._7C.z;
            if (positive.z < z) {
                positive.z = z;
            } else if (z < negative.z) {
                negative.z = z;
            }
        }
    }

    pReaction->set(positive);
    pReaction->add(negative);
}

void Binder::storeContactPlane(HitInfo* pPlanes, u32 capacity) {
    for (u32 i = 0; i < mPlaneNum; i++) {
        const TVec3f& normal = *pPlanes[i].mParentTriangle.getNormal(0);
        if (MR::isFloorPolygon(normal, *_14)) {
            if (_C8 < pPlanes[i]._60) {
                mGroundInfo = pPlanes[i];
                _C8 = pPlanes[i]._60;
            }
        } else if (MR::isWallPolygon(normal, *_14)) {
            if (_158 < pPlanes[i]._60) {
                mWallInfo = pPlanes[i];
                _158 = pPlanes[i]._60;
            }
        } else {
            if (_1E8 < pPlanes[i]._60) {
                mRoofInfo = pPlanes[i];
                _1E8 = pPlanes[i]._60;
            }
        }
    }
}
