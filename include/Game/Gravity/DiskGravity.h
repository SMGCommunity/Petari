#pragma once

#include "Game/Gravity/PlanetGravity.h"

class DiskGravity : public PlanetGravity {
public:
	DiskGravity();

	virtual void updateMtx(const TPos3f &rMtx);
	virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

	void setLocalPosition(const TVec3f &rLocalPos);
	void setLocalDirection(const TVec3f &rLocalDir);
	void setSideDirection(const TVec3f &rSideDir);
	void setRadius(f32 val);
	void setValidDegee(f32 val);
	void setBothSide(bool val);
	void setEnableEdgeGravity(bool val);
	void updateLocalParam();

	TVec3f mLocalPosition;   // _28
	TVec3f mTranslation;     // _34
	TVec3f mLocalDirection;  // _40
	TVec3f mRotation;        // _4C
	TVec3f mSideDirection;   // _58
	TVec3f mSideVecOrtho;    // _64
	TVec3f mWorldSideDir;    // _70
	f32 mRadius;             // _7C
	f32 mWorldRadius;        // _80
	f32 mValidDegree;        // _84
	f32 mValidCos;           // _88
	bool mEnableBothSide;    // _8C
	bool mEnableEdgeGravity; // _8D
};
