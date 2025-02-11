#include "Game/MapObj/CocoNut.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath.hpp"
#include "math_types.hpp"

inline void negateInternalInline(const TVec3f &src, TVec3f *dst) {
	JGeometry::negateInternal((f32 *)&src, (f32 *)dst);
}

CocoNut::CocoNut(const char *pName) : LiveActor(pName),
	_8C(0.0f),
	_90(0.0f),
	//_94(0.0f, 1.0f),
	_D0(55.0f),
	_D4(false),
	_138(0),
	_13C(0),
	mSpawnPosition(gZeroVec),
	_14C(false),
	_150(gZeroVec),
	mSphericalShadow(false),
	mRespawnWhenOutOfView(false),
	_15E(false),
	mContinueRolling(false)
{
	_A0.identity();
	_D8.identity();
	_108.identity();
}

CocoNut::~CocoNut() {
}

void CocoNut::init(const JMapInfoIter &rIter) {
	initMapToolInfo(rIter);
	initModel();
	MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
	MR::initLightCtrl(this);
	initSensor();
	initBinder(_D0, 0.0f, 0);
	MR::onCalcGravity(this);
	initEffect();
	initSound(6, 0);

	if (!mSphericalShadow) {
		MR::initShadowVolumeCylinder(this, _D0);
	}
	else {
		MR::initShadowVolumeSphere(this, _D0);
	}

	MR::setShadowDropLength(this, nullptr, 1500.0f);

	s32 stack_8;
	if (MR::getJMapInfoClippingGroupID(rIter, &stack_8)) {
		MR::setGroupClipping(this, rIter, 32);
		_15E = true;
	}
	else {
		MR::setClippingFar200m(this);
	}

	MR::tryRegisterDemoCast(this, rIter);

	if (!mSphericalShadow) {
		initNerve(&NrvCocoNut::CocoNutNrvWaitOnBind::sInstance);
	}
	else {
		initNerve(&NrvCocoNut::CocoNutNrvWait::sInstance);
	}

	makeActorAppeared();
}

void CocoNut::initAfterPlacement() {
	TPos3f stack_50;

	TVec3f gravity(mGravity);
	MR::makeMtxTR(&stack_50.mMtx[0], this);

	_94.set(stack_50.mMtx[0][2], stack_50.mMtx[1][2], stack_50.mMtx[2][2]);

	if (MR::isSameDirection(_94, gravity, 0.01f)) {
		TVec3f gravityNegated;
		negateInternalInline(gravity, &gravityNegated);

		TPos3f stack_20;
		MR::makeMtxUpNoSupport(&stack_20, gravityNegated);
		_94.set(stack_20.mMtx[0][2], stack_20.mMtx[1][2], stack_20.mMtx[2][2]);
	}
}

void CocoNut::startClipped() {
	if (isNerve(&NrvCocoNut::CocoNutNrvReplaceReady::sInstance)) {
		if (!MR::isDemoActive()) {
			_90 = 0.0f;
			_8C = 0.0f;
			_150.zero();
			mVelocity.zero();
			MR::onBind(this);
			MR::onCalcGravity(this);
			MR::showModel(this);
			MR::validateHitSensors(this);
			if (!mSphericalShadow) {
				_D4 = false;
				setNerve(&NrvCocoNut::CocoNutNrvWait::sInstance);
			}
			else {
				setNerve(&NrvCocoNut::CocoNutNrvWaitOnBind::sInstance);
			}
		}
	}
	else if (mRespawnWhenOutOfView) {
		statusToHide();
		mPosition.set<f32>(mSpawnPosition);
		setNerve(&NrvCocoNut::CocoNutNrvReplaceReady::sInstance);
	}
	LiveActor::startClipped();
}

void CocoNut::hit(const TVec3f &a1, f32 a2) {
	setFrontVec(a1);

	f32 var_f0;
	if (a2 < 1.5f) {
		var_f0 = 1.5f;
	}
	else if (a2 > 35.0f) {
		var_f0 = 35.0f;
	}
	else {
		var_f0 = a2;
	}

	_8C = var_f0;
	if (!isNerve(&NrvCocoNut::CocoNutNrvMove::sInstance)) {
		setNerve(&NrvCocoNut::CocoNutNrvMove::sInstance);
	}
}

