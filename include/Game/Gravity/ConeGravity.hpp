#pragma once

#include "Game/Gravity/PlanetGravity.h"

class ConeGravity : public PlanetGravity {
public:
	ConeGravity();

	virtual void updateMtx(const TPos3f &rMtx);
	virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

	void setLocalMatrix(const TPos3f &rMtx);
	void setEnableBottom(bool val);
	void setTopCutRate(f32 val);

	TPos3f mLocalMtx;   // _28
	TPos3f mWorldMtx;   // _58
	f32 _88;            // set in ctor but never used
	f32 _8C;            // set in ctor but never used
	f32 _90;
	bool mEnableBottom; // _94
	f32 mTopCutRate;    // _98
};
