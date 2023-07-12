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
#include "Game/Map/CollisionParts.h"
#include "Game/Player/MarioParts.h"
#include "Game/Map/WaterInfo.h"
#include "Game/Player/MarioSwim.h"
#include "Game/Screen/GameSceneLayoutHolder.h"
#include "Game/Player/RushEndInfo.h"
#include "Game/Player/MarioConst.h"
#include "Game/Player/MarioEffect.h"
#include "Game/Player/MarioAnimator.h"
#include "Game/Player/MarioShadow.h"
#include "Game/Player/MarioMessenger.h"

static bool gIsLuigi;

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

static float BASE_ROTATION = 0f;

void MarioActor::init(const JMapInfoIter &rInfo) {
	long initialAnimation = -1;
	if(!MR::isValidInfo(rInfo)) return;
	TVec3f position, rotation, scale;

	MR::getJMapInfoTrans(rInfo, &position);
	MR::getJMapInfoRotate(rInfo, &rotation);
	MR::getJMapInfoScale(rInfo, &scale);

	rotation.x = BASE_ROTATION + (f32)fmod(rotation.x - BASE_ROTATION + 360f, (double)360.0);
	rotation.y = BASE_ROTATION + (f32)fmod(rotation.y - BASE_ROTATION + 360f, (double)360.0);
	rotation.z = BASE_ROTATION + (f32)fmod(rotation.z - BASE_ROTATION + 360f, (double)360.0);

	if(MR::isExistJMapArg(rInfo)) {
		MR::getJMapInfoArg0NoInit(rInfo, &initialAnimation);
	}
	init2(position, rotation, initialAnimation);
}

void MarioActor::initAfterPlacement() {
	updateGravityVec(true, true);
	mMario -> _1D8 = _240;
	mMario -> _1F0 = -_240;
	mMario -> _1FC = -_240;
	_300 = mMario -> _1F0;
	_2d0 = _300;
	_2c4 = _240 % -70f;
	calcCenterPos();
	MR::updateHitSensorsAll(this);
	_360 = getGravityVector();
	_1c0 = true;
	_9f4 = getGravityVector();
	updateCameraInfo();
	calcBaseFrontVec(-_240);
}

void MarioActor::initAfterOpeningDemo() {
	mMario -> changeAnimationNonStop("ウォークイン");
	_37c = 0;
}

void MarioActor::calcBaseFrontVec(const TVec3f &rVec) {
	TVec3f cross, j(0f, 1f, 0f);
	f32 y = j.dot(rVec);
	if(y < -0.99f) {
		_258.setInline(0f, 0f, 1f);
	}
	else {
		f32 cosine;
		if(MR::makeAxisAndCosignVecToVec(&cross, &cosine, rVec, j)) {
			TVec3f k(0f, 0f, 1f);
			Mtx transformation;
			PSMTXRotAxisRad(transformation, cross.toCVec(), -JMAAcosRadian(cosine));
			PSMTXMultVecSR(transformation, k.toCVec(), _258.toVec());
			MR::normalize(&_258);
		}
		else {
			_258.setInline(0f, 0f, 1f);
		}
	}
}

void MarioActor::playSound(const char *pName, long duration) {
	mMario -> playSound(pName, duration);
}

void MarioActor::changeAnimation(const char *pName, const char *pNewName) {
	mMario -> changeAnimation(pName, pNewName);
}

void MarioActor::changeAnimationNonStop(const char *pName) {
	mMario -> changeAnimationNonStop(pName);
}

void MarioActor::changeAnimationUpper(const char *pName) {
	if(!mMario -> _71C) {
		if(isAnimationRun("基本")) {
			mMario -> changeAnimationUpper(pName, nullptr);
			return;
		}
	}
	mMario -> changeAnimation(pName, (const char *)nullptr);
}

void MarioActor::stopAnimation(const char *pName) {
	mMario -> stopAnimation(pName, (const char *)nullptr);
}

bool MarioActor::isAnimationRun(const char *pName) const {
	return mMario -> isAnimationRun(pName);
}

void MarioActor::changeNullAnimation(const char *pName, signed char num) {
	mNullAnimation -> appear();
	MR::startBck(mNullAnimation, pName, nullptr);
	_b92 = num;
}

bool MarioActor::isStopNullAnimation() const {
	if(!MR::isBckStopped(mNullAnimation)) {
		return MR::isDead(mNullAnimation);
	}
	return true;
}

