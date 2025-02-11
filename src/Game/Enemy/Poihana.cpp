#include "Game/Enemy/Poihana.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace {
	const Vec sNormalBinderPos = { 0.0f, 130.0f, 120.0f };
	const Vec sTrampleBinderPos = { 0.0f, 150.0f, 0.0f };
};

Poihana::Poihana(const char *pName) : LiveActor(pName) {
	mAnimScaleCtrl = nullptr;
	mBindStarPointer = nullptr;
	mCamInfo = nullptr;
	mBindedActor = nullptr;
	mLaunchIntensity = 0.0f;
	mRespawnPos.set(0.0f);
	_AC = 0.0f;
	_B0 = 0.0f;
	mHomePos.set(0.0f);
	_C0 = 1.0f;
	mBoundTimer = -1;
	mBehavior = POIHANA_BEHAVIOR_NORMAL;
	mCanDrown = false;
	mWaterColumn = nullptr;
	mIsActive = false;
	_E5 = 0;
}

/*void Poihana::init(const JMapInfoIter &rIter) {
	MR::initDefaultPos(this, rIter);
	MR::initActorCamera(this, rIter, &mCamInfo);
	initModelManagerWithAnm("Poihana", nullptr, false);

	MR::connectToSceneEnemy(this);

	// Initialize sensors
	initHitSensor(2);
	MR::addHitSensorPriorBinder(this, "binder", 8, 125.0f, sNormalBinderPos);
	MR::addHitSensorAtJoint(this, "body", "Body", 30, 8, 70.0f, TVec3f(0.0f, 0.0f, 0.0f));

	// Initialize binder
	bool useSmallBinder = false;
	MR::getJMapInfoArg7NoInit(rIter, &useSmallBinder);

	if (useSmallBinder) {
		initBinder(100.0f, 100.0f, 0);
	}
	else {
		initBinder(150.0f, 150.0f, 0);
	}

	mIsActive = true;
	MR::setGroupClipping(this, rIter, 0x20);
	initEffectKeeper(1, nullptr, false);
	initSound(4, false);

	// Initialize 2P behavior
	MR::initStarPointerTarget(this, 150.0f, TVec3f(0.0f, 100.0f, 0.0f));
	mAnimScaleCtrl = new AnimScaleController(nullptr);
	mBindStarPointer = new WalkerStateBindStarPointer(this, mAnimScaleCtrl);

	MR::initShadowVolumeSphere(this, 80.0f);
	MR::addToAttributeGroupSearchTurtle(this);

	mRespawnPos.set(mPosition);
	mHomePos.set(mRespawnPos);

	// Calculate launch intensity, thanks to Shibbo for helping me on that one
	s32 launchIntensity = 1000;
	MR::getJMapInfoArg0NoInit(rIter, &launchIntensity);
	float launchIntensityF = 2.0f * launchIntensity;

	if (launchIntensityF > 0) {
		f32 factor;
		f32 reciprocal = __frsqrte(launchIntensityF);
		factor = reciprocal * launchIntensityF;
		launchIntensityF = -(factor * reciprocal - 3.0f) * factor * 0.5f;
	}

	mLaunchIntensity = -launchIntensityF;

	// Setup color
	//s32 color = 0;
	//MR::getJMapInfoArg2NoInit(rIter, &color);
	//MR::startBtpAndSetFrameAndStop(this, "ColorChange", color);

	// Setup behaviors
	//MR::getJMapInfoArg1NoInit(rIter, &mActiveRange);
	MR::getJMapInfoArg3NoInit(rIter, &mBehavior);
	MR::getJMapInfoArg4NoInit(rIter, &mCanDrown);

	if (mCanDrown) {
		mWaterColumn = MR::createModelObjMapObj("エフェクト水柱", "WaterColumn", (MtxPtr)getBaseMtx());
		mWaterColumn->mScale.setAll(2.0f);
	}

	MR::tryCreateMirrorActor(this, nullptr);
	//MR::useStageSwitchAwake(this, rIter);

	if (mBehavior == POIHANA_BEHAVIOR_SLEEP) {
		initNerve(&NrvPoihana::PoihanaNrvSleep::sInstance);
	}
	else {
		initNerve(&NrvPoihana::PoihanaNrvWait::sInstance);
	}

	makeActorAppeared();
}*/