bool CocoNut::isPossibleToHit(const TVec3f &a1, const TVec3f &a2, const TVec3f &a3) const {
	TVec3f stack_2C;
	TVec3f stack_20;
	TVec3f stack_14;
	TVec3f stack_8;

	stack_2C.sub(a2, a1);
	if (MR::normalizeOrZero(&stack_2C)) {
		return false;
	}
	if (MR::normalizeOrZero(a3, &stack_14)) {
		return false;
	}
	if (isNerve(&NrvCocoNut::CocoNutNrvMove::sInstance)) {
		if (MR::normalizeOrZero(mVelocity, &stack_8)) {
			return false;
		}
		stack_20.sub(stack_14, stack_8);
		if (MR::normalizeOrZero(&stack_20)) {
			return false;
		}
	}
	else {
		stack_20.set<f32>(stack_14);
	}
	return stack_2C.dot(stack_20) < 0.0f;
}

f32 CocoNut::calcMoveSpeed() const {
	return !isNerve(&NrvCocoNut::CocoNutNrvMove::sInstance) ? 0.0f : MR::max(_8C, PSVECMag(&_150));
}

void CocoNut::initSensor() {
	initHitSensor(2);
	MR::addHitSensor(this, "body", 0x17, 0x10, 65.0f * mScale.x, TVec3f(0.0f, 0.0f, 0.0f));
	MR::addHitSensor(this, "eye", 0x7f, 0x10, 1.1f * mScale.x, TVec3f(0.0f, 0.0f, 0.0f));
}

void CocoNut::initModel() {
	initModelManagerWithAnm(getModelName(), nullptr, false);
}

void CocoNut::initEffect() {
	initEffectKeeper(0, "CocoNut", false);
	MR::setEffectHostMtx(this, "RollingSmoke", _D8.toMtxPtr());
	MR::setEffectHostMtx(this, "RollingSmokeAttrWater", _D8.toMtxPtr());
	MR::setEffectHostMtx(this, "RollingSmokeAttrSand", _D8.toMtxPtr());
	MR::setEffectHostMtx(this, "Land", _D8.toMtxPtr());
	MR::setEffectHostMtx(this, "LandAttrWater", _D8.toMtxPtr());
	MR::setEffectHostMtx(this, "WaterColumn", _D8.toMtxPtr());
	MR::setEffectHostMtx(this, getBreakEffectName(), _D8.toMtxPtr());
	MR::setEffectHostMtx(this, "SpinHitMark", _108.toMtxPtr());
}

#ifdef NON_MATCHING
// hell function
void CocoNut::updateRotate(float a1) {
	TMtx34f stack_38;
	TVec3f stack_2C;
	TVec3f stack_20;
	TVec3f stack_14;
	TVec3f stack_8;

	negateInternalInline(mGravity, &stack_20);
	if (!MR::normalizeOrZero(mVelocity, &stack_2C) && !MR::isSameDirection(stack_2C, stack_20, 0.01f)) {

		PSVECCrossProduct(stack_2C, stack_20, stack_14);

		f32 temp1 = PSVECMag(mVelocity) * -180.0f;
		f32 temp2 = a1 * temp1;
		f32 f = PI_180 * (temp2 / (PI * getSize()));

		stack_38.mMtx[0][3] = 0.0f;
		stack_38.mMtx[1][3] = 0.0f;
		stack_38.mMtx[2][3] = 0.0f;

		stack_8.set(stack_14);
		PSVECMag(stack_8);
		PSVECNormalize(stack_8, stack_8);

		f32 fsin = sin(f);
		f32 fcos = cos(f);
		f32 rx = stack_8.x;
		f32 ry = stack_8.y;
		f32 rz = stack_8.z;
		f32 fcos1 = 1.0f - fcos;

		stack_38.mMtx[0][0] = (rx * rx * fcos1) + fcos;
		stack_38.mMtx[0][1] = fcos1 * rx * ry - (fsin * rz);
		stack_38.mMtx[0][2] = fcos1 * rx * rz + (fsin * ry);
		stack_38.mMtx[1][0] = fcos1 * rx * ry + (fsin * rz);
		stack_38.mMtx[1][1] = (ry * ry * fcos1) + fcos;
		stack_38.mMtx[1][2] = fcos1 * ry * rz - (fsin * rx);
		stack_38.mMtx[2][0] = fcos1 * rx * rz - (fsin * ry);
		stack_38.mMtx[2][1] = fcos1 * ry * rz + (fsin * rx);
		stack_38.mMtx[2][2] = (rz * rz * fcos1) + fcos;

		_A0.concat(stack_38, _A0);
	}
}
#endif

void CocoNut::updateGravity() {
	TVec3f stack_8;

	f32 f31 = _13C ? 0.4f : 1.0f;
	stack_8.set<f32>(mGravity);
	f32 f0 = _90 + f31;

	f32 f2 = f0 >= 25.0f ? 25.0f : f0;
	_90 = f2;
	stack_8.scale((f32)(f64)f2); // real
	mVelocity.add(stack_8);
}

