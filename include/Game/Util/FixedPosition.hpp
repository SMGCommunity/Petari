#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include <revolution.h>

class LiveActor;

class FixedPosition {
public:
    FixedPosition(const LiveActor *, const char *, const TVec3f &, const TVec3f &);
	FixedPosition(const LiveActor *, const TVec3f &, const TVec3f &);
	FixedPosition(MtxPtr, const TVec3f &, const TVec3f &);
	FixedPosition(const LiveActor *, const char *, const LiveActor *);

	void init(MtxPtr, const TVec3f &, const TVec3f &);
	void calc();
	void setBaseMtx(MtxPtr);
	void setLocalTrans(const TVec3f &);
	void copyTrans(TVec3f *) const;
	void copyRotate(TVec3f *) const;

	MtxPtr mBaseMtx; // _0
	TVec3f mLocalTrans; // _4
	TVec3f _10;
	TMtx34f _1C;
	bool mNormalizeScale; // _4C
};