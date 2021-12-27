#include "Game/Gravity.h"
#include "Game/Util.h"

GlobalGravityObj::GlobalGravityObj(const char *pName) : LiveActor(pName) {
	mGravityCreator = NULL;
}

void GlobalGravityObj::init(const JMapInfoIter &rIter) {
	MR::initDefaultPos(this, rIter);
	mGravityCreator->createFromJMap(rIter);

	MR::useStageSwitchReadA(this, rIter);
	MR::useStageSwitchReadB(this, rIter);

	if (MR::isValidSwitchA(this) || MR::isValidSwitchB(this))
		MR::connectToSceneMapObjMovement(this);

	MR::invalidateClipping(this);
	MR::registerDemoSimpleCastAll(this);
	MR::addBaseMatrixFollowerGravity(this, rIter);

	makeActorAppeared();
	MR::useStageSwitchSleep(this, rIter);
}

void GlobalGravityObj::makeActorAppeared() {
	LiveActor::makeActorAppeared();
	PlanetGravity* pGravity = mGravityCreator->getGravity();
	pGravity->_24 = true;
	updateSwitch();
}

void GlobalGravityObj::makeActorDead() {
	LiveActor::makeActorDead();
	PlanetGravity* pGravity = mGravityCreator->getGravity();
	pGravity->_24 = false;
}

void GlobalGravityObj::updateSwitch() {
	if (MR::isValidSwitchA(this) || MR::isValidSwitchB(this)) {
		bool switchA = true;
		bool switchB = true;

		if (MR::isValidSwitchA(this))
			switchA = MR::isOnSwitchA(this);
		if (MR::isValidSwitchB(this))
			switchB = MR::isOnSwitchB(this) == false;

		PlanetGravity* pGravity = mGravityCreator->getGravity();
		pGravity->mEnabled = switchA && switchB;
	}
}

void GlobalGravityObj::movement() {
	updateSwitch();
}

PlanetGravity* GlobalGravityObj::getGravity() {
	return mGravityCreator->getGravity();
}

namespace MR {
	GlobalGravityObj* createGlobalPointGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new PointGravityCreator();
		return ret;
	}

	GlobalGravityObj* createGlobalWireGravityObj(const char *pName) {
		GlobalGravityObj* ret = new GlobalGravityObj(pName);
		ret->mGravityCreator = new WireGravityCreator();
		return ret;
	}
}
