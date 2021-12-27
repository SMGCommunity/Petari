#pragma once

#include "Game/Gravity/PlanetGravity.h"

class CubeGravity : public PlanetGravity {
public:
	CubeGravity();

	virtual void updateMtx(const TPos3f &rMtx);
	virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

	void setCube(const TPos3f &);
	void calcGravityArea(const TVec3f &rPosition) const;
	void calcFaceGravity(const TVec3f &rPosition, s32, TVec3f *pDest, f32 *pScalar) const;
	void calcEdgeGravity(const TVec3f &rPosition, s32, TVec3f *pDest, f32 *pScalar) const;
	void calcCornerGravity(const TVec3f &rPosition, s32, TVec3f *pDest, f32 *pScalar) const;

	TPos3f _28;
	TPos3f _58;
	f32 _88;
	f32 _8C;
	f32 _90;
	u8 mActiveFaces; // _94
};