void MarioActor::changeGameOverAnimation() {
	int animation = 0;
	if(mMario -> isStatusActive(0x12)) mMario -> closeStatus(nullptr);

	if(mMario -> isAnimationRun("前方小ダメージ")) animation = 0;
	if(mMario -> isAnimationRun("後方小ダメージ")) animation = 0;
	if(mMario -> isAnimationRun("ファイアラン前兆")) animation = 0;
	if(mMario -> isAnimationRun("炎のランナー")) animation = 0;
	if(mMario -> isAnimationRun("電気ダメージ")) animation = 1;
	if(mMario -> isAnimationRun("電気ダメージ終了")) animation = 1;
	if(mMario -> isAnimationRun("炎ダメージ")) animation = 2;
	if(mMario -> isAnimationRun("ファイアダンス")) animation = 2;
	if(mMario -> isAnimationRun("中ダメージ")) animation = 3;
	if(mMario -> isAnimationRun("中ダメージ空中")) animation = 3;
	if(mMario -> isAnimationRun("中ダメージ着地")) animation = 3;
	if(mMario -> isAnimationRun("中後ダメージ")) animation = 4;
	if(mMario -> isAnimationRun("中後ダメージ空中")) animation = 4;
	if(mMario -> isAnimationRun("中後ダメージ着地")) animation = 4;
	if(mMario -> isAnimationRun("落下")) animation = 5;
	if(mMario -> isAnimationRun("空中ふんばり")) animation = 5;
	if(mMario -> isAnimationRun("つぶれ")) animation = 6;
	if(mMario -> isAnimationRun("つぶれ復帰")) animation = 6;
	if(!mMario -> isAnimationRun("氷結") && !mMario -> isStatusActive(0xd));
	else animation = -1;
	if(mMario -> isSwimming()) animation = 7;
	if(isNerve(&NrvMarioActor::MarioActorNrvGameOverSink::sInstance)) animation = 8;
	if(MR::getKariKariClingNum() && mMario -> getMovementStates()._1) animation = 4;
	if(_39d == 2) animation = 9;
	if(_39d == 1) animation = 10;

	switch(animation) {
		case 0:
			mMario -> changeAnimationNonStop("座りダウン");
			break;
		case 1:
			mMario -> changeAnimationNonStop("感電ダウン");
			break;
		case 2:
			mMario -> changeAnimationNonStop("炎ダウン");
			break;
		case 3:
			mMario -> changeAnimationNonStop("仰向けダウン");
			break;
		case 4:
			mMario -> changeAnimationNonStop("俯せダウン");
			break;
		case 5:
			if(mMario -> getMovementStates()._1) mMario -> changeAnimationNonStop("座りダウン");
			else mMario -> changeAnimationNonStop("奈落ダウン");
			break;
		case 6:
			mMario -> changeAnimationNonStop("つぶれダウン");
			break;
		case 7:
			mMario -> changeAnimationNonStop("水泳ダウン");
			break;
		case 8:
			mMario -> changeAnimationNonStop("埋まりダウン");
			break;
		case 9:
			mMario -> changeAnimationNonStop("レース負け");
			break;
		case 10:
			mMario -> changeAnimationNonStop("水中レース負け");
			break;
	}
	_b90 = true;
	stopEffect("無敵中");
	_a6e = false;
}

XjointTransform* MarioActor::getJointCtrl(const char *pName) const {
	XanimeCore *pCore = mMarioAnim -> _c -> mCore;
	return pCore -> getJointTransform(MR::getJointIndex(this, pName));
}

bool MarioActor::isTurning() const {
	return getMovementStates()._3;
}
bool MarioActor::isDebugMode() const {
	return getMovementStates()._16;
}

void MarioActor::updateRotationInfo() {
	TRot3f stack_44;
	PSMTXConcat(getBaseMtx(), _e3c.toMtxPtr(), stack_44.toMtxPtr());
	MR::makeRTFromMtxPtr(nullptr, &mRotation, stack_44.toMtxPtr(), true);
	if(mRotation.z > 90f && mRotation.x > 90f) {
		f32 diff = 180f - mRotation.y;
		mRotation.z = 0f;
		mRotation.y = diff;
		mRotation.x = 0f;
	}
	stack_44.getEuler(_318);
	_318.scaleInline(57.2957763672f);
	stack_44.getEuler(_324);
	_324.scaleInline(57.2957763672f);
	if(MR::isSameDirection(_240, mMario -> _208, .01f)) _a18 = mRotation;
	else {
		TPos3f stack_14;
		TVec3f stack_8 = -_240;
		MR::makeMtxUpFront(&stack_14, stack_8, mMario -> _208);
		MR::makeRTFromMtxPtr(nullptr, &_a18, stack_14.toMtxPtr(), true);
		if(_a18.z > 90f && _a18.x > 90f) {
			f32 diff = 180f - _a18.y;
			_a18.z = 0f;
			_a18.y = diff;
			_a18.x = 0f;
		}
	}
}

void MarioActor::exeWait() {
	if(_fb8 != 0) {
		_fb8--;
		if(_fb8 == 0) {
			setNerve(_fb4);
			_fb4 = nullptr;
		}
	}
}

