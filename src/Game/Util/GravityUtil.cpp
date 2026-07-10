#include "Game/Util/GravityUtil.hpp"
#include "Game/Gravity/GravityInfo.hpp"
#include "Game/Gravity/PlanetGravity.hpp"
#include "Game/Gravity/PlanetGravityManager.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include <cstring>

namespace {
    PlanetGravityManager* getGravityManager() {
        return MR::getSceneObj< PlanetGravityManager >(SceneObj_PlanetGravityManager);
    }

    void getJMapInfoArgPlus(const JMapInfoIter& rIter, const char* pFieldName, f32* pDest) {
        f32 result;

        if (rIter.getValue(pFieldName, &result) && result >= 0.0f) {
            *pDest = result;
        }
    }

    void getJMapInfoArgPlus(const JMapInfoIter& rIter, const char* pFieldName, s32* pDest) {
        s32 result;

        if (rIter.getValue(pFieldName, &result) && result >= 0.0f) {
            *pDest = result;
        }
    }

    bool calcGravityVectorOrZero(const NameObj* pActor, const TVec3f& rPosition, u32 typeFlags, TVec3f* pDest, GravityInfo* pInfo, u32 host) NO_INLINE {
        if (host == 0) {
            host = (u32)pActor;
        }

        return getGravityManager()->calcTotalGravityVector(pDest, pInfo, rPosition, typeFlags, host);
    }
};  // namespace

namespace MR {
    void registerGravity(PlanetGravity* pGravity) {
        ::getGravityManager()->registerGravity(pGravity);
    }

    bool calcGravityVector(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_NORMAL;
        return ::calcGravityVectorOrZero(pActor, pActor->mPosition, typeFlags, pDest, pInfo, host);
    }

    bool calcGravityVector(const NameObj* pActor, const TVec3f& rPosition, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_NORMAL;
        return ::calcGravityVectorOrZero(pActor, rPosition, typeFlags, pDest, pInfo, host);
    }

    bool calcDropShadowVector(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_SHADOW;
        return ::calcGravityVectorOrZero(pActor, pActor->mPosition, typeFlags, pDest, pInfo, host);
    }

    bool calcDropShadowVector(const NameObj* pActor, const TVec3f& rPosition, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_SHADOW;
        return ::calcGravityVectorOrZero(pActor, rPosition, typeFlags, pDest, pInfo, host);
    }

    bool calcGravityAndDropShadowVector(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_NORMAL | GRAVITY_TYPE_SHADOW;
        return ::calcGravityVectorOrZero(pActor, pActor->mPosition, typeFlags, pDest, pInfo, host);
    }

    bool calcGravityAndMagnetVector(const NameObj* pActor, const TVec3f& rPosition, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_NORMAL | GRAVITY_TYPE_MAGNET;
        return ::calcGravityVectorOrZero(pActor, rPosition, typeFlags, pDest, pInfo, host);
    }

    bool calcGravityVectorOrZero(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_NORMAL;
        return ::calcGravityVectorOrZero(pActor, pActor->mPosition, typeFlags, pDest, pInfo, host);
    }

    bool calcGravityVectorOrZero(const NameObj* pActor, const TVec3f& rPosition, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_NORMAL;
        return ::calcGravityVectorOrZero(pActor, rPosition, typeFlags, pDest, pInfo, host);
    }

    bool calcDropShadowVectorOrZero(const NameObj* pActor, const TVec3f& rPosition, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_SHADOW;
        return ::calcGravityVectorOrZero(pActor, rPosition, typeFlags, pDest, pInfo, host);
    }

    bool calcGravityAndDropShadowVectorOrZero(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_NORMAL | GRAVITY_TYPE_SHADOW;
        return ::calcGravityVectorOrZero(pActor, pActor->mPosition, typeFlags, pDest, pInfo, host);
    }

    bool calcAttractMarioLauncherOrZero(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host) {
        u32 typeFlags = GRAVITY_TYPE_MARIO_LAUNCHER;
        return ::calcGravityVectorOrZero(pActor, pActor->mPosition, typeFlags, pDest, pInfo, host);
    }

    bool isZeroGravity(const LiveActor* pActor) {
        TVec3f dummyGravity;
        return !::calcGravityVectorOrZero(pActor, pActor->mPosition, GRAVITY_TYPE_NORMAL, &dummyGravity, nullptr, 0);
    }

    bool isLightGravity(const GravityInfo& rInfo) {
        PlanetGravity* pGravity = rInfo.mGravityInstance;

        if (pGravity == nullptr) {
            return false;
        }

        return pGravity->mGravityPower == GRAVITY_POWER_LIGHT;
    }

    void settingGravityParamFromJMap(PlanetGravity* pGravity, const JMapInfoIter& rIter) {
        f32 range = pGravity->mRange;
        ::getJMapInfoArgPlus(rIter, "Range", &range);
        pGravity->mRange = range;

        f32 distant = pGravity->mDistant;
        ::getJMapInfoArgPlus(rIter, "Distant", &distant);
        pGravity->mDistant = distant;

        s32 priority = pGravity->mPriority;
        ::getJMapInfoArgPlus(rIter, "Priority", &priority);
        pGravity->setPriority(priority);

        s32 id = pGravity->mGravityId;
        ::getJMapInfoArgPlus(rIter, "Gravity_id", &id);
        pGravity->mGravityId = id;

        getJMapInfoGravityType(rIter, pGravity);
        getJMapInfoGravityPower(rIter, pGravity);

        s32 inverse = pGravity->mIsInverse != false;
        ::getJMapInfoArgPlus(rIter, "Inverse", &inverse);
        pGravity->mIsInverse = inverse;
    }

    void getJMapInfoGravityType(const JMapInfoIter& rIter, PlanetGravity* pGravity) {
        const char* pType = nullptr;

        if (rIter.getValue("Gravity_type", &pType)) {
            if (strcmp(pType, "Normal") == 0) {
                pGravity->mGravityType = GRAVITY_TYPE_NORMAL;
            } else if (strcmp(pType, "Shadow") == 0) {
                pGravity->mGravityType = GRAVITY_TYPE_SHADOW;
            } else if (strcmp(pType, "Magnet") == 0) {
                pGravity->mGravityType = GRAVITY_TYPE_MAGNET;
            }
        }
    }

    void getJMapInfoGravityPower(const JMapInfoIter& rIter, PlanetGravity* pGravity) {
        const char* pPower = nullptr;

        if (rIter.getValue("Power", &pPower)) {
            if (strcmp(pPower, "Light") == 0) {
                pGravity->mGravityPower = GRAVITY_POWER_LIGHT;
            } else if (strcmp(pPower, "Normal") == 0) {
                pGravity->mGravityPower = GRAVITY_POWER_NORMAL;
            } else if (strcmp(pPower, "Heavy") == 0) {
                pGravity->mGravityPower = GRAVITY_POWER_HEAVY;
            }
        }
    }
};  // namespace MR
