#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class CubeGravity : public PlanetGravity {
public:
	CubeGravity();

	virtual void updateMtx(const TPos3f &rMtx);
	virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

	void setCube(const TPos3f &);
	int calcGravityArea(const TVec3f &rPosition) const;
	bool calcFaceGravity(const TVec3f &rPosition, s32, TVec3f *pDest, f32 *pScalar) const;
	bool calcEdgeGravity(const TVec3f &rPosition, s32, TVec3f *pDest, f32 *pScalar) const;
	bool calcCornerGravity(const TVec3f &rPosition, s32, TVec3f *pDest, f32 *pScalar) const;
	

/*inline void init(TVec3f& x, TVec3f& y, TVec3f& z) const {
	mPosition.getXDir(x);
	mPosition.getYDir(y);
	mPosition.getZDir(z);
}*/

	TPos3f mCube; // _28
	TPos3f mPosition; // _58
	f32 lenX; // _88
	f32 lenY; // _8C
	f32 lenZ; // _90
	u8 mActiveFaces; // _94
};