void MarioActor::movement() {
	_468l.y = 0;
	_378++;
	_1e1 = 0;
	PSMTXCopy(_ae0.toMtxPtr(), _ab0.toMtxPtr());
	PSMTXCopy(MR::getCameraViewMtx(), _ae0.toMtxPtr());
	updateCameraInfo();
	_4a8 = 0;
	_4ac = 0.785398185253f;
	LiveActor::movement();
	TVec3f stack_134(mPosition);
	stack_134 -= _294;
	TVec3f stack_128(stack_134);
	stack_128 -= mVelocity;
	_27c = stack_134;
	TVec3f stack_11c(_288);
	_288 = stack_128;
	if(MR::isOppositeDirection(_288, stack_11c, 0.01f)) {
		f32 mag_288 = PSVECMag(_288.toCVec());
		f32 magStack_11c = PSVECMag(stack_11c.toCVec());
		if (
			!MR::isNearZero(mag_288, 0.001f)
			&& !MR::isNearZero(magStack_11c, 0.001f)
			&& MR::isNearZero(mag_288 - magStack_11c, 1f)
		) {
			mPosition -= _288 % 0.5f;
		}
	}
	if(PSVECMag(stack_128.toCVec()) > 0.1f) {
		if(!(getMovementStates()._a)) {
			if(!MR::isNearZero(mVelocity, 0.001f)) {
				f32 diffMag = PSVECMag(_294.translateOpposite(_270).toCVec());
				f32 vMag = PSVECMag(mVelocity.toCVec());
				if(PSVECMag(stack_128.toCVec()) > 2f * (diffMag + vMag)) mMario -> stopWalk();
			}
		}
		if(getMovementStates()._23 && PSVECMag(mVelocity.toCVec()) < PSVECMag(stack_134.toCVec())) {
			if(stack_134.dot(getGravityVec()) < -0f) {
				TVec3f stack_110;
				MR::vecKillElement(mVelocity, getGravityVec(), &stack_110);
				if(MR::isNearZero(stack_110, 0.001f)) {
					MR::vecKillElement(stack_134, getGravityVec(), &stack_110);
				}
				stack_110.setLength(PSVECMag(stack_134.toCVec()));
				mMario -> push(stack_110);
				if(mMario -> _3BC <= 2) {
					f32 scale = PSVECMag(stack_128.toCVec());
					if(scale > 10f) scale = 10f;
					mMario -> _2D4 += -getGravityVec() % scale;
				}
			}
		}
		else if(getMovementStates()._23) {
			TVec3f stack_104(mMario -> _8F8);
			MR::normalizeOrZero(&stack_104);
			TVec3f stack_f8;
			f32 elementA = MR::vecKillElement(stack_134, stack_104, &stack_f8);
			f32 elementB = MR::vecKillElement(mVelocity, stack_104, &stack_f8);
			if(PSVECMag(mVelocity.toCVec()) > 20f && elementA < elementB * 0.5f) {
				if(mMario -> isAnimationRun("坂すべり下向きあおむけ")) {
					mMario -> push(mMario -> _208 % 5f);
				}
				else if(mMario -> isAnimationRun("坂すべり上向きうつぶせ")) {
					mMario -> push(mMario -> _208 % -5f);
				}
				mMario -> _14 |= 0x20000000;
			}
		}
		if(getMovementStates()._0 && !_9f1) {
			if(stack_128.dot(getGravityVec()) < -40f) {
				TVec3f stack_ec(mPosition.translateOpposite(getGravityVec() % 100f));
				TVec3f stack_e0;
				Triangle *pTmp = mMario -> getTmpPolygon();
				
				if(MR::getFirstPolyOnLineToMap(&stack_e0, pTmp, stack_ec, getGravityVec() % 200f)) {
					TVec3f stack_d4;
					if (
						MR::vecKillElement (
							stack_e0.translateOpposite(mPosition),
							getGravityVec(),
							&stack_d4
						) < -5f
						&& pTmp -> mParts
						&& !pTmp -> mParts -> _D4
						&& getMovementStates()._3e != 1
					) {
						mPosition = stack_e0;
						mMario -> _130 = mPosition;
						mMario -> stopJump();
						mMario -> changeAnimation("基本", "落下");
						mMario -> updateGroundInfo();
					}
				}
			}
			else if(stack_128.dot(getGravityVec()) > 40f) {
				if(mMario -> _4C8 -> isValid()) {
					if(MR::isSensorPressObj(mMario -> _4C8 -> mSensor)) {
						mMario -> mDrawStates._1e = true;
					}
				}
				else {
					const u32 stop = mBinder -> _28;
					for(int i = 0; i < stop; i++) {
						if(MR::isSensorPressObj(mBinder -> getPlane(i) -> mSensor)) {
							mMario -> mDrawStates._1e = true;
						}
					}
				}
				if(mMario -> _31C.translateOpposite(mPosition).dot(getGravityVector()) < 0f) {
					bool eject = true;
					CollisionParts *parts = mMario -> _45C -> mParts;
					if(parts && !mMario -> _45C -> mParts -> _D4) {
						TVec3f stack_c8, stack_bc, stack_b0;
						PSMTXMultVec (
							parts -> mInvBaseMatrix.toMtxPtr(),
							mMario -> _31C.toCVec(),
							stack_c8.toVec()
						);
						PSMTXMultVec (
							parts -> mPrevBaseMatrix.toMtxPtr(),
							stack_c8.toCVec(),
							stack_bc.toVec()
						);
						stack_b0 = mMario -> _31C.translateOpposite(stack_bc);
						if(stack_b0.dot(stack_128) > 0f) eject = false;
					}
					if(eject) {
						mPosition = mMario -> _31C;
						mMario -> _2D4.zero();
						mMario -> _148.zero();
						if(!mMario -> _5FC && (getDrawStates()._1e || getPrevDrawStates()._1e)) {
							TVec3f stack_a4(stack_128);
							MR::normalizeOrZero(&stack_a4);
							_3b4 = stack_a4;
							setPress(0, 0);
							_3b0 = 0.1f;
						}
					}
				}
			}
		}
		else if(mMario -> getMovementStates()._1) {
			const u32 stop = mBinder -> _28;
			bool eject = false;
			for(u32 i = 0; i < stop; i++) {
				const Triangle *plane = mBinder -> getPlane(i);
				if(!MR::isSensorPressObj(plane -> mSensor)) continue;
				if(mMario -> _368.dot(*plane -> getNormal(0)) > 0f) {
					if(mMario -> _72C < 200f) eject = true;
					else if(plane -> getNormal(0) -> dot(stack_134) < 0f) eject = true;
					else eject = false;
				}
				else if(plane -> getNormal(0) -> dot(stack_134) < 0f) eject = true;
				else if(mMario -> _5FC) {
					if(!MR::isWallCodeNoAction(plane) && !mMario -> isOnimasuBinderPressSkip()) {
						_3b4 = mMario -> _368;
						mMario -> _10 &= ~(u32)4;
						*mMario -> _4C8 = *plane;
						setPress(2, 0);
						_3b0 = 0.1f;
					}
					eject = true;
				}
			}
			if(eject) {
				TVec3f stack_98;
				f32 element = MR::vecKillElement(stack_134, mMario -> _368, &stack_98);
				mPosition -= mMario -> _368 % element;
			}
		}
	}
	if(mMario -> mDrawStates._1a) {
		mMario -> mDrawStates._1a = false;
		mMarioAnim -> MarioAnimator::update();
	}
	mMario -> recordRelativePosition();
	updateTransForCamera();
	calcAnimInMovement();
	_935 = false;
	mMario -> _2D0 = 0f;
	_f3c_vec[_f40] = mMario -> _208;
	_f40 = (u16)(_f40 + 1) % _f42;
}

