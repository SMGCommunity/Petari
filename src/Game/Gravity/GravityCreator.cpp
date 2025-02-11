#include "Game/Gravity.hpp"
#include "Game/Util.hpp"

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

void CubeGravityCreator::settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {
	// Calculate TR matrix
	TPos3f mtx;
	MR::makeMtxTR(reinterpret_cast<MtxPtr>(&mtx), rTrans, rRotate);

	// Get up vector and calculate scale
	TVec3f upVec;
	TVec3f scale;
	scale.setPS(rScale * 500.0f);
	mtx.getYDir(upVec);

	// Translate and scale matrix
	mtx.setTrans(rTrans + upVec * scale.y);
	MR::preScaleMtx(reinterpret_cast<MtxPtr>(&mtx), scale);

	// Set cube matrix
	mGravityInstance->setCube(mtx);
}

void CubeGravityCreator::settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2) {
	u8 activeFaces = 0;

	if (arg0 & 1) {
		activeFaces |= 1;
	}
	if (arg0 & 2) {
		activeFaces |= 2;
	}
	if (arg1 & 1) {
		activeFaces |= 4;
	}
	if (arg1 & 2) {
		activeFaces |= 8;
	}
	if (arg2 & 1) {
		activeFaces |= 16;
	}
	if (arg2 & 2) {
		activeFaces |= 32;
	}

	mGravityInstance->mActiveFaces = activeFaces;
}

PlanetGravity* CubeGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* DiskGravityCreator::createInstance() {
	mGravityInstance = new DiskGravity();
	return mGravityInstance;
}

void DiskGravityCreator::settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {
	// Calculate TR matrix
	TPos3f mtx;
	MR::makeMtxTR(reinterpret_cast<MtxPtr>(&mtx), rTrans, rRotate);

	// Calculate side and up vectors
	TVec3f sideVec, upVec;
	mtx.getXDir(sideVec);
	mtx.getYDir(upVec);

	// Populate gravity attributes
	mGravityInstance->setLocalPosition(rTrans);
	mGravityInstance->setLocalDirection(upVec);
	mGravityInstance->setSideDirection(sideVec);
	mGravityInstance->setRadius(500.0f * MR::getMaxElement(rScale));
}

void DiskGravityCreator::settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2) {
	// Obj_arg0 = enable both sides?
	mGravityInstance->setBothSide(arg0 != 0);

	// Obj_arg1 = enable edge gravity?
	mGravityInstance->setEnableEdgeGravity(arg1 != 0);

	// Obj_arg3 = valid degree
	if (arg2 >= 0) {
		mGravityInstance->setValidDegee(arg2);
	}
	else {
		mGravityInstance->setValidDegee(360.0f);
	}
}

PlanetGravity* DiskGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* DiskTorusGravityCreator::createInstance() {
	mGravityInstance = new DiskTorusGravity();
	return mGravityInstance;
}

void DiskTorusGravityCreator::settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {
	// Calculate TR matrix
	TPos3f mtx;
	MR::makeMtxTR(reinterpret_cast<MtxPtr>(&mtx), rTrans, rRotate);

	// Calculate side and up vectors
	TVec3f upVec;
	mtx.getYDir(upVec);

	// Populate gravity attributes
	mGravityInstance->setPosition(rTrans);
	mGravityInstance->setDirection(upVec);
	mGravityInstance->setRadius(500.0f * MR::getMaxElement(rScale));
}

void DiskTorusGravityCreator::settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2) {
	// Obj_arg0 = enable both sides?
	mGravityInstance->setBothSide(arg0 != 0);

	// Obj_arg1 = edge type
	switch (arg1) {
	case 0:
		mGravityInstance->setEdgeType(0);
		break;
	case 1:
		mGravityInstance->setEdgeType(1);
		break;
	case 2:
		mGravityInstance->setEdgeType(2);
		break;
	default:
		mGravityInstance->setEdgeType(3);
		break;
	}

	// Obj_arg2 = disk radius
	mGravityInstance->setDiskRadius(arg2);
}

PlanetGravity* DiskTorusGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* ConeGravityCreator::createInstance() {
	mGravityInstance = new ConeGravity();
	return mGravityInstance;
}

void ConeGravityCreator::settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {
	// Calculate local matrix
	TPos3f localMtx;
	MR::makeMtxTRS(reinterpret_cast<MtxPtr>(&localMtx), rTrans, rRotate, rScale * 500.0f);
	mGravityInstance->setLocalMatrix(localMtx);
}

void ConeGravityCreator::settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2) {
	// Obj_arg0 = is enable bottom?
	mGravityInstance->setEnableBottom(arg0 != 0);

	// Obj_arg1 = top cut rate
	mGravityInstance->setTopCutRate(arg1 / 1000.0f);
}

PlanetGravity* ConeGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* PlaneGravityCreator::createInstance() {
	mGravityInstance = new ParallelGravity();
	return mGravityInstance;
}

void PlaneGravityCreator::settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {
	// Calculate rotate matrix
	TPos3f mtx;
	MR::makeMtxRotate(reinterpret_cast<MtxPtr>(&mtx), rRotate);

	// Set normal-form plane
	TVec3f upVec;
	mtx.getYDir(upVec);
	mGravityInstance->setPlane(upVec, rTrans);
}