void Poihana::initAfterPlacement() {
	TVec3f gravityNegated;
	JGeometry::negateInternal((f32 *)&mGravity, (f32 *)&gravityNegated);
	TPos3f baseMtx;
	MR::makeMtxUpNoSupportPos(&baseMtx, gravityNegated, mPosition);
	MR::setBaseTRMtx(this, baseMtx);
	MR::calcFrontVec(&mFrontVec, this);
	MR::trySetMoveLimitCollision(this);
}

void Poihana::control() {
	if (!isNerve(&NrvPoihana::PoihanaNrvHide::sInstance)) {
		mAnimScaleCtrl->updateNerve();
		calcBound();
		controlVelocity();
		calcMyGravity();

		if (!tryDrown() && !tryDPDSwoon() && tryHipDropShock()) {
			return;
		}
	}
}

/*
void Poihana::calcAndSetBaseMtx() {
	TPos3f baseMtx;
	MR::calcMtxFromGravityAndZAxis(&baseMtx, this, mGravity, mFrontVec);

	if (isNerveTypeWalkOrWait()) {
		MR::blendMtx((MtxPtr)getBaseMtx(), (MtxPtr)&baseMtx, 0.3f, (MtxPtr)&baseMtx);
	}

	MR::setBaseTRMtx(this, baseMtx);

	TVec3f newScale;
	newScale.multPS(mScale, mAnimScaleCtrl->_C);
	MR::setBaseScale(this, newScale);
}
*/

// This inline function might be used elsewhere too? It seems unusual for it to be used once
inline void calcRepelVector(const TVec3f &agent, const TVec3f &object, TVec3f& dst) {
	JMAVECScaleAdd(&agent, &object, &dst, -agent.dot(object));
}

void Poihana::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
	bool ret = false;

	if (MR::isSensorPlayer(pReceiver) || MR::isSensorEnemy(pReceiver) ||
	    MR::isSensorMapObj(pReceiver)) {
		ret = true;
	}

	if (!ret) {
		return;
	}

	if (MR::isSensorEnemy(pSender)) {
		if (MR::isSensorPlayer(pReceiver)) {
			contactMario(pSender, pReceiver);
		}

		if (MR::sendMsgPush(pReceiver, pSender)) {
			if (MR::isSensorPlayer(pReceiver)) {
				ret = false;
				if (isNerve(&NrvPoihana::PoihanaNrvShock::sInstance) ||
				    isNerve(&NrvPoihana::PoihanaNrvSwoonLand::sInstance) ||
				    isNerve(&NrvPoihana::PoihanaNrvSwoon::sInstance) ||
				    isNerve(&NrvPoihana::PoihanaNrvRecover::sInstance)) {
					ret = true;
				}

				if (ret) {
					return;
				}
			}

			TVec3f pushVelocity(mPosition - pReceiver->mActor->mPosition);
			MR::normalizeOrZero(&pushVelocity);

			if (mVelocity.dot(pushVelocity) < 0.0f) {
				const TVec3f& velocity = mVelocity;
				calcRepelVector(pushVelocity, velocity, mVelocity);
			}
		}
	}
}

bool Poihana::receiveMsgPush(HitSensor *pSender, HitSensor *pReceiver) {
	if (MR::isSensorEnemy(pSender) || MR::isSensorMapObj(pSender)) {
		TVec3f pushOffset(mPosition - pSender->mActor->mPosition);
		MR::normalizeOrZero(&pushOffset);
		JMAVECScaleAdd(&pushOffset, &mVelocity, &mVelocity, 1.5f);

		return true;
	}

	return false;
}

bool Poihana::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (MR::isMsgStarPieceAttack(msg)) {
		return true;
	}

	if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
		bool flag = false;

		if (isNerve(&NrvPoihana::PoihanaNrvDrown::sInstance) ||
		    isNerve(&NrvPoihana::PoihanaNrvHide::sInstance) ||
		    isNerve(&NrvPoihana::PoihanaNrvAppear::sInstance) ||
		    isNerve(&NrvPoihana::PoihanaNrvShock::sInstance)) {
			flag = true;
		}

		if (flag) {
			goto here;
		}

		flag = false;
		if (isNerve(&NrvPoihana::PoihanaNrvSleepStart::sInstance) ||
		    isNerve(&NrvPoihana::PoihanaNrvSleep::sInstance) ||
		    isNerve(&NrvPoihana::PoihanaNrvGetUp::sInstance)) {
			flag = true;
		}

		if (flag) {
here:
			startBound();
			MR::startSound(this, "SE_EV_POIHANA_TRAMPLE", -1, -1);

			if (MR::isMsgPlayerHipDrop(msg)) {
				MR::sendMsgAwayJump(pSender, pReceiver);
			}

			return true;
		}

		MR::setSensorOffset(this, "binder", sTrampleBinderPos);
		MR::setSensorRadius(this, "binder", 125.0f);
		MR::validateHitSensor(this, "binder");
	}

	if (MR::isMsgPlayerHitAll(msg) && tryShock()) {
		MR::stopSceneForDefaultHit(3);
		return true;
	}

	return false;
}

