#include "Game/Util/MapUtil.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/CollisionCategorizedKeeper.hpp"
#include "Game/Map/CollisionCode.hpp"
#include "Game/Map/CollisionDirector.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/MathUtil.hpp"

static HitInfo mSortBuffer[32];
static u32 mSortCount;

namespace {
    // getStrikeInfoNumCategory
    // getFirstPolyOnLineCategory
    // getFirstPolyOnLineCategoryExceptSensor
    // getFirstPolyOnLineCategoryExceptActor
};

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
        return __fabsf(param1) < 0.34202015f;
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

    // getFirstPolyOnLineToMap
    // getFirstPolyOnLineToMapAndMoveLimit
    // getFirstPolyOnLineToWaterSurface
    // getFirstPolyOnLineToMapExceptSensor
    // getFirstPolyOnLineToMapExceptActor
    // getFirstPolyOnLineToMap
    // getFirstPolyOnLineToWaterSurface
    // getFirstPolyNormalOnLineToMap
    // getNearPolyOnLineSort

    bool getSortedPoly(TVec3f* pDst, Triangle* pTriangle, u32 sortIndex) {
        if (mSortCount <= sortIndex) {
            return false;
        }

        if (pTriangle != nullptr) {
            *pTriangle = mSortBuffer[sortIndex].mParentTriangle;
        }

        if (pDst != nullptr) {
            *pDst = mSortBuffer[sortIndex].mHitPos;
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
        return getCollisionDirector()->mKeepers[0]->checkStrikeLine(rParam1, rParam2, 1, nullptr, nullptr) != 0;
    }

    bool isExistMoveLimitCollision(const TVec3f& rParam1, const TVec3f& rParam2) {
        return getCollisionDirector()->mKeepers[3]->checkStrikeLine(rParam1, rParam2, 1, nullptr, nullptr) != 0;
    }

    // isExistMapCollisionExceptActor

    bool checkStrikePointToMap(const TVec3f& rParam1, HitInfo* pParam2) {
        return getCollisionDirector()->mKeepers[0]->checkStrikePoint(rParam1, pParam2) != 0;
    }

    bool checkStrikeBallToMap(const TVec3f& rParam1, f32 param2) {
        return getCollisionDirector()->mKeepers[0]->checkStrikeBall(rParam1, param2, false, nullptr, nullptr) != 0;
    }

    // calcMapGround
    // calcMapGroundUpper

    bool isFallNextMove(const LiveActor* pActor, f32 param2, f32 param3, f32 param4, const TriangleFilterBase* pParam5) {
        return isFallNextMove(pActor->mPosition, pActor->mVelocity, pActor->mGravity, param2, param3, param4, pParam5);
    }

    // isFallNextMove
    // isFallOrDangerNextMove
    // isFallOrDangerNextMove
    // calcVelocityMovingPoint

    u32 createAreaPolygonList(Triangle* pTriangle, u32 param2, const TVec3f& rParam3, const TVec3f& rParam4) {
        return getCollisionDirector()->mKeepers[0]->createAreaPolygonList(pTriangle, param2, rParam3, rParam4);
    }

    u32 createAreaPolygonListArray(Triangle* pTriangle, u32 param2, TVec3f* pParam3, u32 param4) {
        return getCollisionDirector()->mKeepers[0]->createAreaPolygonListArray(pTriangle, param2, pParam3, param4);
    }

    // trySetMoveLimitCollision

    bool isBindedGroundIce(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f) {
            return isGroundCodeIce(&pActor->mBinder->mGroundInfo.mParentTriangle);
        }

        return false;
    }

    bool isBindedGroundSand(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f) {
            return isGroundCodeSand(&pActor->mBinder->mGroundInfo.mParentTriangle);
        }

        return false;
    }

    bool isBindedGroundDamageFire(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f) {
            return isGroundCodeDamageFire(&pActor->mBinder->mGroundInfo.mParentTriangle);
        }

        return false;
    }

    bool isBindedGroundWaterBottomH(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f) {
            return isGroundCodeWaterBottomH(&pActor->mBinder->mGroundInfo.mParentTriangle);
        }

        return false;
    }

    bool isBindedGroundWaterBottomM(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f) {
            return isGroundCodeWaterBottomM(&pActor->mBinder->mGroundInfo.mParentTriangle);
        }

        return false;
    }

    bool isBindedGroundWater(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f) {
            return isGroundCodeWaterIter(pActor->mBinder->mGroundInfo.mParentTriangle.getAttributes());
        }

        return false;
    }

    bool isBindedGroundSinkDeath(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f) {
            return isGroundCodeSinkDeath(&pActor->mBinder->mGroundInfo.mParentTriangle);
        }

        return false;
    }

    bool isBindedGroundAreaMove(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f) {
            return isGroundCodeAreaMove(&pActor->mBinder->mGroundInfo.mParentTriangle);
        }

        return false;
    }

    bool isBindedGroundRailMove(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f) {
            return isGroundCodeRailMove(&pActor->mBinder->mGroundInfo.mParentTriangle);
        }

        return false;
    }

    bool isBindedGroundBrake(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f) {
            return isGroundCodeBrake(&pActor->mBinder->mGroundInfo.mParentTriangle);
        }

        return false;
    }

    bool isBindedDamageFire(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f || isGroundCodeDamageFire(&pActor->mBinder->mGroundInfo.mParentTriangle)) {
            return true;
        }

        if (pActor->mBinder->_158 <= 0.0f || isGroundCodeDamageFire(&pActor->mBinder->mWallInfo.mParentTriangle)) {
            return true;
        }

        if (pActor->mBinder->_1E8 <= 0.0f || isGroundCodeDamageFire(&pActor->mBinder->mRoofInfo.mParentTriangle)) {
            return true;
        }

        return false;
    }

    bool isBindedDamageElectric(const LiveActor* pActor) {
        if (pActor->mBinder == nullptr) {
            return false;
        }

        if (pActor->mBinder->_C8 <= 0.0f || isGroundCodeDamageElectric(&pActor->mBinder->mGroundInfo.mParentTriangle)) {
            return true;
        }

        if (pActor->mBinder->_158 <= 0.0f || isGroundCodeDamageElectric(&pActor->mBinder->mWallInfo.mParentTriangle)) {
            return true;
        }

        if (pActor->mBinder->_1E8 <= 0.0f || isGroundCodeDamageElectric(&pActor->mBinder->mRoofInfo.mParentTriangle)) {
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

    bool isSoundCodeSand(const Triangle* pTriangle) {
        return getSoundCodeIndex(pTriangle) == CollisionSoundCode_Sand;
    }

    bool isCameraCodeThrough(const Triangle* pTriangle) {
        return getCameraCodeIndex(pTriangle) == CollisionCameraCode_Through;
    }

    bool isCodeSand(const Triangle* pTriangle) {
        return isSoundCodeSand(pTriangle) || isGroundCodeSand(pTriangle) || isGroundCodeNoStampSand(pTriangle);
    }

    // getCameraPolyFast
    // getFirstPolyOnLineBFast
};  // namespace MR
