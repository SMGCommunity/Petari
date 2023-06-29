#include "Game/Map/HitInfo.h"
#include "Game/Player/MarioActor.h"
#include "Game/Gravity.h"
#include "Game/LiveActor/Binder.h"
#include "Game/Util/CameraUtil.h"
#include "Game/Player/MarioHolder.h"
#include "Game/Util/FootPrint.h"
#include "JSystem/JAudio2/JAIAudible.h"
#include "JSystem/JMath/JMath.h"
#include "Game/Player/MarioNullBck.h"

static bool isLuigi;

Triangle &Triangle::operator=(const Triangle &rOther) {
    mParts = rOther.mParts;
    mIdx = rOther.mIdx;
    mSensor = rOther.mSensor;
    mNormals[0] = rOther.mNormals[0];
    mNormals[1] = rOther.mNormals[1];
    mNormals[2] = rOther.mNormals[2];
    mNormals[3] = rOther.mNormals[3];
    mPos[0] = rOther.mPos[0];
    mPos[1] = rOther.mPos[1];
    mPos[2] = rOther.mPos[2];

    return *this;
}

static float ZERO = 0f;

void MarioActor::init(const JMapInfoIter &rInfo) {
	long stack_8 = -1;
	if(!MR::isValidInfo(rInfo)) return;
	TVec3f stack_24, stack_18, stack_c;

	MR::getJMapInfoTrans(rInfo, &stack_24);
	MR::getJMapInfoRotate(rInfo, &stack_18);
	MR::getJMapInfoScale(rInfo, &stack_c);

	stack_18.x = ZERO + (f32)fmod(stack_18.x - ZERO + 360f, (double)360.0);
	stack_18.y = ZERO + (f32)fmod(stack_18.y - ZERO + 360f, (double)360.0);
	stack_18.z = ZERO + (f32)fmod(stack_18.z - ZERO + 360f, (double)360.0);

	if(MR::isExistJMapArg(rInfo)) {
		MR::getJMapInfoArg0NoInit(rInfo, &stack_8);
	}
	init2(stack_24, stack_18, stack_8);
}

void MarioActor::initAfterPlacement() {
	updateGravityVector(true, true);
	_230 -> _1D8 = _240;
	_230 -> _1F0 = -_240;
	_230 -> _1FC = -_240;
	_300 = _230 -> _1F0;
	_2d0 = _300;
	_2c4 = _240 % -70f;
	calcCenterPos();
	MR::updateHitSensorsAll(this);
	_360 = getGravityVector();
	_1c0 = 1;
	_9f4 = getGravityVector();
	updateCameraInfo();
	calcBaseFrontVec(-_240);
}

void MarioActor::initAfterOpeningDemo() {
	_230 -> changeAnimationNonStop("ウォークイン");
	_37c = 0;
}

void MarioActor::calcBaseFrontVec(const TVec3f &rVec) {
	TVec3f stack_24, j(0f, 1f, 0f);
	f32 y = j.dot(rVec);
	if(y < -0.99f) {
		_258.setInline(0f, 0f, 1f);
	}
	else {
		f32 stack_8;
		if(MR::makeAxisAndCosignVecToVec(&stack_24, &stack_8, rVec, j)) {
			TVec3f k(0f, 0f, 1f);
			Mtx stack_30;
			PSMTXRotAxisRad(stack_30, stack_24.toCVec(), -JMAAcosRadian(stack_8));
			PSMTXMultVecSR(stack_30, k.toCVec(), _258.toVec());
			MR::normalize(&_258);
		}
		else {
			_258.setInline(0f, 0f, 1f);
		}
	}
}

void MarioActor::playSound(const char *name, long num) {
	_230 -> playSound(name, num);
}

void MarioActor::changeAnimation(const char *name, const char *newName) {
	_230 -> changeAnimation(name, newName);
}

void MarioActor::changeAnimationNonStop(const char *name) {
	_230 -> changeAnimationNonStop(name);
}

void MarioActor::changeAnimationUpper(const char *name) {
	if(_230 -> _71C) {
		if(isAnimationRun("基本")) {
			_230 -> changeAnimationUpper(name, NULL);
			return;
		}
	}
	_230 -> changeAnimation(name, (const char *)NULL);
}

void MarioActor::stopAnimation(const char *name) {
	_230 -> stopAnimation(name, (const char *)NULL);
}

bool MarioActor::isAnimationRun(const char *name) const {
	return _230 -> isAnimationRun(name);
}

void MarioActor::changeNullAnimation(const char *name, signed char num) {
	_b8c -> appear();
	MR::startBck(_b8c, name, NULL);
	_b92 = num;
}

bool MarioActor::isStopNullAnimation() const {
	if(!MR::isBckStopped(_b8c)) {
		return MR::isDead(_b8c);
	}
	return true;
}
