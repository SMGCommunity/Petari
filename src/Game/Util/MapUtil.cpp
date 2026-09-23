#include "Game/Util/MapUtil.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/CollisionCategorizedKeeper.hpp"
#include "Game/Map/CollisionCode.hpp"
#include "Game/Map/CollisionDirector.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/CollisionPartsFilter.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/TriangleFilter.hpp"

static HitInfo mSortBuffer[32];
static u32 mSortCount;

void MapUtil_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)1000000.0f;
    (void)0.001f;
    (void)0.34202015f;
    (void)1000.0f;
    (void)150.0f;
    (void)2.5f;
    (void)4.0f;
    (void)5.0f;
    (void)35.0f;
    (void)5000.0f;
}

namespace {
    u32 getStrikeInfoNumCategory(s32 category) NO_INLINE;
    bool getFirstPolyOnLineCategoryExceptActor(TVec3f*, Triangle*, const TVec3f&, const TVec3f&, const LiveActor*, s32) NO_INLINE;

    u32 getStrikeInfoNumCategory(s32 category) {
        return MR::getCollisionDirector()->getCategoryKeeper(category)->_10;
    }

    const HitInfo* getStrikeInfoCategory(s32 category, u32 index) {
        return MR::getCollisionDirector()->getCategoryKeeper(category)->getStrikeInfo(index);
    }

    bool getFirstPolyOnLineCategory(TVec3f* pPos, Triangle* pTriangle, const TVec3f& rStart, const TVec3f& rDirection,
                                    const TriangleFilterBase* pTriangleFilter, const CollisionPartsFilterBase* pPartsFilter, s32 category) {
        u32 count = MR::getCollisionDirector()->getCategoryKeeper(category)->checkStrikeLine(rStart, rDirection, 0, pPartsFilter, nullptr);
        const HitInfo* pHit;
        if (count == 0) {
            return false;
        }

        f32 minDistance = 1000000.0f;
        s32 nearest = -1;
        for (u32 i = 0; i < count; i++) {
            pHit = getStrikeInfoCategory(category, i);
            if (pTriangleFilter && pTriangleFilter->isInvalidTriangle(&pHit->mParentTriangle)) {
                continue;
            }

            if (minDistance > pHit->_60) {
                nearest = i;
                minDistance = pHit->_60;
            }
        }

        if (nearest == -1) {
            return false;
        }

        pHit = getStrikeInfoCategory(category, nearest);
        if (pPos) {
            *pPos = pHit->mHitPos;
        }

        if (pTriangle) {
            *pTriangle = pHit->mParentTriangle;
        }

        return true;
    }

    bool getFirstPolyOnLineCategoryExceptSensor(TVec3f* pPos, Triangle* pTriangle, const TVec3f& rStart, const TVec3f& rDirection,
                                                const HitSensor* pSensor, s32 category) {
        CollisionPartsFilterSensor filter(pSensor);
        return getFirstPolyOnLineCategory(pPos, pTriangle, rStart, rDirection, nullptr, &filter, category);
    }

    bool getFirstPolyOnLineCategoryExceptActor(TVec3f* pPos, Triangle* pTriangle, const TVec3f& rStart, const TVec3f& rDirection,
                                               const LiveActor* pActor, s32 category) {
        CollisionPartsFilterActor filter(pActor);
        return getFirstPolyOnLineCategory(pPos, pTriangle, rStart, rDirection, nullptr, &filter, category);
    }
};  // namespace

namespace MR {
    const TVec3f* getNormal(const Triangle* pTriangle) {
        return pTriangle->getNormal(0);
    }

    bool isWallPolygon(const TVec3f& rParam1, const TVec3f& rParam2) {
        if (isNearZero(rParam1)) {
            return false;
        }

        return isWallPolygon(rParam1.dot(rParam2));
    }

    bool isFloorPolygon(const TVec3f& rParam1, const TVec3f& rParam2) {
        if (isNearZero(rParam1)) {
            return false;
        }

        return isFloorPolygon(rParam1.dot(rParam2));
    }

    bool isFloorPolygonCos(const TVec3f& rParam1, const TVec3f& rParam2, f32 param3) {
        if (isNearZero(rParam1)) {
            return false;
        }

        if (-rParam1.dot(rParam2) < param3) {
            return false;
        }

        return isFloorPolygon(rParam1.dot(rParam2));
    }