void MarioActor::control() {
	if(mSuperKinokoCollected) {
		if(MR::tryStartDemoWithoutCinemaFrame(this, "マリオスーパー化")) { // 6-up camera
			mSuperKinokoCollected = false;
			changeMaxLife(6);
			MR::stopAnimFrame(this);
			MR::requestPowerupHPMeter();
			mMario -> startPadVib("マリオ[変身]");
			MR::startSystemSE("SE_SY_SUPER_KINOKO_GET", -1, -1);
			_3da = 0x5a;
		}
	}
	else if(mPowerupCollected) {
		if(MR::tryStartDemoWithoutCinemaFrame(this, "マリオ変身")) {
			mPowerupCollected = false;
			mTransforming = true;
			if(_3d4 == 6) {
				MR::requestMovementOn(_9a4);
				changeTeresaAnimation("change", -1);
			}
			_3d8 = 0x40;
			MR::stopAnimFrame(this);
			playEffect("変身");
			mMario -> startPadVib("マリオ[変身]");
		}
	}
	control2();
	_294 = mPosition;
}

void MarioActor::control2() {
	if((_3d8 && _3d4)  || _3da) {
		mMario -> _350.zero();
		mMario -> _35C.zero();
		_7dc = 0;
		_930 = 0;
		mVelocity.zero();
		_264.zero();
		_270 = mPosition;
		if (
			getMovementStates()._1
			&& !MR::isSameMtx (
				mMario -> _45C -> getPrevBaseMtx() -> toMtxPtr(),
				mMario -> _45C -> getBaseMtx() -> toMtxPtr()
			)
		) {
			mMario -> _130 = mPosition;
			mMario -> checkEnforceMove();
			mPosition = mMario -> _130;
		}
	}
	else {
		if(_bc4) _bc4--;
		_951 = 0;
		controlMain();
		if(!_ea4) {
			if(!_f44) MR::offBind(this);
			else MR::onBind(this);
		}
		_978 = mVelocity;
		if(getDrawStates()._b) mBinder -> _1ec &= ~(u32)0x40;
		else mBinder -> _1ec |= 0x40;
		if(mMario -> isDamaging()) _424 = 0;
	}
}