bool Poihana::receiveMsgEnemyAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (MR::isMsgExplosionAttack(msg) && tryShock()) {
		return true;
	}

	return false;
}

/*bool Poihana::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
	if (MR::isMsgAutoRushBegin(msg) && MR::isSensorPlayer(pSender)) {
		if (isNerve(&NrvPoihana::PoihanaNrvShootUpCharge::sInstance)) {
			return false;
		}
		else if (MR::isOnGroundPlayer()) {
			if (isNerve(&NrvPoihana::PoihanaNrvShootUp::sInstance)) {
				if (getNerveStep() < 30) {
					if (!tryToStartBind(pSender)) {
						return false;
					}
				}
			}
			else if (isNerve(&NrvPoihana::PoihanaNrvShootUpCharge::sInstance) ||
			         !MR::isNear(pSender, pReceiver, 100.0f)) {
				return false;
			}
			else {
				setNerve(&NrvPoihana::PoihanaNrvShootUpCharge::sInstance);
			}
		}
		else if (tryToStartBind(pSender)) {
			setNerve(&NrvPoihana::PoihanaNrvShootUp::sInstance);
			return true;
		}
	}
	else if (MR::isMsgUpdateBaseMtx(msg) && mBindedActor) {
		updateBindActorMtx();
		return true;
	}

	return false;
}*/

void Poihana::exeNonActive() {
	if (MR::isFirstStep(this)) {
		mVelocity.zero();
		MR::offBind(this);
		MR::offCalcShadow(this, nullptr);
		MR::offCalcAnim(this);

		mIsActive = false;

		MR::invalidateHitSensors(this);
	}

	if (MR::isNearPlayerAnyTime(this, 3500.0f)) {
		if (mBehavior == POIHANA_BEHAVIOR_SLEEP) {
			setNerve(&NrvPoihana::PoihanaNrvSleep::sInstance);
		}
		else {
			setNerve(&NrvPoihana::PoihanaNrvWait::sInstance);
		}
	}
}

void Poihana::endNonActive() {
	MR::onBind(this);
	MR::onCalcShadow(this, nullptr);
	MR::onCalcAnim(this);

	mIsActive = true;

	MR::setSensorOffset(this, "binder", sNormalBinderPos);
	MR::setSensorRadius(this, "binder", 125.0f);
	MR::validateHitSensor(this, "binder");
	MR::validateHitSensors(this);
}

void Poihana::exeWait() {
	if (MR::isFirstStep(this)) {
		MR::startBckNoInterpole(this, "Wait");
		MR::setSensorOffset(this, "binder", sNormalBinderPos);
		MR::setSensorRadius(this, "binder", 125.0f);
		MR::validateHitSensor(this, "binder");
	}

	if (MR::isNearPlayer(this, 800.0f)) {
		setNerve(&NrvPoihana::PoihanaNrvSearch::sInstance);
	}
	else if (MR::isGreaterStep(this, 180)) {
		setNerve(&NrvPoihana::PoihanaNrvWalkAround::sInstance);
	}
	else {
		tryNonActive();
	}
}

void Poihana::exeWalkAround() {
	if (MR::isFirstStep(this)) {
		if (!MR::isBckPlaying(this, "Walk")) {
			MR::startBck(this, "Walk", nullptr);
		}

		mRandDir = MR::getRandom((s32)-2, (s32)2);
	}

	MR::rotateVecDegree(&mFrontVec, mGravity, mRandDir);
	JMAVECScaleAdd(&mFrontVec, &mVelocity, &mVelocity, 0.5f);

	if (isNeedForBackHome()) {
		setNerve(&NrvPoihana::PoihanaNrvGoBack::sInstance);
	}

	else if (MR::isNearPlayer(this, 800.0f)) {
		setNerve(&NrvPoihana::PoihanaNrvSearch::sInstance);
	}

	else if (MR::isGreaterStep(this, 120)) {
		if (mBehavior == POIHANA_BEHAVIOR_SLEEP) {
			setNerve(&NrvPoihana::PoihanaNrvSleepStart::sInstance);
		}
		else {
			setNerve(&NrvPoihana::PoihanaNrvWait::sInstance);
		}
	}
}