#ifdef NON_MATCHING
// issues around MR::deleteEffect and PSVECNormalize calls
void CocoNut::processMove() {
	TVec3f stack_2C;
	TVec3f stack_20;
	TVec3f stack_14;
	TVec3f stack_8;

	if (isOnGround()) {
		f32 temp_f31 = calcMoveSpeed();
		bool temp_r31 = MR::isBindedGroundWater(this);
		TVec3f *temp_r3_1 = MR::getGroundNormal(this);
		if (2.5f < _90) {
			if (mGravity.dot(*temp_r3_1) < 0.0f) {
				_90 *= 0.5f;
				s32 var_r5 = 100.0f * (temp_f31 / 35.0f);
				if (var_r5 > 100) {
					var_r5 = 100;
				}
				if (var_r5 > 0) {
					if (temp_r31) {
						MR::startSound(this, "SE_OJ_COCONUT_BOUND_WATER", var_r5, -1);
					}
					else {
						MR::startSound(this, "SE_OJ_COCONUT_BOUND", var_r5, -1);
					}
				}
			}
			if (_138 >= 10 && 3.0f < _90) {
				MR::emitEffect(this, "Land");
			}
		}
		else {
			_90 = 0.0f;
		}
		MR::emitEffect(this, "RollingSmoke");
		s32 var_r5_2 = 100.0f * (temp_f31 / 35.0f);
		if (var_r5_2 > 100) {
			var_r5_2 = 100;
		}
		if (var_r5_2 >= 10) {
			if (temp_r31) {
				MR::startLevelSound(this, "SE_OJ_LV_COCONUT_ROLL_WATER", var_r5_2, -1, -1);
			}
			else {
				MR::startLevelSound(this, "SE_OJ_LV_COCONUT_ROLL", var_r5_2, -1, -1);
			}
		}
		_8C *= 0.925f;
		_14C = MR::calcVelocityAreaMoveOnGround(&stack_2C, this);
		if (_14C) {
			_150.set(stack_2C);
			_150.scaleInline(0.75f);
		}
		_138 = 0;
		_13C = false;
		updateRotate(1.0f);
	}
	else {
		// volatile?
		if (_138 < 10) {
			_138++;
		}
		else {
			MR::deleteEffect(this, "RollingSmoke");
		}
		updateRotate(0.75f);
	}

	if (getWallNormal(&stack_20) && _94.dot(stack_20) < 0.0f) {
		stack_14.set(_94);
		PSVECMag(stack_14);
		PSVECNormalize(stack_14, stack_14);

		stack_8.set(stack_20);
		PSVECMag(stack_8);
		PSVECNormalize(stack_8, stack_8);

		f32 ok2 = -2.0f * stack_14.dot(stack_8);
		JMAVECScaleAdd(stack_8, _94, _94, ok2);

		PSVECMag(_94);
		PSVECNormalize(_94, _94);
		_94.normalize(_94);

		_8C *= 0.8f;

		MR::startSound(this, "SE_OJ_COCONUT_BOUND", 100.0f * (calcMoveSpeed() / 35.0f), -1);
	}
	setFrontVec(_94);
	mVelocity.set(_94);
	mVelocity.scale(_8C);

	bool ok = _14C && _138 < 10;
	if (!ok) {
		_150.scaleInline(0.925f);
	}
	mVelocity.add(_150);
	updateGravity();
}
#endif

void CocoNut::setFrontVec(const TVec3f &a1) {
	TVec3f stack_14;
	TVec3f stack_8(mGravity);
	if (!MR::normalizeOrZero(a1, &stack_14)) {
		if (MR::isSameDirection(a1, stack_8, 0.01f)) {
			_94.set<f32>(stack_14);
		}
		else {
			MR::vecKillElement(stack_14, stack_8, &_94);
			MR::normalize(&_94);
		}
	}
}

bool CocoNut::tryHit(HitSensor *pOtherSensor, HitSensor *pMySensor) {
	CocoNut *nut = reinterpret_cast<CocoNut *>(pMySensor->mActor);
	if (!isNerve(&NrvCocoNut::CocoNutNrvMove::sInstance)) {
		return false;
	}
	f32 moveSpeed = nut->calcMoveSpeed();
	if (calcMoveSpeed() < moveSpeed) {
		return false;
	}
	TVec3f *otherSensorPos = &pOtherSensor->mPosition;
	TVec3f *mySensorPos = &pMySensor->mPosition;
	if (!nut->isPossibleToHit(*mySensorPos, *otherSensorPos, mVelocity)) {
		return false;
	}

	TVec3f stack_1C;
	TVec3f stack_10;
	f32 stack_C;
	f32 stack_8;
	calcHitSpeedAndFrontVec(&stack_C, &stack_8, &stack_1C, &stack_10, *otherSensorPos, *mySensorPos);
	hit(stack_1C, stack_C);
	nut->hit(stack_10, stack_8);
	return true;
}

