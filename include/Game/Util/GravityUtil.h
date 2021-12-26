#pragma once

#include "JSystem/JGeometry.h"

class JMapInfoIter;
class PlanetGravity;
class GravityInfo;
class NameObj;
class LiveActor;
class NameObj;

namespace {
	void getJMapInfoArgPlus(const JMapInfoIter &rIter, const char *pFieldName, f32 *pDest) NO_INLINE;
	void getJMapInfoArgPlus(const JMapInfoIter &rIter, const char *pFieldName, s32 *pDest) NO_INLINE;
	bool calcGravityVectorOrZero(const NameObj *pActor, const TVec3f &rPosition, u32 gravityType, TVec3f * pDest, GravityInfo *pInfo, u32 unk) NO_INLINE;
}

namespace MR {
	void registerGravity(PlanetGravity *pGravity);

	bool calcGravityVector(const LiveActor *pActor, TVec3f *pDest, GravityInfo *rInfo, u32 unk);
	bool calcGravityVector(const NameObj *pActor, const TVec3f &rPosition, TVec3f *pDest, GravityInfo *rInfo, u32 unk);
	bool calcDropShadowVector(const LiveActor *pActor, TVec3f *pDest, GravityInfo *rInfo, u32 unk);
	bool calcDropShadowVector(const NameObj *pActor, const TVec3f &rPosition, TVec3f *pDest, GravityInfo *rInfo, u32 unk);
	bool calcGravityAndDropShadowVector(const LiveActor *pActor, TVec3f *pDest, GravityInfo *rInfo, u32 unk);
	bool calcGravityAndMagnetVector(const LiveActor *pActor, TVec3f *pDest, GravityInfo *rInfo, u32 unk);
	bool calcGravityVectorOrZero(const LiveActor *pActor, TVec3f *pDest, GravityInfo *rInfo, u32 unk);
	bool calcGravityVectorOrZero(const NameObj *pActor, const TVec3f &rPosition, TVec3f *pDest, GravityInfo *rInfo, u32 unk);
	bool calcDropShadowVectorOrZero(const NameObj *pActor, const TVec3f &rPosition, TVec3f *pDest, GravityInfo *rInfo, u32 unk);
	bool calcGravityAndDropShadowVectorOrZero(const LiveActor *pActor, TVec3f *pDest, GravityInfo *rInfo, u32 unk);
	bool calcAttractMarioLauncherOrZero(const LiveActor *pActor, TVec3f *pDest, GravityInfo *rInfo, u32 unk);

	bool isZeroGravity(const LiveActor *pActor);
	bool isLightGravity(const GravityInfo &rInfo);

	void settingGravityParamFromJMap(PlanetGravity *pGravity, const JMapInfoIter &rIter);
	void getJMapInfoGravityType(const JMapInfoIter &rIter, PlanetGravity *pGravity);
	void getJMapInfoGravityPower(const JMapInfoIter &rIter, PlanetGravity *pGravity);
};