void Poihana::exeSleepStart() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "SleepStart", nullptr);
		MR::startSound(this, "SE_EV_POIHANA_SLEEP_START", -1, -1);
		MR::invalidateHitSensor(this, "binder");
	}

	if (MR::isBckStopped(this)) {
		setNerve(&NrvPoihana::PoihanaNrvSleep::sInstance);
	}
}

void Poihana::exeSleep() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Sleep", nullptr);
		MR::invalidateHitSensor(this, "binder");
	}

	if (isNeedForGetUp()) {
		setNerve(&NrvPoihana::PoihanaNrvGetUp::sInstance);
	}
	else {
		tryNonActive();
	}
}

void Poihana::exeGetUp() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "GetUp", nullptr);
		MR::startSound(this, "SE_EV_POIHANA_WAKEUP", -1, -1);
		MR::startSound(this, "SE_EM_POIHANA_WAKEUP", -1, -1);
	}

	if (MR::isBckStopped(this)) {
		MR::setSensorOffset(this, "binder", sNormalBinderPos);
		MR::setSensorRadius(this, "binder", 125.0f);
		MR::validateHitSensor(this, "binder");
		setNerve(&NrvPoihana::PoihanaNrvWait::sInstance);
	}
}

void Poihana::exeSearch() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Search", nullptr);
		MR::startSound(this, "SE_EV_POIHANA_FIND", -1, -1);
	}

	MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, *MR::getPlayerPos(), 4.0f);

	if (MR::isBckStopped(this)) {
		setNerve(&NrvPoihana::PoihanaNrvChasePlayer::sInstance);
	}
}

void Poihana::exeChasePlayer() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Run", nullptr);
		MR::setSensorOffset(this, "binder", sNormalBinderPos);
		MR::setSensorRadius(this, "binder", 125.0f);
		MR::validateHitSensor(this, "binder");
	}

	MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, *MR::getPlayerPos(), 4.0f);
	JMAVECScaleAdd(&mFrontVec, &mVelocity, &mVelocity, 0.5f);

	if (isNeedForBackHome()) {
		setNerve(&NrvPoihana::PoihanaNrvGoBack::sInstance);
	}
}

void Poihana::exeShootUpCharge() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "ThrowStart", nullptr);
	}

	TVec3f& gravity = mGravity;
	f32 dot = gravity.dot(mVelocity);
	mVelocity.scale(dot, gravity);

	if (MR::isBckStopped(this)) {
		setNerve(&NrvPoihana::PoihanaNrvShootUp::sInstance);
	}
}

/*void Poihana::exeShootUp() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Throw", nullptr);
		MR::startSound(this, "SE_EV_POIHANA_SHOOT_UP", -1, -1);
		MR::startActorCameraNoTarget(this, mCamInfo, -1);
	}

	f32 dot = mGravity.dot(mVelocity);
	mVelocity.scale(dot, mGravity);

	if (MR::isStep(this, 2)) {
		endBind();
		MR::invalidateHitSensor(this, "binder");
	}

	if (MR::isStep(this, 30)) {
		MR::setSensorOffset(this, "binder", sNormalBinderPos);
		MR::setSensorRadius(this, "binder", 125.0f);
		MR::validateHitSensor(this, "binder");
	}

	if (MR::isBckStopped(this)) {
		if (mBehavior == POIHANA_BEHAVIOR_NEW_HOME) {
			mHomePos.set(mPosition);
			setNerve(&NrvPoihana::PoihanaNrvWalkAround::sInstance);
		}
		else {
			setNerve(&NrvPoihana::PoihanaNrvGoBack::sInstance);
		}
	}
}*/

void Poihana::endShootUp() {
	endBind();

	MR::setSensorOffset(this, "binder", sNormalBinderPos);
	MR::setSensorRadius(this, "binder", 125.0f);
	MR::validateHitSensor(this, "binder");

	MR::endActorCamera(this, mCamInfo, true, -1);
}