void MarioActor::controlMain() {
	if(_3c0) {
		_7dc = 0;
		_930 = 0;
	}
	updateActionTrigger();
	updateControllerSwing();
	updateControllerSwingLeft();
	update2D();
	if(_a61) _a6c++;
	if(!isEnableMoveMario()) {
		_7dc = 0;
		_930 = 0;
		careMovingObject();
	}
	else {
		mVelocity.zero();
		updateBehavior();
		updateRotationInfo();
		updateTakingPosition();
		updateSearchLight();
		updateThrowing();
		updateBeeWingAnimation();
		updateFairyStar();
		_930 = 0;
	}
}

void MarioActor::updateBehavior() {
	updateLife();
	updatePlayerMode();
	_37c++;
	if(_ef6) _ef6--;
	if(_3a8) _3a8--;
	if(_3ac) _3ac--;
	_264 = mPosition.translateOpposite(_270);
	_270 = mPosition;
	updateBindRatio();
	updateEffect();
	if(_b94 && !--_b94) {
		mMario -> stopAnimationUpper("ハンマー投げ回転中", nullptr);
		mMario -> stopAnimation("ハンマー投げ回転中", (const char *)nullptr);
	}
	updatePunching();
	if(!doPressing() && !doStun() && !doRush()) {
		updateGravityVec(false, false);
		if(!tryJumpRush()) {
			checkPriorRushTarget();
			if(!tryThrow() && !tryStandardRush()) {
				if(getMovementStates()._1) initForJump();
				updateSwingTimer();
				updateSwingAction();
				updateThrowVector();
				lockOnDPD();
				if(!checkClapCatchStart()) {
					_7dc = 0;
					mMario -> Mario::update();
					updateForCamera();
					updateTornado();
					tryCoinPull();
					mMario -> mDrawStates._1a = true;
				}
			}
		}
	}
}

void MarioActor::updateBindRatio() {
	if(!_934 && !MR::isNearZero(_978.translateOpposite(_264), 0.001f)) {
		f32 mag = PSVECMag(_978.toCVec());
		if(mag / PSVECMag(_978.translateOpposite(_264).toCVec()) < 2f) {
			_984 += 0.1f;
		}
		else {
			_984 -= 0.1f;
		}
	}
	else if(!MR::isNearZero(_978, 0.001f)) _984 -= 0.01f;
	_984 = MR::clamp(_984, 0f, 1f);
}

bool MarioActor::isInPunchTimerRange() const {
	bool ret = false;
	if(_3e6) {
		ret = MR::isInRange(_945, 6f, 20f);
	}
	return ret; 
}

void MarioActor::updatePunching() {
	if(_944) {
		bool canSpinring = true;
		_944--;
		_945++;
		if(!_946 && !getMovementStates()._2b) canSpinring = false;
		if (
			canSpinring
			&& !mMario -> isSwimming()
			&& !_944
			&& selectAction("スピン回復エフェクト") == true
		) {
			playEffect("スピンリング");
		}
		bool canPunch = isInPunchTimerRange();
		if(!mMario -> isSwimming()) canPunch = true;
		if(canPunch) {
			const Mario::MovementStates &moveStates = getMovementStates();
			if(moveStates._8 || moveStates._1a || moveStates._19) mMario -> tryWallPunch();
		}
	}
	if (
		mMario -> isAnimationRun("ハンマー投げリリース")
		&& mMario -> getMovementStates()._1
		&& !_38c
		&& !mMario -> _420
		&& mMario -> Mario::isStickOn()
	) {
		mMario -> stopAnimation(nullptr, (const char *)nullptr);
	}
}