    bool isWallPolygon(f32 param1) {
        return MR::abs(param1) < 0.34202015f;
    }

    bool isFloorPolygon(f32 param1) {
        if (isWallPolygon(param1)) {
            return false;
        }

        return param1 < 0.0f;
    }

    bool isCeilingPolygon(f32 param1) {
        if (isWallPolygon(param1) || isFloorPolygon(param1)) {
            return false;
        }

        return true;
    }

    bool isWaterPolygon(const Triangle* pTriangle) {
        const char* pFloorCodeString = getFloorCodeString(pTriangle);

        if (pFloorCodeString != nullptr) {
            if (strcmp(pFloorCodeString, "Water") == 0) {
                return true;
            }

            if (strcmp(pFloorCodeString, "Shallow") == 0) {
                return true;
            }
        }

        return false;
    }

    bool isThroughPolygon(const Triangle* pTriangle) {
        const char* pFloorCodeString = getFloorCodeString(pTriangle);

        if (pFloorCodeString != nullptr) {
            if (strcmp(pFloorCodeString, "Water") == 0) {
                return true;
            }

            if (strcmp(pFloorCodeString, "Shallow") == 0) {
                return true;
            }

            if (strcmp(pFloorCodeString, "PullBack") == 0) {
                return true;
            }
        }

        return false;
    }

    bool getFirstPolyOnLineToMap(TVec3f* pPos, Triangle* pTriangle, const TVec3f& rStart, const TVec3f& rDirection) {
        return ::getFirstPolyOnLineCategory(pPos, pTriangle, rStart, rDirection, nullptr, nullptr, 0);
    }

    bool getFirstPolyOnLineToMapAndMoveLimit(TVec3f* pPos, Triangle* pTriangle, const TVec3f& rStart, const TVec3f& rDirection) {
        Triangle mapTriangle;
        TVec3f mapPos;
        bool hitMap = ::getFirstPolyOnLineCategory(&mapPos, &mapTriangle, rStart, rDirection, nullptr, nullptr, 0);
        Triangle limitTriangle;
        TVec3f limitPos;
        bool hitLimit = ::getFirstPolyOnLineCategory(&limitPos, &limitTriangle, rStart, rDirection, nullptr, nullptr, 3);

        if (hitMap && hitLimit) {
            if (rStart.distance(mapPos) >= rStart.distance(limitPos)) {
                hitMap = false;
            }
        }

        if (hitMap) {
            if (pPos) {
                pPos->set(mapPos);
            }

            if (pTriangle) {
                *pTriangle = mapTriangle;
            }

            return true;
        }

        if (hitLimit) {
            if (pPos) {
                pPos->set(limitPos);
            }

            if (pTriangle) {
                *pTriangle = limitTriangle;
            }

            return true;
        }

        return false;
    }

    bool getFirstPolyOnLineToWaterSurface(TVec3f* pPos, Triangle* pTriangle, const TVec3f& rStart, const TVec3f& rDirection) {
        return ::getFirstPolyOnLineCategory(pPos, pTriangle, rStart, rDirection, nullptr, nullptr, 2);
    }

    bool getFirstPolyOnLineToMapExceptSensor(TVec3f* pPos, Triangle* pTriangle, const TVec3f& rStart, const TVec3f& rDirection,
                                             const HitSensor* pSensor) {
        return ::getFirstPolyOnLineCategoryExceptSensor(pPos, pTriangle, rStart, rDirection, pSensor, 0);
    }

    bool getFirstPolyOnLineToMapExceptActor(TVec3f* pPos, Triangle* pTriangle, const TVec3f& rStart, const TVec3f& rDirection,
                                            const LiveActor* pActor) {
        return ::getFirstPolyOnLineCategoryExceptActor(pPos, pTriangle, rStart, rDirection, pActor, 0);
    }

    bool getFirstPolyOnLineToMap(TVec3f* pPos, Triangle* pTriangle, const TVec3f& rStart, const TVec3f& rDirection,
                                 const CollisionPartsFilterBase* pPartsFilter, const TriangleFilterBase* pTriangleFilter) {
        return ::getFirstPolyOnLineCategory(pPos, pTriangle, rStart, rDirection, pTriangleFilter, pPartsFilter, 0);
    }