void Poihana::exeGoBack() {
	if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Walk")) {
		MR::startBck(this, "Walk", nullptr);
	}

	MR::turnDirectionToTargetUseGroundNormalDegree(this, &mFrontVec, mHomePos, 2.0f);
	JMAVECScaleAdd(&mFrontVec, &mVelocity, &mVelocity, 0.5f);

	if (MR::isNearPlayer(this, 800.0f) && MR::isGreaterStep(this, 120)) {
		setNerve(&NrvPoihana::PoihanaNrvSearch::sInstance);
	}
	else if (MR::isNear(this, mHomePos, 100.0f)) {
		setNerve(&NrvPoihana::PoihanaNrvWait::sInstance);
	}
}

void Poihana::exeShock() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "PunchDamage", nullptr);
		MR::startBlowHitSound(this);
		MR::invalidateHitSensor(this, "binder");
	}

	if (MR::isOnGround(this) && MR::isGreaterStep(this, 12)) {
		setNerve(&NrvPoihana::PoihanaNrvSwoon::sInstance);
	}
}

void Poihana::exeSwoonLand() {
	if (MR::isFirstStep(this)) {
		MR::startBckNoInterpole(this, "SwoonLand");
		MR::startSound(this, "SE_EV_POIHANA_SWOON", -1, -1);
	}

	if (MR::isBckStopped(this)) {
		setNerve(&NrvPoihana::PoihanaNrvSwoon::sInstance);
	}
}

void Poihana::exeSwoon() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Swoon", nullptr);
	}

	MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);

	if (MR::isStep(this, 110)) {
		setNerve(&NrvPoihana::PoihanaNrvRecover::sInstance);
	}
}

void Poihana::exeRecover() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Recover", nullptr);
		MR::startSound(this, "SE_EM_POIHANA_RECOVER", -1, -1);
		MR::startSound(this, "SE_EV_POIHANA_RECOVER", -1, -1);
		mScale.set(1.0f);
	}

	if (MR::isBckStopped(this)) {
		MR::setSensorOffset(this, "binder", sNormalBinderPos);
		MR::setSensorRadius(this, "binder", 125.0f);
		MR::validateHitSensor(this, "binder");
		setNerve(&NrvPoihana::PoihanaNrvWait::sInstance);
	}
}

// Needs to be reviewed
void Poihana::exeShake() {
	f32 _f31 = 0.2f - 0.01f * getNerveStep();
	f32 scale = MR::sinDegree(getNerveStep() * 0.01745329251f) * _f31 + 36.0f;
	mScale.set(scale);

	if (MR::isStep(this, 20)) {
		mScale.set(1.0f);
		setNerve(&NrvPoihana::PoihanaNrvSearch::sInstance);
	}
}

void Poihana::exeDrown() {
	if (MR::isFirstStep(this)) {
		MR::startBck(this, "Drown", nullptr);
		MR::invalidateClipping(this);
		MR::invalidateHitSensors(this);
		MR::startSound(this, "SE_EM_FALL_INTO_WATER_S", -1, -1);

		mWaterColumn->appear();
		MR::tryStartAllAnim(mWaterColumn, "Splash");
	}

	if (MR::isBckStopped(this)) {
		setNerve(&NrvPoihana::PoihanaNrvHide::sInstance);
	}
}

void Poihana::exeHide() {
	if (MR::isFirstStep(this)) {
		MR::startSound(this, "SE_EM_EXPLODE_S_WATER", -1, -1);
		MR::emitEffect(this, "DeathWater");
		MR::hideModel(this);
	}

	if (MR::isStep(this, 150)) {
		setNerve(&NrvPoihana::PoihanaNrvAppear::sInstance);
	}
}

void Poihana::exeAppear() {
	if (MR::isFirstStep(this)) {
		MR::resetPosition(this, mRespawnPos);
		MR::emitEffect(this, "Appear");
	}

	if (MR::isLessStep(this, 60)) {
		MR::startLevelSound(this, "SE_EM_LV_POIHANA_REVIVE_EFFECT", -1, -1, -1);
	}

	if (MR::isStep(this, 60)) {
		MR::showModel(this);
		MR::startBck(this, "Appear", nullptr);
		MR::startSound(this, "SE_EM_POIHANA_REVIVE_APPEAR", -1, -1);
	}
	else if (MR::isBckStopped(this) && MR::isGreaterStep(this, 60)) {
		MR::validateClipping(this);
		MR::validateHitSensors(this);
		MR::setSensorOffset(this, "binder", sNormalBinderPos);
		MR::setSensorRadius(this, "binder", 125.0f);
		MR::validateHitSensor(this, "binder");
		setNerve(&NrvPoihana::PoihanaNrvWait::sInstance);
	}
}

