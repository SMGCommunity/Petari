#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class GravityInfo;
class JMapInfoIter;
class LiveActor;
class NameObj;
class PlanetGravity;

namespace MR {
    void registerGravity(PlanetGravity* pGravity);
    bool calcGravityVector(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool calcGravityVector(const NameObj* pObj, const TVec3f& rPosition, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool calcDropShadowVector(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool calcDropShadowVector(const NameObj* pObj, const TVec3f& rPosition, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool calcGravityAndDropShadowVector(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool calcGravityAndMagnetVector(const NameObj* pObj, const TVec3f& rPosition, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool calcGravityVectorOrZero(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool calcGravityVectorOrZero(const NameObj* pObj, const TVec3f& rPosition, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool calcDropShadowVectorOrZero(const NameObj* pObj, const TVec3f& rPosition, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool calcGravityAndDropShadowVectorOrZero(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool calcAttractMarioLauncherOrZero(const LiveActor* pActor, TVec3f* pDest, GravityInfo* pInfo, u32 host);
    bool isZeroGravity(const LiveActor* pActor);
    bool isLightGravity(const GravityInfo& rInfo);
    void settingGravityParamFromJMap(PlanetGravity* pGravity, const JMapInfoIter& rIter);
    void getJMapInfoGravityType(const JMapInfoIter& rIter, PlanetGravity* pGravity);
    void getJMapInfoGravityPower(const JMapInfoIter& rIter, PlanetGravity* pGravity);
};  // namespace MR