    bool getFirstPolyOnLineToWaterSurface(TVec3f* pPos, Triangle* pTriangle, const TVec3f& rStart, const TVec3f& rDirection,
                                          const CollisionPartsFilterBase* pPartsFilter, const TriangleFilterBase* pTriangleFilter) {
        return ::getFirstPolyOnLineCategory(pPos, pTriangle, rStart, rDirection, pTriangleFilter, pPartsFilter, 2);
    }

    bool getFirstPolyNormalOnLineToMap(TVec3f* pNormal, const TVec3f& rStart, const TVec3f& rDirection, TVec3f* pPos, const HitSensor* pSensor) {
        Triangle triangle;
        if (!::getFirstPolyOnLineCategoryExceptSensor(pPos, &triangle, rStart, rDirection, pSensor, 0)) {
            return false;
        }

        pNormal->set(*triangle.getFaceNormal());
        return true;
    }

    u32 getNearPolyOnLineSort(const TVec3f& rOrigin, const TVec3f& rStart, const TVec3f& rDirection, const HitSensor* pSensor) {
        u32 count = getCollisionDirector()->getCategoryKeeper(0)->checkStrikeLine(rStart, rDirection, 0, nullptr, nullptr);
        if (count == 0) {
            return 0;
        }

        const HitInfo* hits[32];
        u32 excluded = 0;
        for (u32 i = 0; i < count; i++) {
            hits[i] = getCollisionDirector()->getCategoryKeeper(0)->getStrikeInfo(i);
            if (pSensor && hits[i]->mParentTriangle.getSensor() == pSensor) {
                hits[i] = nullptr;
                excluded++;
            }
        }

        mSortCount = count - excluded;
        if (mSortCount >= 32) {
            mSortCount = 32;
        }

        for (u32 i = 0; i < mSortCount; i++) {
            f32 minDistance = 1000000.0f;
            u32 nearest = 0;
            for (u32 j = 0; j < count; j++) {
                if (hits[j]) {
                    const HitInfo* pHit = getCollisionDirector()->getCategoryKeeper(0)->getStrikeInfo(j);
                    TVec3f offset(rOrigin);
                    offset.sub(pHit->mHitPos);
                    f32 distance = offset.length();
                    if (minDistance > distance) {
                        nearest = j;
                        minDistance = distance;
                    }
                }
            }

            mSortBuffer[i] = *getCollisionDirector()->getCategoryKeeper(0)->getStrikeInfo(nearest);
            hits[nearest] = nullptr;
        }

        return mSortCount;
    }

    bool getSortedPoly(TVec3f* pDst, Triangle* pTriangle, u32 sortIndex) {
        if (mSortCount <= sortIndex) {
            return false;
        }

        const HitInfo& rHit = mSortBuffer[sortIndex];
        if (pTriangle != nullptr) {
            *pTriangle = rHit.mParentTriangle;
        }

        if (pDst != nullptr) {
            *pDst = rHit.mHitPos;
        }

        return true;
    }

    const Triangle* getSortedPoly(u32 sortIndex) {
        if (mSortCount <= sortIndex) {
            return nullptr;
        }

        return &mSortBuffer[sortIndex].mParentTriangle;
    }

    bool isExistMapCollision(const TVec3f& rParam1, const TVec3f& rParam2) {
        return getCollisionDirector()->getCategoryKeeper(0)->checkStrikeLine(rParam1, rParam2, 1, nullptr, nullptr) != 0;
    }

    bool isExistMoveLimitCollision(const TVec3f& rParam1, const TVec3f& rParam2) {
        return getCollisionDirector()->getCategoryKeeper(3)->checkStrikeLine(rParam1, rParam2, 1, nullptr, nullptr) != 0;
    }

    bool isExistMapCollisionExceptActor(const TVec3f& rStart, const TVec3f& rDirection, const LiveActor* pActor) {
        CollisionPartsFilterActor filter(pActor);
        return getCollisionDirector()->getCategoryKeeper(0)->checkStrikeLine(rStart, rDirection, 1, &filter, nullptr) != 0;
    }

    bool checkStrikePointToMap(const TVec3f& rParam1, HitInfo* pParam2) {
        return getCollisionDirector()->getCategoryKeeper(0)->checkStrikePoint(rParam1, pParam2) != 0;
    }

    bool checkStrikeBallToMap(const TVec3f& rParam1, f32 param2) {
        return getCollisionDirector()->getCategoryKeeper(0)->checkStrikeBall(rParam1, param2, false, nullptr, nullptr) != 0;
    }