void Poihana::exeDPDSwoon() {
	if (MR::isFirstStep(this)) {
		MR::invalidateHitSensor(this, "Binder");
	}

	MR::updateActorStateAndNextNerve(this, (ActorStateBaseInterface*)mBindStarPointer,
	                                 &NrvPoihana::PoihanaNrvWait::sInstance);
}

void Poihana::endDPDSwoon() {
	mBindStarPointer->kill();

	MR::setSensorOffset(this, "binder", sNormalBinderPos);
	MR::setSensorRadius(this, "binder", 125.0f);
	MR::validateHitSensor(this, "binder");
}

bool Poihana::tryToStartBind(HitSensor* pSender) {
	if (mBindedActor != nullptr) {
		return false;
	}

	LiveActor *bindedActor = pSender->mActor;
	if (MR::isInWater(bindedActor, TVec3f(0.0f, 0.0f, 0.0f))) {
		return false;
	}

	MR::tryRumblePadMiddle(this, 0);
	mBindedActor = pSender->mActor;
	MR::startBckPlayer("Rise", (const char *)nullptr);
	MR::invalidateClipping(this);
	return true;
}

void Poihana::updateBindActorMtx() {
	TPos3f binderMtx;
	MR::makeMtxTR(binderMtx.toMtxPtr(), mBindedActor);
	MR::setBaseTRMtx(mBindedActor, binderMtx);
}

void Poihana::endBind() {
	MR::validateClipping(this);

	if (MR::isPlayerInRush()) {
		TVec3f jumpVec;
		jumpVec.scale(-mLaunchIntensity, mGravity);
		MR::endBindAndPlayerJump(this, jumpVec, 0);
		MR::startSound(this, "SE_PM_HELI_JUMP", -1, -1);
	}

	mBindedActor = nullptr;
}

void Poihana::startBound() {
	mBoundTimer = 0;
	mScale.set(1.0f);
}

/*
* This function calculates Poihana's scale for 40 frames after being trampled. This is
* used to simulate the "vibrating" visual effect. This is not 1:1 the same as in SMG1,
* but it looks VERY similar and appears to be even more efficient compared to SMG1's
* unusually complicated calculations.
*/
/*void Poihana::calcBound() {
	if (mBoundTimer != -1) {
		f32 scale = 1.0f;

		if (mBoundTimer < 40) {
			scale = 0.05f * sin(0.393f * mBoundTimer) + 1.0f;
			mBoundTimer++;
		}
		else {
			mBoundTimer = -1;
		}

		mScale.setAll(scale);
	}
}*/

void Poihana::contactMario(HitSensor *pSender, HitSensor *pReceiver) {
	bool isShooting;

	if (!isNerveTypeWalkOrWait()) {
		isShooting = false;
		if (isNerve(&NrvPoihana::PoihanaNrvShootUpCharge::sInstance) ||
		    isNerve(&NrvPoihana::PoihanaNrvShootUp::sInstance)) {
			isShooting = true;
		}
	} else {
		goto doFlip;
	}

	if (isShooting) {
doFlip:
		if (!isBackAttack(pReceiver)) {
			return;
		}

		if (MR::sendMsgEnemyAttackFlipWeak(pReceiver, pSender)) {
			setNerve(&NrvPoihana::PoihanaNrvShake::sInstance);
		}
	} else if (isNerve(&NrvPoihana::PoihanaNrvSleep::sInstance)) {
			setNerve(&NrvPoihana::PoihanaNrvGetUp::sInstance);
	}
}