bool CocoNut::tryPushedFromActor(HitSensor *pOtherSensor, HitSensor *pMySensor) {
	TVec3f stack_34;
	TVec3f stack_28;
	TVec3f stack_1C;
	TVec3f stack_10;
	f32 stack_C;
	f32 stack_8;

	TVec3f *otherSensorPos = &pOtherSensor->mPosition;
	TVec3f *mySensorPos = &pMySensor->mPosition;
	if (_13C) {
		return false;
	}
	if (isNerve(&NrvCocoNut::CocoNutNrvMove::sInstance)) {
		stack_34.sub(*otherSensorPos, *mySensorPos);
		MR::normalize(&stack_34);
		if (0.0f < stack_34.dot(_94)) {
			return false;
		}
		calcHitSpeedAndFrontVec(&stack_C, &stack_8, &stack_28, &stack_1C, *otherSensorPos, *mySensorPos);
		hit(stack_28, stack_C);
	}
	else {
		f32 mySensorRadius = pMySensor->mRadius;
		f32 otherSensorRadius = pOtherSensor->mRadius;
		if (((otherSensorRadius + mySensorRadius) - PSVECDistance(otherSensorPos, mySensorPos)) < 0.0f) {
			return false;
		}
		stack_10.sub(*otherSensorPos, *mySensorPos);
		if (MR::normalizeOrZero(&stack_10)) {
			return false;
		}
		hit(stack_10, 1.5f);
	}
	return true;
}

void CocoNut::reviseFrontVec() {
	HitSensor *sensor;

	HitSensor *eye = getSensor("eye");
	LiveActor *found_actor = nullptr;

	for (int i = 0; i < eye->mSensorCount; i++) {
		sensor = eye->mSensors[i];
		if ((sensor->isType(0x26) || sensor->isType(0x56)) && !MR::isDead(sensor->mActor)) {
			found_actor = sensor->mActor;
			break;
		}
	}
	if (found_actor == nullptr) {
		return;
	}

	TVec3f stack_28(mGravity);
	TVec3f stack_20;
	TVec3f stack_14;
	TVec3f stack_8;

	stack_14.sub(found_actor->mPosition, this->mPosition);
	if (!MR::isSameDirection(stack_14, stack_20, 0.01f)) {
		MR::vecKillElement(stack_14, stack_20, &stack_8);
		MR::normalize(&stack_8);
		f32 temp_f31 = stack_8.dot(_94);
		if (MR::cosDegree(15.0f) < temp_f31) {
			JMAVECLerp(&_94, &stack_8, &_94, 0.8f);
		}
	}
}

void CocoNut::statusToWait() {
	mVelocity.zero();
	_90 = 0.0f;
	_8C = 0.0f;
	_150.zero();
	MR::validateClipping(this);
	if (!mSphericalShadow && !MR::isBindedGroundIce(this)) {
		_D4 = true;
		MR::offBind(this);
		MR::offCalcGravity(this);
		if (!isNerve(&NrvCocoNut::CocoNutNrvWait::sInstance)) {
			setNerve(&NrvCocoNut::CocoNutNrvWait::sInstance);
		}
	}
	else {
		setNerve(&NrvCocoNut::CocoNutNrvWaitOnBind::sInstance);
	}
}

void CocoNut::tryMoveEnd() {
	if (isOnGround()) {
		bool var_r3 = _14C && _138 < 10;
		if (!var_r3 && calcMoveSpeed() < 1.5f && (!mContinueRolling || !isContactWithOtherCocoNut())) {
			statusToWait();
			return;
		}
	}
	if (sendMsgToBindedSensor()) {
		setNerve(&NrvCocoNut::CocoNutNrvBreak::sInstance);
		return;
	}
	if (!tryDisappear()) {
		_8C = MR::max(_8C, 1.5f);
	}
}

bool CocoNut::tryDisappear() {
	TVec3f stack_14;
	stack_14.scale(-100.0f, mGravity);
	if (MR::isInWater(this, stack_14)) {
		setNerve(&NrvCocoNut::CocoNutNrvInWater::sInstance);
		return true;
	}
	if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
		setNerve(&NrvCocoNut::CocoNutNrvBreak::sInstance);
		return true;
	}
	return false;
}