    bool calcMapGround(const TVec3f& rPos, TVec3f* pGround, f32 length) {
        TVec3f direction(0.0f, -length, 0.0f);
        return ::getFirstPolyOnLineCategory(pGround, nullptr, rPos, direction, nullptr, nullptr, 0);
    }

    bool calcMapGroundUpper(TVec3f* pGround, const LiveActor* pActor) {
        CollisionParts* pParts = getCollisionParts(pActor);
        TVec3f gravity;
        calcGravityVector(pActor, pActor->mPosition, &gravity, nullptr, 0);
        f32 radius = 0.0f;
        calcModelBoundingRadius(&radius, pActor);
        f32 upperDistance = 2.5f * radius;
        f32 length = 4.0f * radius;
        TVec3f start(pActor->mPosition - gravity * upperDistance);
        TVec3f direction(gravity * length);
        HitInfo hit;
        pParts->checkStrikeLine(&hit, 1, start, direction, nullptr);
        CollisionPartsFilterSensor filter(pParts->mHitSensor);
        return ::getFirstPolyOnLineCategory(pGround, nullptr, hit.mHitPos, direction, nullptr, &filter, 0);
    }

    bool isFallNextMove(const LiveActor* pActor, f32 param2, f32 param3, f32 param4, const TriangleFilterBase* pParam5) {
        return isFallNextMove(pActor->mPosition, pActor->mVelocity, pActor->mGravity, param2, param3, param4, pParam5);
    }

    bool isFallNextMove(const TVec3f& rPos, const TVec3f& rVelocity, const TVec3f& rGravity, f32 forwardDistance, f32 upperDistance,
                        f32 lowerDistance, const TriangleFilterBase* pTriangleFilter) {
        if (isNearZero(rGravity)) {
            return false;
        }

        TVec3f forward;
        forward.scaleAdd(-rGravity.dot(rVelocity), rGravity, rVelocity);
        if (isNearZero(forward)) {
            return false;
        }

        normalize(&forward);
        forward *= forwardDistance;
        TVec3f start;
        start = forward + rPos - rGravity * upperDistance;
        bool hit = getCollisionDirector()->getCategoryKeeper(0)->checkStrikeLine(start, rGravity * (upperDistance + lowerDistance), 1, nullptr,
                                                                                 pTriangleFilter) != 0;
        return !hit;
    }

    bool isFallOrDangerNextMove(const LiveActor* pActor, f32 forwardDistance, f32 upperDistance, f32 lowerDistance) {
        TriangleFilterDangerCode filter;
        return isFallNextMove(pActor, forwardDistance, upperDistance, lowerDistance, &filter);
    }

    bool isFallOrDangerNextMove(const TVec3f& rPos, const TVec3f& rVelocity, const TVec3f& rGravity, f32 forwardDistance, f32 upperDistance,
                                f32 lowerDistance) {
        TriangleFilterDangerCode filter;
        return isFallNextMove(rPos, rVelocity, rGravity, forwardDistance, upperDistance, lowerDistance, &filter);
    }

    void calcVelocityMovingPoint(const Triangle* pTriangle, const TVec3f& rPoint, TVec3f* pVelocity) {
        if (isSameMtx(pTriangle->getBaseMtx()->toMtxPtr(), pTriangle->getPrevBaseMtx()->toMtxPtr())) {
            pVelocity->zero();
            return;
        }

        TVec3f localPoint;
        PSMTXMultVec(pTriangle->getBaseInvMtx()->toMtxPtr(), rPoint, &localPoint);
        TVec3f previousPoint;
        PSMTXMultVec(pTriangle->getPrevBaseMtx()->toMtxPtr(), &localPoint, &previousPoint);
        *pVelocity = rPoint - previousPoint;
    }

    u32 createAreaPolygonList(Triangle* pTriangle, u32 param2, const TVec3f& rParam3, const TVec3f& rParam4) {
        return getCollisionDirector()->getCategoryKeeper(0)->createAreaPolygonList(pTriangle, param2, rParam3, rParam4);
    }

    u32 createAreaPolygonListArray(Triangle* pTriangle, u32 param2, TVec3f* pParam3, u32 param4) {
        return getCollisionDirector()->getCategoryKeeper(0)->createAreaPolygonListArray(pTriangle, param2, pParam3, param4);
    }