// Needs review
/*void Poihana::controlVelocity() {
	if (!mIsActive) {
		return;
	}

	// Calculate front vector
	TVec3f gravity;
	if (MR::isBindedGround(this)) {
		gravity.set(-*MR::getGroundNormal(this));
	}
	else {
		gravity.set(mGravity);
	}

	TVec3f frontVec(mFrontVec);
	MR::turnVecToPlane(&mFrontVec, frontVec, gravity);

	// Calculate velocity
	if (MR::isBindedGround(this)) {
		f32 dot = mFrontVec.dot(mVelocity) * -1.0f;

		TVec3f addVel;
		JMAVECScaleAdd(mFrontVec, mVelocity, addVel, dot);
		addVel.scale(0.8f);

		mVelocity.scale(mFrontVec.dot(mVelocity), mFrontVec);
		mVelocity.add(addVel);

		if (mVelocity.dot(gravity) > 0.0f) {
			dot = gravity.dot(mVelocity) * -1.0f;
			JMAVECScaleAdd(gravity, mVelocity, mVelocity, dot);
		}

		mVelocity.scale(0.95f);
	}

	JMAVECScaleAdd(gravity, mVelocity, mVelocity, 2.0f);

	if (!isNerve(&NrvPoihana::PoihanaNrvShock::sInstance)) {
		f32 magVel = isNerve(&NrvPoihana::PoihanaNrvChasePlayer::sInstance) ? 10.0f : 5.0f;

		if (PSVECMag((Vec *)&mVelocity) > magVel) {
			f32 squared = mVelocity.squared();

			if (squared > 0.0000038146973f) {
				mVelocity.scale(JGeometry::TUtil<f32>::inv_sqrt(squared));
			}
		}

		if (MR::isNearZero(mVelocity, 0.001f)) {
			mVelocity.zero();
		}
	}
}*/

void Poihana::calcMyGravity() {
	if (!mIsActive) {
		return;
	}

	TVec3f upVec, gravityPos;
	MR::calcUpVec(&upVec, this);
	JMAVECScaleAdd(&upVec, &mPosition, &gravityPos, 20.0f);
	MR::calcGravity(this, gravityPos);
}

bool Poihana::tryNonActive() {
	if (!MR::isStageStateScenarioOpeningCamera()) {
		bool notNear = !MR::isNearPlayerAnyTime(this, 3500.0f);
		if (notNear && MR::isBindedGround(this)) {
			setNerve(&NrvPoihana::PoihanaNrvNonActive::sInstance);
			return true;
		}
	}

	return false;
}

bool Poihana::tryDrown() {
	if (!mCanDrown) {
		return false;
	}

	if (isNerve(&NrvPoihana::PoihanaNrvAppear::sInstance)) {
		return false;
	}

	if (isNerve(&NrvPoihana::PoihanaNrvDrown::sInstance)) {
		return false;
	}

	if (!MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
		return false;
	}

	setNerve(&NrvPoihana::PoihanaNrvDrown::sInstance);
	return true;
}

bool Poihana::tryDPDSwoon() {
	if (!isNerveTypeWalkOrWait()) {
		return false;
	}

	if (!mBindStarPointer->tryStartPointBind()) {
		return false;
	}

	setNerve(&NrvPoihana::PoihanaNrvDPDSwoon::sInstance);
	return true;
}

bool Poihana::tryShock() {
	bool ret = false;

	if (isNerve(&NrvPoihana::PoihanaNrvDrown::sInstance) ||
	    isNerve(&NrvPoihana::PoihanaNrvHide::sInstance) ||
	    isNerve(&NrvPoihana::PoihanaNrvAppear::sInstance)) {
		ret = true;
	}

	if (ret) {
		return false;
	}

	if (isNerve(&NrvPoihana::PoihanaNrvShock::sInstance)) {
		return false;
	}

	if (isNerve(&NrvPoihana::PoihanaNrvSwoonLand::sInstance)) {
		return false;
	}

	mVelocity.scale(-30.0f, mGravity);
	setNerve(&NrvPoihana::PoihanaNrvShock::sInstance);
	return true;
}

bool Poihana::tryHipDropShock() {
	bool isNotNear = !MR::isNearPlayerAnyTime(this, 500.0f);

	if (isNotNear) {
		return false;
	}

	if (MR::isPlayerHipDropLand()) {
		return tryShock();
	}

	return false;
}

bool Poihana::isNerveTypeWalkOrWait() const {
	return (isNerve(&NrvPoihana::PoihanaNrvWait::sInstance) ||
	        isNerve(&NrvPoihana::PoihanaNrvSearch::sInstance) ||
	        isNerve(&NrvPoihana::PoihanaNrvWalkAround::sInstance) ||
	        isNerve(&NrvPoihana::PoihanaNrvChasePlayer::sInstance) ||
	        isNerve(&NrvPoihana::PoihanaNrvGoBack::sInstance));
}