bool MarioActor::doRush() {
	if(_934) {
		tryCoinPull();
		tryRushInRush();
		if(!_934) return false;
		updateGravityVec(false, false);
		if(!checkClapCatchStart() && _7dc) bodyClap();
		_7dc = 0;
		_7e2 = false;
		mMario -> _130 = mPosition;
		mMario -> updateGroundInfo();
		mMario -> updateCubeCode();
		mMario -> decDamageAfterTimer();
		if(selectWaterInOutRush(_924)) {
			WaterInfo stack_44;
			if(!MR::getWaterAreaObj(&stack_44, mPosition)) mMario -> forceExitSwim();
		}
		else if(!selectWaterInOut(_924 -> mActor -> mName)) {
			s32 initial = mMario -> mSwim -> _144;
			mMario -> mSwim -> checkWaterCube(false);
			if((int)mMario -> mSwim -> _144 != initial) {
				if(mMario -> mSwim -> _144 <= 1 && (u32)initial - 2 <= 1) {
					playEffectRT("水面ジャンプ水柱", mMario -> mSwim -> _160, mMario -> mSwim -> _16c);
					emitEffectWaterColumn(mMario -> mSwim -> _160, mMario -> mSwim -> _16c);
				}
				else if((u32)initial <= 1 && mMario -> mSwim -> _144 - 2 <= 1) {
					playEffectRT("水面ジャンプ水柱", -mMario -> _328, mMario -> mSwim -> _16c);
					emitEffectWaterColumn(mMario -> mSwim -> _160, mMario -> mSwim -> _16c);
				}
				if(initial == 2) {
					_384 = 8;
					MR::getGameSceneLayoutHolder().changeLifeMeterGround();
					mMario -> forceExitSwim();
				}
				else if(initial == 0) MR::getGameSceneLayoutHolder().changeLifeMeterSwim();
			}
		}
		if(mMario -> isForceStopRush()) {
			bool received = _924 -> receiveMessage(0x95, getSensor("body"));
			if(_934 && received) {
				RushEndInfo stack_20(nullptr, 4, TVec3f(0f, 0f, 0f), false, 0);
				endRush(&stack_20);
			}
		}
		mMarioAnim -> setHoming();
		updateTornado();
		if(_934 && selectRecoverFlyMeter(_924)) {
			for(u32 i = 0; i < 4; i++) {
				mMario -> incAirWalkTimer();
			}
		}
		updateSwingTimer();
		return true;
	}
	else {
		_7e2 = false;
		return false;
	}
	
}

void MarioActor::updateSwingTimer() {
	if(_934) stopSpinTicoEffect(false);
	else {
		if(_948) _948--;
		if(_94c && --_94c == 6) _94e = 5;
		if(_94e && --_94e == 0) {
			mMario -> startPadVib((unsigned long)0);
			mMario -> playSound("スピン回復終了", -1);
			Color8 stack_8;
			stack_8.set(0x50, 0x80, 0xc8, 0);
			_1aa = 0xf;
			_1ac = 1.5f;
			_1b0 = stack_8;
			_1b5 = true;
		}
		if(_946) {
			if(--_946 == 0x18) {
				selectAction("スピン回復エフェクト");
				stopEffectForce("スピンリング");
			}
			if(_946 == 0xd) _94c = 0x13;
		}
	}
}

void MarioActor::updateSwingAction() {
	if(isJumping() && _3d4 != 6 && !mMario -> isStatusActive(0x18)) _946 = 0;
	bool requestRush = isRequestRush();
	bool requestSpinJump2P = false;
	if(!requestRush) {
		if(requestRush = isRequestSpinJump2P()) requestSpinJump2P = true;
	}
	_1e1 = requestRush;
	if(_946) {
		if(!requestRush) return;
		if(!isPunching()) {
			if(_948) _948 += 0x96;
			else _948 += 0x12c;
		}
		if(!requestRush) return;
		tryReleaseBombTeresa();
		return;
	}
	bool canRush = true;
	if(!requestRush) return;
	if(mMario -> isAnimationRun("壁はじき")) canRush = false;
	if(isJumping() && mMario -> _428) canRush = false;
	if(_3d4 == 9) canRush = false;
	if(mMario -> getCurrentStatus() == 1) canRush = false;
	if(mMario -> isSwimming()) canRush = false;
	if(mMario -> isStatusActive(0x18)) canRush = false;
	if(mMario -> isStatusActive(0x13)) canRush = false;
	if(_468l.x) canRush = false;
	if(mMario -> isStatusActive(2)) canRush = false;
	if(_3c0) canRush = false;
	if(_ea4) canRush = false;
	if(!canRush) return;
	u8 action = selectAction("スピンアタック");
	switch(action) {
		case 1:
			bool didSpinPunch = true;
			if (
				!mMario -> mMovementStates._f
				&& isJumping()
				&& !mMario -> isDamaging()
				&& !mMario -> mMovementStates._2b
			) {
				bool tmp = false;
				if(_f0c) tmp = true;
				mMario -> trySpinJump(tmp);
				setPunchHitTimer(0x19);
				tryReleaseBombTeresa();
				if(requestSpinJump2P) MR::start2PJumpAssistSound();
			}
			else if(!getMovementStates()._f && !mMario -> isAnimationRun("地上ひねり")) {
				const char *pLastAnimationName = mMarioAnim -> _10 -> getCurrentAnimationName();
				if(_3d4 == 4) {
					if(!mMario -> isAnimationRun("ハチスピン")) didSpinPunch = trySpinPunch();
				}
				else didSpinPunch = trySpinPunch();
				_974 = 0;
				if(pLastAnimationName != mMarioAnim -> _10 -> getCurrentAnimationName()) {
					mMario -> playSound("パンチ風切り", -1);
				}
			}
			if(_3d4 == 4) {
				if(isJumping()) {
					if(!mMario -> isAnimationRun("ハチスピン空中")) {
						mMario -> playSound("声スピン", -1);
						mMario -> playSound("スピンジャンプ", -1);
					}
					mMario -> changeAnimation("ハチスピン空中", (const char *)nullptr);
				}
				else if(getMovementStates()._a || _9f1) {
					mMario -> changeAnimation("サマーソルト", (const char *)nullptr);
				}
				else mMario -> changeAnimation("ハチスピン", (const char *)nullptr);
			}
			if(didSpinPunch) _946 = mConst -> _0[mConst -> _8] -> _426 + 0x22;
			break;
		case 2:
			if(isEnableSpinPunch() && !mMario -> isSwimming()) shootFireBall();
			break;
		case 3:
			if(isEnableSpinPunch()) {
				doFreezeAttack();
				setPunchHitTimer(0x1e);
			}
			break;
		case 4:
			if(mMario -> _418 != 0) break;
			mMario -> startTeresaDisappear();
			const Constants *pConstants = mConst -> _0[mConst -> _8];
			_946 = pConstants -> _6c8 + pConstants -> _426;
			break;
		case 5:
			if(!isEnableSpinPunch()) break;
			if(isJumping()) mMario -> changeAnimation("ハチスピン空中", (const char *)nullptr);
			else {
				if(getMovementStates()._a || _9f1) {
					mMario -> changeAnimation("サマーソルト", (const char *)nullptr); //Summersault
				}
				else mMario -> changeAnimation("ハチスピン", (const char *)nullptr);
			}
			_946 = mConst -> _0[mConst -> _8] -> _426 + 0x22;
			break;
	}
}