bool CocoNut::isValidPushedFromPlayer(const HitSensor *arg0, const HitSensor *arg1) const {
	if (_90 < 0.0f) {
		return false;
	}
	if (isNerve(&NrvCocoNut::CocoNutNrvMove::sInstance) && MR::isLessStep(this, 15)) {
		return false;
	}
	TVec3f *playerVelocity = MR::getPlayerVelocity();
	f32 nutVelocitySquared = mVelocity.squared();
	if (playerVelocity->squared() < nutVelocitySquared) {
		return false;
	}
	TVec3f stack_14;
	stack_14.sub(arg0->mPosition, arg1->mPosition);
	if (MR::normalizeOrZero(&stack_14)) {
		return false;
	}
	TVec3f stack_8;
	if (MR::normalizeOrZero(*playerVelocity, &stack_8)) {
		return false;
	}
	if (stack_14.dot(stack_8) < MR::cosDegree(45.0f)) {
		return false;
	}

	return true;
}

#ifdef NON_MATCHING
// the frsqrte is most likely an inlined function. possibly JGeometry::TUtil<f32>::sqrt
void CocoNut::calcHitSpeedAndFrontVec(f32 *arg0, f32 *arg1, TVec3f *arg2, TVec3f *arg3, const TVec3f &arg4, const TVec3f &arg5) const {
	TVec3f stack_14;
	TVec3f stack_8;

	arg3->sub(arg5, arg4);
	MR::normalize(arg3);
	stack_14.set(mGravity);
	PSVECCrossProduct(arg3, stack_14, arg2);
	MR::normalize(arg2);
	if (MR::normalizeOrZero(mVelocity, &stack_8)) {
		stack_8.set(_94);
	}
	f32 var_f30 = stack_8.dot(*arg2);
	if (var_f30 < 0.0f) {
		negateInternalInline(stack_14, &stack_14);
		PSVECCrossProduct(arg3, stack_14, arg2);
		MR::normalize(arg2);
		var_f30 = stack_8.dot(*arg2);
	}

	f32 temp_f2 = 1.0f - (var_f30 * var_f30);

	f32 var_f31;
	if (temp_f2 > 0.0f) {
        f32 temp_f31 = __frsqrte(temp_f2);
        f32 temp_f3 = temp_f31 * temp_f2;
        var_f31 = -((temp_f3 * temp_f31) - 3.0f) * temp_f3 * 0.5f;
    } else {
        var_f31 = temp_f2;
    }

	f32 temp_f1_2 = calcMoveSpeed();
	*arg1 = temp_f1_2 * var_f31;
	*arg0 = temp_f1_2 * var_f30;
}
#endif

bool CocoNut::isOnGround() const {
	if (0.0f < _90 && MR::isOnGround(this)) {
		TVec3f *groundNormal = MR::getGroundNormal(this);
		if (groundNormal->dot(mGravity) < MR::cosDegree(120.0f)) {
			return true;
		}
	}
	return false;
}

bool CocoNut::getWallNormal(TVec3f *arg0) const {
	if (MR::isBindedWall(this)) {
		arg0->set<f32>(*MR::getWallNormal(this));
		return true;
	}
	if (0.0f < _90 && (MR::isOnGround(this)) && !isOnGround()) {
		arg0->set<f32>(*MR::getGroundNormal(this));
		return true;
	}
	return false;
}

bool CocoNut::sendMsgToBindedSensor() {
	if (MR::isBindedGround(this)) {
		return sendMsgEnemyAttackToBindedSensor(MR::getGroundSensor(this));
	}
	if (MR::isBindedWall(this)) {
		return sendMsgEnemyAttackToBindedSensor(MR::getWallSensor(this));
	}
	if (MR::isBindedRoof(this)) {
		return sendMsgEnemyAttackToBindedSensor(MR::getRoofSensor(this));
	}
	return 0;
}

bool CocoNut::sendMsgEnemyAttackToBindedSensor(HitSensor *arg0) {
	if (_13C) {
		return MR::sendMsgEnemyAttack(arg0, getSensor("body"));
	}
	return false;
}

bool CocoNut::isValidReceiveMsg(const HitSensor *a1) const {
	return (
		isNerve(&NrvCocoNut::CocoNutNrvWait::sInstance) ||
		isNerve(&NrvCocoNut::CocoNutNrvWaitOnBind::sInstance) ||
		isNerve(&NrvCocoNut::CocoNutNrvMove::sInstance)
	) && a1 == getSensor("body");
}

