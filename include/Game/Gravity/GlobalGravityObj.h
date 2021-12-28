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
	NameObj* createGlobalCubeGravityObj(const char *pName);
	NameObj* createGlobalConeGravityObj(const char *pName);
	NameObj* createGlobalDiskGravityObj(const char *pName);
	NameObj* createGlobalDiskTorusGravityObj(const char *pName);
	NameObj* createGlobalPlaneGravityObj(const char *pName);
	NameObj* createGlobalPlaneInBoxGravityObj(const char *pName);
	NameObj* createGlobalPlaneInCylinderGravityObj(const char *pName);
	NameObj* createGlobalPointGravityObj(const char *pName);
	NameObj* createGlobalSegmentGravityObj(const char *pName);
	NameObj* createGlobalWireGravityObj(const char *pName);
};
