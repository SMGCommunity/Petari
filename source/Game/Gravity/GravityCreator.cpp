#include "Game/Gravity.h"
#include "Game/Util.h"

GravityCreator::GravityCreator() {

}

PlanetGravity* GravityCreator::createFromJMap(const JMapInfoIter &rIter) {
	PlanetGravity* instance = createInstance();

	if (instance) {
		TVec3f trans(0, 0, 0);
		TVec3f rotate(0, 0, 0);
		TVec3f scale(1, 1, 1);
		MR::getJMapInfoTrans(rIter, &trans);
		MR::getJMapInfoRotate(rIter, &rotate);
		MR::getJMapInfoScale(rIter, &scale);
		settingFromSRT(trans, rotate, scale);

		s32 arg0;
		s32 arg1;
		s32 arg2;
		MR::getJMapInfoArg0WithInit(rIter, &arg0);
		MR::getJMapInfoArg1WithInit(rIter, &arg1);
		MR::getJMapInfoArg2WithInit(rIter, &arg2);
		settingFromJMapArgs(arg0, arg1, arg2);

		settingFromJMapOtherParam(rIter);

		MR::settingGravityParamFromJMap(instance, rIter);
		getGravity()->updateIdentityMtx();
		MR::registerGravity(instance);
	}

	return instance;
}

PlanetGravity* CubeGravityCreator::createInstance() {
	mGravityInstance = new CubeGravity();
	return mGravityInstance;
}

PlanetGravity* CubeGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* DiskGravityCreator::createInstance() {
	mGravityInstance = new DiskGravity();
	return mGravityInstance;
}

PlanetGravity* DiskGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* DiskTorusGravityCreator::createInstance() {
	mGravityInstance = new DiskTorusGravity();
	return mGravityInstance;
}

PlanetGravity* DiskTorusGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* ConeGravityCreator::createInstance() {
	mGravityInstance = new ConeGravity();
	return mGravityInstance;
}

PlanetGravity* ConeGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* PlaneGravityCreator::createInstance() {
	mGravityInstance = new ParallelGravity();
	return mGravityInstance;
}

PlanetGravity* PlaneGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* PlaneInBoxGravityCreator::createInstance() {
	mGravityInstance = new ParallelGravity();
	mGravityInstance->setRangeType(ParallelGravity::RangeType_Box);
	return mGravityInstance;
}

PlanetGravity* PlaneInBoxGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* PlaneInCylinderGravityCreator::createInstance() {
	mGravityInstance = new ParallelGravity();
	mGravityInstance->setRangeType(ParallelGravity::RangeType_Cylinder);
	return mGravityInstance;
}

PlanetGravity* PlaneInCylinderGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* PointGravityCreator::createInstance() {
	mGravityInstance = new PointGravity();
	return mGravityInstance;
}

void PointGravityCreator::settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {
	mGravityInstance->mDistant = 500.0f * rScale.x;
	mGravityInstance->mTranslation.setInline(rTrans);
}

PlanetGravity* PointGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* SegmentGravityCreator::createInstance() {
	mGravityInstance = new SegmentGravity();
	return mGravityInstance;
}

PlanetGravity* SegmentGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* WireGravityCreator::createInstance() {
	mGravityInstance = new WireGravity();
	return mGravityInstance;
}

void WireGravityCreator::settingFromJMapOtherParam(const JMapInfoIter &rIter) {
	// Get data from JMap
	mRailRider = new RailRider(rIter);
	s32 numPointsInBetween = 20;
	MR::getJMapInfoArg0NoInit(rIter, &numPointsInBetween);

	// Determine number of points, the interval and allocate the necessary space
	u32 numPoints = numPointsInBetween + 2;
	f32 railInterval = mRailRider->getTotalLength() / (numPoints - 1);
	mGravityInstance->setPointListSize(numPoints);

	// Iterate over the points and add them to the gravity
	mRailRider->setCoord(0.0f);
	mRailRider->setSpeed(railInterval);

	for (s32 i = 0; i < numPoints - 1; i++) {
		mGravityInstance->addPoint(mRailRider->mCurPos);
		mRailRider->move();
	}
}

PlanetGravity* WireGravityCreator::getGravity() {
	return mGravityInstance;
}