const char *CocoNut::getModelName() {
	bool watermelonMode = MR::isStarPieceCounterStop();
	return watermelonMode ? "Watermelon" : "CocoNut";
}

const char *CocoNut::getBreakEffectName() {
	bool watermelonMode = MR::isStarPieceCounterStop();
	return watermelonMode ? "BreakWatermelon" : "CocoNutBreak";
}

void CocoNut::makeArchiveList(NameObjArchiveListCollector *pArchiveList, const JMapInfoIter &rIter) {
	pArchiveList->addArchive(getModelName());
}

void CocoNut::calcAndSetBaseMtx() {
	_A0.mMtx[0][3] = this->mPosition.x;
	_A0.mMtx[1][3] = this->mPosition.y;
	_A0.mMtx[2][3] = this->mPosition.z;

	MR::setBaseTRMtx(this, _A0);

	if (isNerve(&NrvCocoNut::CocoNutNrvMove::sInstance) && MR::isOnGround(this)) {
		TVec3f *groundNormal = MR::getGroundNormal(this);
		f32 temp_f31 = _D0;

		TVec3f stack_8(*groundNormal);
		stack_8.scale(temp_f31);

		TVec3f stack_14;
		stack_14.sub(mPosition, stack_8);

		if (MR::isSameDirection(*groundNormal, _94, 0.01f)) {
			MR::makeMtxUpNoSupportPos(&_D8, *groundNormal, stack_14);
		}
		else {
			MR::makeMtxUpFrontPos(&_D8, *groundNormal, _94, stack_14);
		}

		f32 temp_f9 = mScale.x;
		_D8.mMtx[0][0] *= temp_f9;
		_D8.mMtx[0][1] *= temp_f9;
		_D8.mMtx[0][2] *= temp_f9;
		_D8.mMtx[1][0] *= temp_f9;
		_D8.mMtx[1][1] *= temp_f9;
		_D8.mMtx[1][2] *= temp_f9;
		_D8.mMtx[2][0] *= temp_f9;
		_D8.mMtx[2][1] *= temp_f9;
		_D8.mMtx[2][2] *= temp_f9;
	}
}

void CocoNut::attackSensor(HitSensor *arg0, HitSensor *arg1) {
	if (isValidReceiveMsg(arg0)) {
		if (MR::isSensorPlayer(arg1) && !isValidPushedFromPlayer(arg0, arg1)) {
			if (!MR::isPlayerHipDropFalling()) {
				MR::sendMsgPush(arg1, arg0);
			}
		}
		else if (arg1->isType(0x17)) {
			if (MR::sendMsgPush(arg1, arg0)) {
				MR::startSound(this, "SE_OJ_COCONUT_HIT", -1, -1);
			}
		}
		else {
			if (_13C && isNerve(&NrvCocoNut::CocoNutNrvMove::sInstance) && MR::sendMsgToEnemyAttackBlow(arg1, arg0)) {
				MR::startSound(this, "SE_OJ_COCONUT_HIT", -1, -1);
				setNerve(&NrvCocoNut::CocoNutNrvBreak::sInstance);
			}
			else {
				MR::sendMsgPush(arg1, arg0);
			}
		}
	}
}

bool CocoNut::receiveMsgPush(HitSensor *pMySensor, HitSensor *pOtherSensor) {
	if (!isValidReceiveMsg(pOtherSensor)) {
		return false;
	}
	if (pMySensor->isType(0x17)) {
		return tryHit(pOtherSensor, pMySensor);
	}
	return tryPushedFromActor(pOtherSensor, pMySensor);
}

