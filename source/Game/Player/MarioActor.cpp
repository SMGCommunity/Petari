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
#include "Game/Enemy/KariKariDirector.h"
#include "Game/Animation/XanimePlayer.h"

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

void MarioActor::changeGameOverAnimation() {
	int r30 = 0;
	if(_230 -> isStatusActive(0x12)) _230 -> closeStatus(NULL);

	if(_230 -> isAnimationRun("0x001")) r30 = 0;
	if(_230 -> isAnimationRun("0x002")) r30 = 0;
	if(_230 -> isAnimationRun("0x003")) r30 = 0;
	if(_230 -> isAnimationRun("0x004")) r30 = 0;
	if(_230 -> isAnimationRun("0x005")) r30 = 1;
	if(_230 -> isAnimationRun("0x006")) r30 = 1;
	if(_230 -> isAnimationRun("0x007")) r30 = 2;
	if(_230 -> isAnimationRun("0x008")) r30 = 2;
	if(_230 -> isAnimationRun("0x009")) r30 = 3;
	if(_230 -> isAnimationRun("0x00a")) r30 = 3;
	if(_230 -> isAnimationRun("0x00b")) r30 = 3;
	if(_230 -> isAnimationRun("0x00c")) r30 = 4;
	if(_230 -> isAnimationRun("0x00d")) r30 = 4;
	if(_230 -> isAnimationRun("0x00e")) r30 = 4;
	if(_230 -> isAnimationRun("0x00f")) r30 = 5;
	if(_230 -> isAnimationRun("0x010")) r30 = 5;
	if(_230 -> isAnimationRun("0x011")) r30 = 6;
	if(_230 -> isAnimationRun("0x012")) r30 = 6;
	if(!_230 -> isAnimationRun("0x013") && !_230 -> isStatusActive(0xd));
	else r30 = -1;
	if(_230 -> isSwimming()) r30 = 7;
	if(isNerve(&NrvMarioActor::MarioActorNrvGameOverSink::sInstance)) r30 = 8;
	if(MR::getKariKariClingNum() && _230 -> _8 >> 0x1e & 1) r30 = 4;
	if(_39d == 2) r30 = 9;
	if(_39d == 1) r30 = 10;

	switch(r30) {
		case 0:
			_230 -> changeAnimationNonStop("0x015");
			break;
		case 1:
			_230 -> changeAnimationNonStop("0x016");
			break;
		case 2:
			_230 -> changeAnimationNonStop("0x017");
			break;
		case 3:
			_230 -> changeAnimationNonStop("0x018");
			break;
		case 4:
			_230 -> changeAnimationNonStop("0x019");
			break;
		case 5:
			if(_230 -> _8 >> 0x1e & 1) _230 -> changeAnimationNonStop("0x01a");
			else _230 -> changeAnimationNonStop("0x01b");
			break;
		case 6:
			_230 -> changeAnimationNonStop("0x01c");
			break;
		case 7:
			_230 -> changeAnimationNonStop("0x01d");
			break;
		case 8:
			_230 -> changeAnimationNonStop("0x01e");
			break;
		case 9:
			_230 -> changeAnimationNonStop("0x01f");
			break;
		case 10:
			_230 -> changeAnimationNonStop("0x020");
			break;
	}
	_b90 = 1;
	stopEffect("0x021");
	_a6e = 0;
}

XjointTransform* MarioActor::getJointCtrl(const char *name) const {
	XanimeCore *core = _234 -> _c -> mCore;
	return core -> getJointTransform(MR::getJointIndex(this, name));
}

void MarioActor::updateRotationInfo() {
	TRot3f stack_44;
	PSMTXConcat(getBaseMtx(), _e3c.toMtxPtr(), stack_44.toMtxPtr());
	MR::makeRTFromMtxPtr(NULL, &mRotation, stack_44.toMtxPtr(), true);
	if(mRotation.z > 90f && mRotation.x > 90f) {
		f32 diff = 180f - mRotation.y;
		mRotation.z = 0f;
		mRotation.y = diff;
		mRotation.x = 0f;
	}
	stack_44.getEuler(_318);
	_318.multAssignInline(57.2957763672f);
	stack_44.getEuler(_324);
	_324.multAssignInline(57.2957763672f);
	if(MR::isSameDirection(_240, _230 -> _208, .01f)) _a18 = mRotation;
	else {
		TPos3f stack_14;
		TVec3f stack_8 = -_240;
		MR::makeMtxUpFront(&stack_14, stack_8, _230 -> _208);
		MR::makeRTFromMtxPtr(NULL, &_a18, stack_14.toMtxPtr(), true);
		if(_a18.z > 90f && _a18.x > 90f) {
			f32 diff = 180f - _a18.y;
			_a18.z = 0f;
			_a18.y = diff;
			_a18.x = 0f;
		}
	}
}