    bool trySetMoveLimitCollision(LiveActor* pActor) {
        TVec3f start(pActor->mPosition);
        TVec3f direction(pActor->mGravity);
        start -= direction * 150.0f;
        direction *= 1000.0f;
        if (getCollisionDirector()->getCategoryKeeper(3)->checkStrikeLine(start, direction, 0, nullptr, nullptr)) {
            pActor->mBinder->setExCollisionParts(getCollisionDirector()->getCategoryKeeper(3)->getStrikeInfo(0)->mParentTriangle.mParts);
            return true;
        }

        if (getCollisionDirector()->getCategoryKeeper(0)->checkStrikeLine(start, direction, 0, nullptr, nullptr)) {
            const HitInfo* pHit = getCollisionDirector()->getCategoryKeeper(0)->getStrikeInfo(0);
            CollisionParts* pSameParts = nullptr;
            CollisionParts* pParts = pHit->mParentTriangle.mParts;
            getCollisionDirector()->getCategoryKeeper(3)->searchSameHostParts(&pSameParts, pParts);
            pActor->mBinder->setExCollisionParts(pSameParts);
            return true;
        }

        return false;
    }

    bool isBindedGroundIce(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (!pActor->mBinder->isBindedGround()) {
            return false;
        }

        return isGroundCodeIce(&pActor->mBinder->mGroundInfo.mParentTriangle);
    }

    bool isBindedGroundSand(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (!pActor->mBinder->isBindedGround()) {
            return false;
        }

        return isGroundCodeSand(&pActor->mBinder->mGroundInfo.mParentTriangle);
    }

    bool isBindedGroundDamageFire(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (!pActor->mBinder->isBindedGround()) {
            return false;
        }

        return isGroundCodeDamageFire(&pActor->mBinder->mGroundInfo.mParentTriangle);
    }

    bool isBindedGroundWaterBottomH(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (!pActor->mBinder->isBindedGround()) {
            return false;
        }

        return isGroundCodeWaterBottomH(&pActor->mBinder->mGroundInfo.mParentTriangle);
    }

    bool isBindedGroundWaterBottomM(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (!pActor->mBinder->isBindedGround()) {
            return false;
        }

        return isGroundCodeWaterBottomM(&pActor->mBinder->mGroundInfo.mParentTriangle);
    }

    bool isBindedGroundWater(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (!pActor->mBinder->isBindedGround()) {
            return false;
        }

        return isGroundCodeWaterIter(pActor->mBinder->mGroundInfo.mParentTriangle.getAttributes());
    }

    bool isBindedGroundSinkDeath(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (!pActor->mBinder->isBindedGround()) {
            return false;
        }

        return isGroundCodeSinkDeath(&pActor->mBinder->mGroundInfo.mParentTriangle);
    }

    bool isBindedGroundAreaMove(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (!pActor->mBinder->isBindedGround()) {
            return false;
        }

        return isGroundCodeAreaMove(&pActor->mBinder->mGroundInfo.mParentTriangle);
    }

    bool isBindedGroundRailMove(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (!pActor->mBinder->isBindedGround()) {
            return false;
        }

        return isGroundCodeRailMove(&pActor->mBinder->mGroundInfo.mParentTriangle);
    }

    bool isBindedGroundBrake(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (!pActor->mBinder->isBindedGround()) {
            return false;
        }

        return isGroundCodeBrake(&pActor->mBinder->mGroundInfo.mParentTriangle);
    }

    bool isBindedDamageFire(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->isBindedGround() && isGroundCodeDamageFire(&pActor->mBinder->mGroundInfo.mParentTriangle)) {
            return true;
        }

        if (pActor->mBinder->isBindedWall() && isGroundCodeDamageFire(&pActor->mBinder->mWallInfo.mParentTriangle)) {
            return true;
        }

        if (pActor->mBinder->isBindedRoof() && isGroundCodeDamageFire(&pActor->mBinder->mRoofInfo.mParentTriangle)) {
            return true;
        }

