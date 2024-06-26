#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class ConeGravity : public PlanetGravity {
public:
	ConeGravity();

	virtual void updateMtx(const TPos3f &rMtx);
	virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

	void setLocalMatrix(const TPos3f &rMtx);
	void setEnableBottom(bool val);
	void setTopCutRate(f32 val);

    /*
     * The x column represents the radius vector. The y column represents the central axis vector,
     * point from the center of the base of the cone to the apex. The translation column represents
     * the location of the center of the base of the cone, and the z column is unused.
     */
	TPos3f mLocalMtx;   // _28
	TPos3f mWorldMtx;   // _58
    
	f32 mValidDegree;   // _88; set in ctor but never used
	f32 mValidCos;      // _8C; set in ctor but never used
	f32 mWorldRadius;   // _90
	bool mEnableBottom; // _94
	f32 mTopCutRate;    // _98
};
