#include "Game/Gravity.hpp"
#include "Game/Util.hpp"

GlobalGravityObj::GlobalGravityObj(const char *pName) : LiveActor(pName) {
	mGravityCreator = nullptr;
}

void GlobalGravityObj::init(const JMapInfoIter &rIter) {
	MR::initDefaultPos(this, rIter);
	mGravityCreator->createFromJMap(rIter);

	MR::useStageSwitchReadA(this, rIter);
	MR::useStageSwitchReadB(this, rIter);

	if (MR::isValidSwitchA(this) || MR::isValidSwitchB(this)) {
		MR::connectToSceneMapObjMovement(this);
	}

	MR::invalidateClipping(this);
	MR::registerDemoSimpleCastAll(this);
	MR::addBaseMatrixFollowerGravity(this, rIter);

	makeActorAppeared();
	MR::useStageSwitchSleep(this, rIter);
}

void GlobalGravityObj::makeActorAppeared() {
	LiveActor::makeActorAppeared();
	PlanetGravity* pGravity = mGravityCreator->getGravity();
	pGravity->mAppeared = true;
	updateSwitch();
}

void GlobalGravityObj::makeActorDead() {
	LiveActor::makeActorDead();
	PlanetGravity* pGravity = mGravityCreator->getGravity();
	pGravity->mAppeared = false;
}

void GlobalGravityObj::updateSwitch() {
	if (MR::isValidSwitchA(this) || MR::isValidSwitchB(this)) {
		bool switchA = true;
		bool switchB = true;

		if (MR::isValidSwitchA(this)) {
			switchA = MR::isOnSwitchA(this);
		}
		if (MR::isValidSwitchB(this)) {
			switchB = MR::isOnSwitchB(this) == false;
		}

		PlanetGravity* pGravity = mGravityCreator->getGravity();
		pGravity->mActivated = switchA && switchB;
	}
}

void GlobalGravityObj::movement() {
	updateSwitch();
}

PlanetGravity* GlobalGravityObj::getGravity() {
	return mGravityCreator->getGravity();
}

namespace MR {
	NameObj* createGlobalCubeGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new CubeGravityCreator();
		return ret;
	}

	NameObj* createGlobalConeGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new ConeGravityCreator();
		return ret;
	}

	NameObj* createGlobalDiskGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new DiskGravityCreator();
		return ret;
	}

	NameObj* createGlobalDiskTorusGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new DiskTorusGravityCreator();
		return ret;
	}

	NameObj* createGlobalPlaneGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new PlaneGravityCreator();
		return ret;
	}

	NameObj* createGlobalPlaneInBoxGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new PlaneInBoxGravityCreator();
		return ret;
	}

	NameObj* createGlobalPlaneInCylinderGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new PlaneInCylinderGravityCreator();
		return ret;
	}

	NameObj* createGlobalPointGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new PointGravityCreator();
		return ret;
	}

	NameObj* createGlobalSegmentGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new SegmentGravityCreator();
		return ret;
	}

	NameObj* createGlobalWireGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new WireGravityCreator();
		return ret;
	}
}