void MarioActor::updateRealMtx() {
	updateBaseScaleMtx();
	getRealMtx(_c2c.toMtxPtr(), "HandL");
	getRealMtx(_c5c.toMtxPtr(), "HandR");
	PSMTXCopy(MR::getJointMtx(this, "HandL"), _c8c.toMtxPtr());
	PSMTXCopy(MR::getJointMtx(this, "HandR"), _cbc.toMtxPtr());
	PSMTXCopy(MR::getJointMtx(this, "Face0"), _cec.toMtxPtr());
	getRealMtx(_d1c.toMtxPtr(), "FootR");
	getRealMtx(_d4c.toMtxPtr(), "Spine1");
	getRealMtx(_d7c.toMtxPtr(), "CapPosition");
	getRealMtx(_dac.toMtxPtr(), "Hip");
	getRealMtx(_ddc.toMtxPtr(), "Spine2");
	TVec3f stack_50;
	MR::extractMtxTrans(_ddc.toMtxPtr(), &stack_50);
	if(MR::isNan(stack_50)) {
		MR::getJointMtx(this, "Spine2");
		getBaseMtx();
	}
	bool notStatus12 = true;
	if(mMario -> isStatusActive(0x12)) notStatus12 = false;
	if(!notStatus12) return;
	TVec3f stack_44, stack_38;
	MR::extractMtxTrans(_d7c.toMtxPtr(), &stack_44);
	MR::extractMtxTrans(_dac.toMtxPtr(), &stack_38);
	_4b8 = stack_44.translateOpposite(stack_38);
	_4c4 = stack_38.translateOpposite(stack_44);
	if(MR::normalizeOrZero(&_4b8)) _4b8 = mMario -> _1F0;
	if(MR::normalizeOrZero(&_4c4)) _4c4 = -mMario -> _1F0;
	if(mMario -> getMovementStates()._a && mMario -> mMovementStates._1) _4c4 = -mMario -> _1F0;
	_2ac = stack_44;
}

void MarioActor::decLife(unsigned short amt) {
	if(getMovementStates()._1f) return;
	if(_388 > 0 && amt > _388) return;
	if(amt) {
		_388 = amt;
		return;
	}
	if(mHealth) mHealth--;
	_388 = 0;
	if(gIsLuigi) {
		if(mMaxHealth == 3) return;
		if(mHealth > 3) return;
		mMaxHealth = 3;
		return;
	}
	if(mMaxHealth == 3) return;
	if(mHealth > 3) return;
	mMaxHealth = 3;
}

void MarioActor::decLifeLarge() {
	decLife(0);
}

void MarioActor::decLifeMiddle() {
	decLife(0);
}

void MarioActor::decLifeSmall() {
	decLife(0);
}

void MarioActor::resetWaterLife() {
	_384 = 8;
}

void MarioActor::updateLife() {
	if(!_388) return;
	if(--_388 != 0) return;
	if(!mHealth) return;
	mHealth--;
}

static const char *sMiddleWaterLifeReduction = "水中ライフ減少";

void MarioActor::incLife(unsigned long amt) {
	if(isEnableNerveChange() && !_3e4) {
		const u32 health = getHealth();
		if(mHealth != mMaxHealth) mMario -> playSound("ライフ回復", -1);
		mHealth += amt;
		if(mHealth >= mMaxHealth) mHealth = mMaxHealth;
		if(health == 1 && mMarioAnim -> isAnimationStop()) {
			mMarioAnim -> _c -> changeTrackAnimation(3, "ノーマルウエイト");
			if(mMario -> _970 && strcmp(mMario -> _970, "DamageWait")) {
				mMario -> startBas(nullptr, false, 0f, 0f);
				setBlink(nullptr);
			}
		}
	}
}