bool Poihana::isNeedForBackHome() const {
	if (isNerve(&NrvPoihana::PoihanaNrvWalkAround::sInstance)) {
		return !MR::isNear(this, mHomePos, 350.0f);
	}

	if (!isNerve(&NrvPoihana::PoihanaNrvChasePlayer::sInstance)) {
		bool ret;
		if (mBehavior == POIHANA_BEHAVIOR_NEW_HOME) {
			ret = !MR::isNearPlayer(this, 1100.0f);
		} else {
			ret = false;
			if (!MR::isNear(this, mHomePos, 2000.0f) || !MR::isNearPlayer(this, 1110.0f)) {
				ret = true;
			}
		}

		return ret;
	}

	return false;
}

bool Poihana::isNeedForGetUp() const {
	bool ret = false;

	if (MR::isNearPlayer(this, 500.0f)) {
		bool flag = true;

		f32 mag = PSVECMag((Vec *)MR::getPlayerVelocity());

		if (!(mag >= 10.0f) && !MR::isPlayerSwingAction()) {
			flag = false;
		}

		if (flag) {
			ret = true;
		}
	}

	return ret;
}

bool Poihana::isBackAttack(HitSensor *pMySensor) const {
	TVec3f frontVec;
	MR::calcFrontVec(&frontVec, this);
	JGeometry::negateInternal((f32 *)&frontVec, (f32 *)&frontVec);

	TVec3f sensorRelative(pMySensor->mPosition - mPosition);
	return sensorRelative.dot(frontVec) > 0.0f;
}

namespace NrvPoihana {
	INIT_NERVE(PoihanaNrvNonActive);
	INIT_NERVE(PoihanaNrvWait);
	INIT_NERVE(PoihanaNrvWalkAround);
	INIT_NERVE(PoihanaNrvSleepStart);
	INIT_NERVE(PoihanaNrvSleep);
	INIT_NERVE(PoihanaNrvGetUp);
	INIT_NERVE(PoihanaNrvSearch);
	INIT_NERVE(PoihanaNrvChasePlayer);
	INIT_NERVE(PoihanaNrvShootUpCharge);
	INIT_NERVE(PoihanaNrvShootUp);
	INIT_NERVE(PoihanaNrvGoBack);
	INIT_NERVE(PoihanaNrvShock);
	INIT_NERVE(PoihanaNrvSwoon);
	INIT_NERVE(PoihanaNrvSwoonLand);
	INIT_NERVE(PoihanaNrvRecover);
	INIT_NERVE(PoihanaNrvShake);
	INIT_NERVE(PoihanaNrvDrown);
	INIT_NERVE(PoihanaNrvHide);
	INIT_NERVE(PoihanaNrvAppear);
	INIT_NERVE(PoihanaNrvDPDSwoon);

	void PoihanaNrvNonActive::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeNonActive();
	}

	void PoihanaNrvNonActive::executeOnEnd(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->endNonActive();
	}

	void PoihanaNrvWait::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeWait();
	}

	void PoihanaNrvWalkAround::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeWalkAround();
	}

	void PoihanaNrvSleepStart::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeSleepStart();
	}

	void PoihanaNrvSleep::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeSleep();
	}

	void PoihanaNrvGetUp::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeGetUp();
	}

	void PoihanaNrvSearch::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeSearch();
	}

	void PoihanaNrvChasePlayer::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeChasePlayer();
	}

	void PoihanaNrvShootUpCharge::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeShootUpCharge();
	}

	void PoihanaNrvShootUp::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeShootUp();
	}

	void PoihanaNrvShootUp::executeOnEnd(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->endShootUp();
	}

	void PoihanaNrvGoBack::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeGoBack();
	}

	void PoihanaNrvShock::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeShock();
	}

	void PoihanaNrvSwoon::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeSwoon();
	}

	void PoihanaNrvSwoonLand::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeSwoonLand();
	}

	void PoihanaNrvRecover::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeRecover();
	}

	void PoihanaNrvShake::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeShake();
	}

	void PoihanaNrvDrown::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeDrown();
	}

	void PoihanaNrvHide::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeHide();
	}

	void PoihanaNrvAppear::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeAppear();
	}

	void PoihanaNrvDPDSwoon::execute(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->exeDPDSwoon();
	}

	void PoihanaNrvDPDSwoon::executeOnEnd(Spine *pSpine) const {
		Poihana *pActor = (Poihana*)pSpine->mExecutor;
		pActor->endDPDSwoon();
	}
}