bool CocoNut::receiveMsgPlayerAttack(u32 a1, HitSensor *pOtherSensor, HitSensor *pMySensor) {
	if (!isValidReceiveMsg(pMySensor)) {
		return false;
	}
	if (!MR::isMsgPlayerHitAll(a1) && !MR::isMsgPlayerTrample(a1) && !MR::isMsgPlayerHipDrop(a1) && !MR::isMsgStarPieceReflect(a1)) {
		return false;
	}
	f32 var_f31;
	f32 var_f30;
	if (MR::isMsgPlayerSpinAttack(a1)) {
		var_f31 = 35.0f;
		var_f30 = -20.0f;
		MR::startSound(this, "SE_PM_SPIN_HIT", -1, -1);
		MR::startSound(this, "SE_OJ_COCONUT_LAUNCH", -1, -1);
		emitEffectSpinHit(pOtherSensor, pMySensor);
		_13C = true;
	}
	else if (MR::isMsgPlayerHipDrop(a1) || MR::isMsgInvincibleAttack(a1)) {
		var_f31 = 25.0f;
		var_f30 = -15.0f;
		MR::startSound(this, "SE_OJ_COCONUT_FLIP_M", -1, -1);
		if (MR::isMsgPlayerHipDrop(a1)) {
			MR::sendMsgAwayJump(pOtherSensor, pMySensor);
		}
	}
	else {
		var_f31 = 10.0f;
		var_f30 = -5.0f;
		MR::startSound(this, "SE_OJ_COCONUT_FLIP_S", -1, -1);
	}
	_8C = var_f31;
	_90 = var_f30;
	TVec3f stack_2C;
	TVec3f stack_20;
	TVec3f stack_14;
	stack_2C.sub(pMySensor->mPosition, pOtherSensor->mPosition);
	if (MR::isMsgStarPieceReflect(a1) && !MR::normalizeOrZero(stack_2C, &stack_20) && !MR::normalizeOrZero(pOtherSensor->mActor->mVelocity, &stack_14)) {
		if (stack_20.dot(stack_14) < MR::cosDegree(60.0f)) {
			TVec3f stack_8(stack_14);
			stack_8.scale(65.0f);
			stack_2C.add(stack_8);
		}
	}
	setFrontVec(stack_2C);
	if (MR::isMsgPlayerSpinAttack(a1)) {
		reviseFrontVec();
	}
	setNerve(&NrvCocoNut::CocoNutNrvMove::sInstance);
	return true;
}

bool CocoNut::receiveOtherMsg(u32 arg0, HitSensor *pOtherSensor, HitSensor *pMySensor) {
	f32 temp_f0;
	f32 temp_f1;
	f32 var_f2;

	if (!isValidReceiveMsg(pMySensor)) {
		return false;
	}
	if (MR::isMsgPlayerKick(arg0)) {
		if (isValidPushedFromPlayer(pMySensor, pOtherSensor)) {
			TVec3f *playerVelocity = MR::getPlayerVelocity();
			setFrontVec(*playerVelocity);
			temp_f1 = _94.dot(*playerVelocity);
			var_f2 = 18.0f;
			temp_f0 = 1.1f * temp_f1;

			_8C = MR::max(var_f2, temp_f0);
			_90 = -(0.7f * temp_f1);
			MR::startSound(this, "SE_OJ_COCONUT_FLIP_S", -1, -1);
			setNerve(&NrvCocoNut::CocoNutNrvMove::sInstance);
			return true;
		}
	}
	else {
		if (MR::isMsgHitmarkEmit(arg0)) {
			return true;
		}
	}
	return false;
}

void CocoNut::initMapToolInfo(const JMapInfoIter &rIter) {
	MR::initDefaultPos(this, rIter);
	MR::getJMapInfoArg0NoInit(rIter, &mSphericalShadow);
	MR::getJMapInfoArg1NoInit(rIter, &mRespawnWhenOutOfView);
	MR::getJMapInfoArg2NoInit(rIter, &mContinueRolling);
	_D0 = 55.0f * mScale.x;
	mSpawnPosition.set<f32>(mPosition);
}

void CocoNut::statusToHide() {
	mVelocity.zero();
	MR::offBind(this);
	MR::offCalcGravity(this);
	MR::hideModel(this);
	MR::invalidateHitSensors(this);
	MR::clearHitSensors(this);
	MR::deleteEffectAll(this);
}

void CocoNut::emitEffectSpinHit(const HitSensor *pOtherSensor, const HitSensor *pMySensor) {
	TVec3f point; // point 70% of the way between pOtherSensor and pMySensor
	JMAVECLerp(&pOtherSensor->mPosition, &pMySensor->mPosition, &point, 0.7f);
	_108.mMtx[0][0] = 1.0f;
	_108.mMtx[1][0] = 0.0f;
	_108.mMtx[2][0] = 0.0f;
	_108.mMtx[0][1] = 0.0f;
	_108.mMtx[1][1] = 1.0f;
	_108.mMtx[2][1] = 0.0f;
	_108.mMtx[0][2] = 0.0f;
	_108.mMtx[1][3] = 0.0f;
	_108.mMtx[2][3] = 1.0f;
	_108.mMtx[0][3] = point.x;
	_108.mMtx[1][3] = point.y;
	_108.mMtx[2][3] = point.z;
	MR::emitEffect(this, "SpinHitMark");
}

