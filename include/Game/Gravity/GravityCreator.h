#pragma once

#include "Game/Gravity/ConeGravity.h"
#include "Game/Gravity/CubeGravity.h"
#include "Game/Gravity/DiskGravity.h"
#include "Game/Gravity/DiskTorusGravity.h"
#include "Game/Gravity/ParallelGravity.h"
#include "Game/Gravity/PointGravity.h"
#include "Game/Gravity/SegmentGravity.h"
#include "Game/Gravity/WireGravity.h"
#include "Game/LiveActor/RailRider.h"
#include "Game/Util/JMapInfo.h"

class GravityCreator {
public:
	GravityCreator();

	virtual PlanetGravity* getGravity() = 0;
	virtual PlanetGravity* createInstance() = 0;
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {}
	virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2) {}
	virtual void settingFromJMapOtherParam(const JMapInfoIter &rIter) {}

	PlanetGravity* createFromJMap(const JMapInfoIter &rIter);
};

class CubeGravityCreator : public GravityCreator {
public:
	inline CubeGravityCreator() : GravityCreator(), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale);
	virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

	CubeGravity* mGravityInstance; // _4
};

class DiskGravityCreator : public GravityCreator {
public:
	inline DiskGravityCreator() : GravityCreator(), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale);
	virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

	DiskGravity* mGravityInstance; // _4
};

class DiskTorusGravityCreator : public GravityCreator {
public:
	inline DiskTorusGravityCreator() : GravityCreator(), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale);
	virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

	DiskTorusGravity* mGravityInstance; // _4
};

class ConeGravityCreator : public GravityCreator {
public:
	inline ConeGravityCreator() : GravityCreator(), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale);
	virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

	ConeGravity* mGravityInstance; // _4
};

class PlaneGravityCreator : public GravityCreator {
public:
	inline PlaneGravityCreator() : GravityCreator(), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale);

	ParallelGravity* mGravityInstance; // _4
};

class PlaneInBoxGravityCreator : public GravityCreator {
public:
	inline PlaneInBoxGravityCreator() : GravityCreator(), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale);
	virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

	ParallelGravity* mGravityInstance; // _4
};

class PlaneInCylinderGravityCreator : public GravityCreator {
public:
	inline PlaneInCylinderGravityCreator() : GravityCreator(), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale);
	virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

	ParallelGravity* mGravityInstance; // _4
};

class PointGravityCreator : public GravityCreator {
public:
	inline PointGravityCreator() : GravityCreator(), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale);

	PointGravity* mGravityInstance; // _4
};

class SegmentGravityCreator : public GravityCreator {
public:
	inline SegmentGravityCreator() : GravityCreator(), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale);
	virtual void settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2);

	SegmentGravity* mGravityInstance; // _4
};

class WireGravityCreator : public GravityCreator {
public:
	inline WireGravityCreator() : GravityCreator(), mRailRider(NULL), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromJMapOtherParam(const JMapInfoIter &rIter);

	RailRider* mRailRider;         // _4
	WireGravity* mGravityInstance; // _8
};