        return false;
    }

    bool isBindedDamageElectric(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->isBindedGround() && isGroundCodeDamageElectric(&pActor->mBinder->mGroundInfo.mParentTriangle)) {
            return true;
        }

        if (pActor->mBinder->isBindedWall() && isGroundCodeDamageElectric(&pActor->mBinder->mWallInfo.mParentTriangle)) {
            return true;
        }

        if (pActor->mBinder->isBindedRoof() && isGroundCodeDamageElectric(&pActor->mBinder->mRoofInfo.mParentTriangle)) {
            return true;
        }

        return false;
    }

    u32 getCameraID(const Triangle* pTriangle) {
        return getCollisionDirector()->mCode->getCameraID(*pTriangle);
    }

    const char* getFloorCodeString(const Triangle* pTriangle) {
        return getCollisionDirector()->mCode->getFloorCodeString(*pTriangle);
    }

    const char* getWallCodeString(const Triangle* pTriangle) {
        return getCollisionDirector()->mCode->getWallCodeString(*pTriangle);
    }

    const char* getSoundCodeString(const Triangle* pTriangle) {
        return getCollisionDirector()->mCode->getSoundCodeString(*pTriangle);
    }

    s32 getFloorCodeIndex(const JMapInfoIter& rIter) {
        return getCollisionDirector()->mCode->getFloorCode(rIter);
    }

    s32 getSoundCodeIndex(const JMapInfoIter& rIter) {
        return getCollisionDirector()->mCode->getSoundCode(rIter);
    }

    s32 getFloorCodeIndex(const Triangle* pTriangle) {
        return getCollisionDirector()->mCode->getFloorCode(pTriangle->getAttributes());
    }

    s32 getWallCodeIndex(const Triangle* pTriangle) {
        return getCollisionDirector()->mCode->getWallCode(pTriangle->getAttributes());
    }

    s32 getSoundCodeIndex(const Triangle* pTriangle) {
        return getCollisionDirector()->mCode->getSoundCode(pTriangle->getAttributes());
    }

    s32 getCameraCodeIndex(const Triangle* pTriangle) {
        return getCollisionDirector()->mCode->getCameraCode(pTriangle->getAttributes());
    }

    bool isGroundCodeWaterIter(const JMapInfoIter& rIter) {
        s32 code = getFloorCodeIndex(rIter);

        return code == CollisionFloorCode_WaterBottomH || code == CollisionFloorCode_WaterBottomM || code == CollisionFloorCode_WaterBottomL ||
               code == CollisionFloorCode_Wet;
    }

    bool isGroundCodeDeath(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_Death;
    }

    bool isGroundCodeDamage(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_DamageNormal;
    }

    bool isGroundCodeIce(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_Ice;
    }

    bool isGroundCodeDamageFire(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_DamageFire;
    }

    bool isGroundCodeFireDance(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_FireDance;
    }

    bool isGroundCodeSand(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_Sand;
    }

    bool isGroundCodeDamageElectric(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_DamageElectric;
    }

    bool isGroundCodeWaterBottomH(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_WaterBottomH;
    }

    bool isGroundCodeWaterBottomM(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_WaterBottomM;
    }

    bool isGroundCodeSinkDeath(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_SinkDeath;
    }

    bool isGroundCodeRailMove(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_RailMove;
    }

    bool isGroundCodeAreaMove(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_AreaMove;
    }

    bool isGroundCodeNoStampSand(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_NoStampSand;
    }

    bool isGroundCodeSinkDeathMud(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_SinkDeathMud;
    }

    bool isGroundCodeBrake(const Triangle* pTriangle) {
        return getFloorCodeIndex(pTriangle) == CollisionFloorCode_Brake;
    }

    bool isWallCodeGhostThrough(const Triangle* pTriangle) {
        return getWallCodeIndex(pTriangle) == CollisionWallCode_GhostThroughCode;
    }

    bool isWallCodeRebound(const Triangle* pTriangle) {
        return getWallCodeIndex(pTriangle) == CollisionWallCode_Rebound;
    }

    bool isWallCodeNoAction(const Triangle* pTriangle) {
        return getWallCodeIndex(pTriangle) == CollisionWallCode_NoAction;
    }

    bool isCameraCodeThrough(const Triangle* pTriangle) {
        return getCameraCodeIndex(pTriangle) == CollisionCameraCode_Through;
    }

    bool isCodeSand(const Triangle* pTriangle) {
        return ((getSoundCodeIndex(pTriangle) == CollisionSoundCode_Sand || getSoundCodeIndex(pTriangle) == CollisionSoundCode_Beach) ||
                isGroundCodeSand(pTriangle)) ||
               isGroundCodeNoStampSand(pTriangle);
    }

    const Triangle* getCameraPolyFast(const TVec3f& rStart, const TVec3f& rDirection, const HitSensor* pSensor) {
        Triangle triangle;
        f32 remaining = rDirection.length();
        TVec3f direction(rDirection);
        direction.normalize();
        TVec3f start(rStart);
        TVec3f step(direction * 5000.0f);
        do {
            f32 length = 5000.0f;
            if (remaining < length) {
                length = remaining;
                step = direction * remaining;
            }

            s32 count = getNearPolyOnLineSort(start, start, step, pSensor);
            while (count != 0) {
                return getSortedPoly(0);
            }

            remaining -= length;
            start += step;
        } while (!isNearZero(remaining));

        return nullptr;
    }

    bool getFirstPolyOnLineBFast(const TVec3f& rStart, const TVec3f& rDirection, TVec3f* pPos, Triangle* pTriangle) {
        Triangle triangle;
        f32 remaining = rDirection.length();
        TVec3f direction(rDirection);
        direction.normalize();
        TVec3f start(rStart);
        TVec3f step(direction * 5000.0f);
        do {
            f32 length = 5000.0f;
            if (remaining < length) {
                length = remaining;
                step = direction * remaining;
            }

            u32 count = getNearPolyOnLineSort(start, start, step, nullptr);
            for (u32 i = 0; i < count; i++) {
                Triangle hitTriangle;
                TVec3f hitPos;
                if (getSortedPoly(&hitPos, &hitTriangle, i)) {
                    if (isWaterPolygon(&hitTriangle)) {
                        continue;
                    }

                    TVec3f backPos(hitPos - *getNormal(&hitTriangle) * 5.0f);
                    TVec3f normal(*getNormal(&hitTriangle) * 35.0f);
                    if (isExistMapCollision(backPos, normal)) {
                        continue;
                    }
                }

                if (pPos) {
                    *pPos = hitPos;
                }

                if (pTriangle) {
                    *pTriangle = hitTriangle;
                }

                return true;
            }

            remaining -= length;
            start += step;
        } while (!isNearZero(remaining));

        return false;
    }
};  // namespace MR

