#pragma once

#include "JSystem/JGeometry.h"
#include "Game/Gravity/PlanetGravity.h"
#include "Game/Gravity/PointGravity.h"
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

class PointGravityCreator : public GravityCreator {
public:
	inline PointGravityCreator() : GravityCreator(), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale);

	PointGravity* mGravityInstance; // _4
};

class WireGravityCreator : public GravityCreator {
public:
	inline WireGravityCreator() : GravityCreator(), mRailRider(NULL), mGravityInstance(NULL) {}

	virtual PlanetGravity* getGravity();
	virtual PlanetGravity* createInstance();
	virtual void settingFromJMapOtherParam(const JMapInfoIter &rIter);

	RailRider* mRailRider; // _4
	WireGravity* mGravityInstance; // _8
};
