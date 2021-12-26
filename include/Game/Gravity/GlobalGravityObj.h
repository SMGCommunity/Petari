#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Gravity/GravityCreator.h"

class GlobalGravityObj : public LiveActor {
public:
	GlobalGravityObj(const char *pName);

	virtual ~GlobalGravityObj() {}
	virtual void init(const JMapInfoIter &rIter);
	virtual void movement();
	virtual void makeActorAppeared();
	virtual void makeActorDead();

	void updateSwitch();
	PlanetGravity* getGravity();

	GravityCreator* mGravityCreator; // _8C
};

namespace MR {
	GlobalGravityObj* createGlobalCubeGravityObj(const char *pName);
	GlobalGravityObj* createGlobalConeGravityObj(const char *pName);
	GlobalGravityObj* createGlobalDiskGravityObj(const char *pName);
	GlobalGravityObj* createGlobalDiskTorusGravityObj(const char *pName);
	GlobalGravityObj* createGlobalPlaneGravityObj(const char *pName);
	GlobalGravityObj* createGlobalPlaneInBoxGravityObj(const char *pName);
	GlobalGravityObj* createGlobalPlaneInCylinderGravityObj(const char *pName);
	GlobalGravityObj* createGlobalPointGravityObj(const char *pName);
	GlobalGravityObj* createGlobalSegmentGravityObj(const char *pName);
	GlobalGravityObj* createGlobalWireGravityObj(const char *pName);
};