bool CocoNut::isContactWithOtherCocoNut() const {
	HitSensor *body = getSensor("body");
	for (int i = 0; i < body->mSensorCount; i++) {
		HitSensor *sensor = body->mSensors[i];
		if (body->mSensors[i]->isType(0x17)) {
			return true;
		}
	}
	return false;
}

void CocoNut::exeWait() {
	if (MR::isFirstStep(this)) {
		MR::deleteEffect(this, "RollingSmoke");
	}
	if (!_D4) {
		if (tryDisappear()) {
			return;
		}
		updateGravity();
	}
	if (!_D4 && MR::isOnGround(this)) {
		statusToWait();
	}
}

void CocoNut::exeWaitOnBind() {
	if (MR::isFirstStep(this)) {
		MR::deleteEffect(this, "RollingSmoke");
	}
	if (!tryDisappear()) {
		updateGravity();
		if (MR::isOnGround(this)) {
			statusToWait();
		}
	}
}

void CocoNut::exeMove() {
	if (MR::isFirstStep(this)) {
		_138 = 0;
		_14C = false;
		_150.zero();
		MR::onBind(this);
		MR::onCalcGravity(this);
		MR::calcGravity(this);
		if (!_15E) {
			MR::invalidateClipping(this);
		}
	}
	if (_13C && MR::isStep(this, 1)) {
		MR::stopScene(4);
	}
	processMove();
	tryMoveEnd();
}

void CocoNut::exeInWater() {
	if (MR::isFirstStep(this)) {
		mVelocity.zero();
		MR::offBind(this);
		MR::offCalcGravity(this);
		MR::hideModel(this);
		MR::invalidateHitSensors(this);
		MR::clearHitSensors(this);
		MR::deleteEffectAll(this);
		TVec3f gravityNegated;
		negateInternalInline(mGravity, &gravityNegated);
		MR::makeMtxUpNoSupportPos(&_D8, gravityNegated, mPosition);
		MR::emitEffect(this, "WaterColumn");
		MR::startSound(this, "SE_OJ_FALL_IN_WATER_M", -1, -1);
		MR::releaseSoundHandle(this, "SE_OJ_FALL_IN_WATER_M");
	}
	if (!MR::isEffectValid(this, "WaterColumn")) {
		mPosition.set<f32>(mSpawnPosition);
		setNerve(&NrvCocoNut::CocoNutNrvReplaceReady::sInstance);
	}
}

void CocoNut::exeBreak() {
	if (MR::isFirstStep(this)) {
		statusToHide();
		TVec3f gravityNegated;
		negateInternalInline(mGravity, &gravityNegated);
		MR::makeMtxUpNoSupportPos(&_D8, gravityNegated, mPosition);
		MR::emitEffect(this, getBreakEffectName());
	}
	if (!MR::isEffectValid(this, getBreakEffectName())) {
		mPosition.set<f32>(mSpawnPosition);
		setNerve(&NrvCocoNut::CocoNutNrvReplaceReady::sInstance);
	}
}

namespace NrvCocoNut {
	INIT_NERVE(CocoNutNrvWait)
	INIT_NERVE(CocoNutNrvWaitOnBind)
	INIT_NERVE(CocoNutNrvMove)
	INIT_NERVE(CocoNutNrvInWater)
	INIT_NERVE(CocoNutNrvBreak)
	INIT_NERVE(CocoNutNrvReplaceReady)

	void CocoNutNrvReplaceReady::execute(Spine *pSpine) const {
		CocoNut *nut = reinterpret_cast<CocoNut*>(pSpine->mExecutor);
		if (MR::isFirstStep(nut)) {
			MR::validateClipping(nut);
		}
	}
	void CocoNutNrvBreak::execute(Spine *pSpine) const {
		CocoNut *nut = reinterpret_cast<CocoNut*>(pSpine->mExecutor);
		nut->exeBreak();
	}
	void CocoNutNrvInWater::execute(Spine *pSpine) const {
		CocoNut *nut = reinterpret_cast<CocoNut*>(pSpine->mExecutor);
		nut->exeInWater();
	}
	void CocoNutNrvMove::execute(Spine *pSpine) const {
		CocoNut *nut = reinterpret_cast<CocoNut*>(pSpine->mExecutor);
		nut->exeMove();
	}
	void CocoNutNrvWaitOnBind::execute(Spine *pSpine) const {
		CocoNut *nut = reinterpret_cast<CocoNut*>(pSpine->mExecutor);
		nut->exeWaitOnBind();
	}
	void CocoNutNrvWait::execute(Spine *pSpine) const {
		CocoNut *nut = reinterpret_cast<CocoNut*>(pSpine->mExecutor);
		nut->exeWait();
	}
}