void MarioActor::changeMaxLife(long max) {
	mMaxHealth = max;
	while(mHealth != max) {
		if(mHealth > max) decLife(0);
		else if(mHealth < max) incLife(1);
		else break;
	}
}

bool MarioActor::doPressing() {
	if(!_390) return false;
	switch(_39c) {
		case 0:
			if(!mMario -> checkVerticalPress(false)) {
				if(--_390 == 0x1e && !mHealth && isEnableNerveChange()) {
					setNerve(&NrvMarioActor::MarioActorNrvGameOver::sInstance);
				}
				if(!_390) {
					mMario -> changeAnimation("つぶれ解除", (const char *)nullptr);
					_f44 = true;
				}
			}
			break;
		case 2:
		case 3:
			if(--_390 == 0x1e && !mHealth && isEnableNerveChange()) {
				setNerve(&NrvMarioActor::MarioActorNrvGameOver::sInstance);
			}
			if(!_390) {
				mMario -> changeAnimation("つぶれ解除", (const char *)nullptr);
				_f44 = true;
			}
			break;
		case 4:
			if(_390 > 2) _390--;
			mMario -> mMovementStates._8 = false;
			mMario -> mMovementStates._19 = false;
			mMario -> mMovementStates._1a = false;
			break;
	}
	if(_39c != 1) careMovingObject();
	mMarioAnim -> setSpeed(0f);
	mMario -> _350.zero();
	mMario -> _35C.zero();
	resetSensorCount();
	return true;
}

void MarioActor::careMovingObject() {
	if(_1e0) return;
	if (
		getMovementStates()._1
		&& !MR::isSameMtx (
			mMario -> _45C -> getPrevBaseMtx() -> toMtxPtr(),
			mMario -> _45C -> getBaseMtx() -> toMtxPtr()
		)
	) {
		mMario -> _130 = mPosition;
		mMario -> checkEnforceMove();
		mPosition = mMario -> _130;
	}
	mMario -> _130 = mPosition;
	mMario -> _160.zero();
	mMario -> powerAreaMove();
	mMario -> powerRailMove();
	mMario -> _130 += mMario -> _160;
	mMario -> _160.zero();
	mPosition = mMario -> _130;
	mMario -> checkGround();
	mMario -> updateFloorCode();
}

bool MarioActor::doStun() {
	if(_38c) {
		mMario -> mDrawStates._1a = true;
		resetSensorCount();
		if (
			getMovementStates()._1
			&& !MR::isSameMtx (
				mMario -> _45C -> getPrevBaseMtx() -> toMtxPtr(),
				mMario -> _45C -> getBaseMtx() -> toMtxPtr()
			)
		) {
			mMario -> _130 = mPosition;
			mMario -> checkEnforceMove();
			mPosition = mMario -> _130;
		}
		if(!--_38c) {
			f32 stickX, stickY;
			getStickValue(&stickX, &stickY);
			if (
				MR::isNearZero(stickX, 0.001f)
				&& MR::isNearZero(stickY, 0.001f)
				&& !getMovementStates()._a
			) mMario -> stopWalk();
		}
		mMario -> inputStick();
		return true;
	}
	else return false;
}

void MarioActor::scaleMtx(MtxPtr rawMtx) {
	TVec3f i, j, k;
	const TRot3f *pMtx = (TRot3f *)rawMtx;
	f32 scalar = 0.35f * (1f - _3b0) + 1f;
	pMtx -> getXDir(i);
	pMtx -> getYDir(j);
	pMtx -> getZDir(k);
	f32 elementX = MR::vecKillElement(i, _3b4, &i);
	f32 elementY = MR::vecKillElement(j, _3b4, &j);
	f32 elementZ = MR::vecKillElement(k, _3b4, &k);
	elementZ *= _3b0;
	elementX *= _3b0;
	elementY *= _3b0;
	i.scale(scalar);
	j.scale(scalar);
	k.scale(scalar);
	i += _3b4 % elementX;
	j += _3b4 % elementY;
	k += _3b4 % elementZ;
	rawMtx[0][0] = i.x;
	rawMtx[1][0] = i.y;
	rawMtx[2][0] = i.z;
	rawMtx[0][1] = j.x;
	rawMtx[1][1] = j.y;
	rawMtx[2][1] = j.z;
	rawMtx[0][2] = k.x;
	rawMtx[1][2] = k.y;
	rawMtx[2][2] = k.z;
}

void MarioActor::updateBaseScaleMtx() {
	if(mMario -> mMovementStates._3c) PSMTXScale(_bf8.toMtxPtr(), mScale.x, mScale.y, mScale.z);
	else PSMTXIdentity(_bf8.toMtxPtr());
}