PlanetGravity* PlaneGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* PlaneInBoxGravityCreator::createInstance() {
	mGravityInstance = new ParallelGravity();
	mGravityInstance->setRangeType(ParallelGravity::RangeType_Box);
	return mGravityInstance;
}

void PlaneInBoxGravityCreator::settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {
	// Calculate scale and TR matrix
	TPos3f mtx;
	TVec3f scale = rScale * 500.0f;
	MR::makeMtxTR(reinterpret_cast<MtxPtr>(&mtx), rTrans, rRotate);

	// Get up vector
	TVec3f upVec;
	mtx.getYDir(upVec);

	// Translate and scale matrix
	mtx.setTrans(rTrans + upVec * scale.y);
	MR::preScaleMtx(reinterpret_cast<MtxPtr>(&mtx), scale);

	// Set cube matrix
	mGravityInstance->setPlane(upVec, rTrans);
	mGravityInstance->setRangeBox(mtx);
}

PlanetGravity* PlaneInBoxGravityCreator::getGravity() {
	return mGravityInstance;
}

void PlaneInBoxGravityCreator::settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2) {
	// Obj_arg0 = base distance
	mGravityInstance->setBaseDistance(arg0);

	// Obj_arg1 = distance calc type
	switch (arg1) {
	case 0:
		mGravityInstance->setDistanceCalcType(ParallelGravity::DistanceCalcType_X);
		break;
	case 1:
		mGravityInstance->setDistanceCalcType(ParallelGravity::DistanceCalcType_Y);
		break;
	case 2:
		mGravityInstance->setDistanceCalcType(ParallelGravity::DistanceCalcType_Z);
		break;
	default:
		mGravityInstance->setDistanceCalcType(ParallelGravity::DistanceCalcType_Default);
		break;
	}
}

PlanetGravity* PlaneInCylinderGravityCreator::createInstance() {
	mGravityInstance = new ParallelGravity();
	mGravityInstance->setRangeType(ParallelGravity::RangeType_Cylinder);
	return mGravityInstance;
}

PlanetGravity* PlaneInCylinderGravityCreator::getGravity() {
	return mGravityInstance;
}

void PlaneInCylinderGravityCreator::settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {
	// Calculate TR matrix
	TPos3f mtx;
	MR::makeMtxTR(reinterpret_cast<MtxPtr>(&mtx), rTrans, rRotate);

	// Set normal-form plane
	TVec3f upVec;
	mtx.getYDir(upVec);
	mGravityInstance->setPlane(upVec, rTrans);

	// Set cylinder range
	mGravityInstance->setRangeCylinder(500.0f * rScale.x, 500.0f * rScale.y);
}

void PlaneInCylinderGravityCreator::settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2) {
	// Obj_arg0 = base distance
	mGravityInstance->setBaseDistance(arg0);
}

PlanetGravity* PointGravityCreator::createInstance() {
	mGravityInstance = new PointGravity();
	return mGravityInstance;
}

void PointGravityCreator::settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {
	mGravityInstance->mDistant = 500.0f * rScale.x;
	mGravityInstance->mOrigPosition.setPS(rTrans);
}

PlanetGravity* PointGravityCreator::getGravity() {
	return mGravityInstance;
}

PlanetGravity* SegmentGravityCreator::createInstance() {
	mGravityInstance = new SegmentGravity();
	return mGravityInstance;
}

void SegmentGravityCreator::settingFromSRT(const TVec3f &rTrans, const TVec3f &rRotate, const TVec3f &rScale) {
	// Calculate TRS matrix
	TPos3f mtx;
	MR::makeMtxTRS(reinterpret_cast<MtxPtr>(&mtx), rTrans, rRotate, rScale);

	// Get side and up vectors
	TVec3f sideVec, upVec;
	mtx.getXDir(sideVec);
	mtx.getYDir(upVec);

	// Set gravity points and side vec
	mGravityInstance->setGravityPoint(0, rTrans);
	mGravityInstance->setGravityPoint(1, rTrans + upVec * 1000.0f);
	mGravityInstance->setSideVector(sideVec);
}

void SegmentGravityCreator::settingFromJMapArgs(s32 arg0, s32 arg1, s32 arg2) {
	// Obj_arg0 = valid edge
	switch (arg0) {
	case 0:
		mGravityInstance->setEdgeValid(0, false);
		mGravityInstance->setEdgeValid(1, false);
		break;
	case 1:
		mGravityInstance->setEdgeValid(0, true);
		mGravityInstance->setEdgeValid(1, false);
		break;
	case 2:
		mGravityInstance->setEdgeValid(0, false);
		mGravityInstance->setEdgeValid(1, true);
		break;
	default:
		mGravityInstance->setEdgeValid(0, true);
		mGravityInstance->setEdgeValid(1, true);
		break;
	}

	// Obj_arg1 = valid side degree
	if (arg1 >= 0.0f) {
		mGravityInstance->setValidSideDegree(arg1);
	}
	else {
		mGravityInstance->setValidSideDegree(360.0f);
	}
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