namespace Collision {
    s32 checkStrikePointToMap(const TVec3f& rPoint, HitInfo* pHit) {
        return MR::getCollisionDirector()->getCategoryKeeper(0)->checkStrikePoint(rPoint, pHit);
    }

    s32 checkStrikeBallToMap(const TVec3f& rCenter, f32 radius, const CollisionPartsFilterBase* pPartsFilter,
                             const TriangleFilterBase* pTriangleFilter) {
        return MR::getCollisionDirector()->getCategoryKeeper(0)->checkStrikeBall(rCenter, radius, false, pPartsFilter, pTriangleFilter);
    }

    s32 checkStrikeBallToMapWithMovingReaction(const TVec3f& rCenter, f32 radius, const CollisionPartsFilterBase* pPartsFilter,
                                               const TriangleFilterBase* pTriangleFilter) {
        return MR::getCollisionDirector()->getCategoryKeeper(0)->checkStrikeBall(rCenter, radius, true, pPartsFilter, pTriangleFilter);
    }

    s32 checkStrikeBallToMapWithThickness(const TVec3f& rCenter, f32 radius, f32 thickness, const CollisionPartsFilterBase* pPartsFilter,
                                          const TriangleFilterBase* pTriangleFilter) {
        return MR::getCollisionDirector()->getCategoryKeeper(0)->checkStrikeBallWithThickness(rCenter, radius, thickness, pPartsFilter,
                                                                                              pTriangleFilter);
    }

    s32 checkStrikeLineToMap(const TVec3f& rStart, const TVec3f& rDirection, s32 count, const CollisionPartsFilterBase* pPartsFilter,
                             const TriangleFilterBase* pTriangleFilter) {
        return MR::getCollisionDirector()->getCategoryKeeper(0)->checkStrikeLine(rStart, rDirection, count, pPartsFilter, pTriangleFilter);
    }

    s32 checkStrikeLineToSunshade(const TVec3f& rStart, const TVec3f& rDirection, s32 count, const CollisionPartsFilterBase* pPartsFilter,
                                  const TriangleFilterBase* pTriangleFilter) {
        return MR::getCollisionDirector()->getCategoryKeeper(1)->checkStrikeLine(rStart, rDirection, count, pPartsFilter, pTriangleFilter);
    }

    const HitInfo* getStrikeInfoMap(u32 index) {
        return MR::getCollisionDirector()->getCategoryKeeper(0)->getStrikeInfo(index);
    }

    u32 getStrikeInfoNumMap() {
        return ::getStrikeInfoNumCategory(0);
    }
}  // namespace Collision
