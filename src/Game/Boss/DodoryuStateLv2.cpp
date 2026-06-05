#include "Game/Boss/DodoryuStateLv2.hpp"
#include "Game/Boss/Dodoryu.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <JSystem/JMath/JMath.hpp>
#include <cmath>

namespace {
    NEW_NERVE_ONEND(DodoryuStateLv2NrvStart, DodoryuStateLv2, Start, Start);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvReadyChase, DodoryuStateLv2, ReadyChase, ReadyChase);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvChaseHide, DodoryuStateLv2, ChaseHide, ChaseHide);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvReadyAppear, DodoryuStateLv2, ReadyAppear, ReadyAppear);
    NEW_NERVE(DodoryuStateLv2NrvAppearSign, DodoryuStateLv2, AppearSign);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvChaseAppearStart, DodoryuStateLv2, ChaseAppearStart, ChaseAppearStart);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvChaseAppear, DodoryuStateLv2, ChaseAppear, ChaseAppear);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvChaseAppearEnd, DodoryuStateLv2, ChaseAppearEnd, ChaseAppearEnd);
    NEW_NERVE(DodoryuStateLv2NrvJumpOut, DodoryuStateLv2, JumpOut);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvEscape, DodoryuStateLv2, Escape, Escape);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvEscapeJump, DodoryuStateLv2, EscapeJump, EscapeJump);
    NEW_NERVE(DodoryuStateLv2NrvRecover, DodoryuStateLv2, Recover);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvDive, DodoryuStateLv2, Dive, Dive);
    NEW_NERVE(DodoryuStateLv2NrvKnockDown, DodoryuStateLv2, KnockDown);
    NEW_NERVE(DodoryuStateLv2NrvLaugh, DodoryuStateLv2, Laugh);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvLaughEnd, DodoryuStateLv2, LaughEnd, LaughEnd);
    NEW_NERVE(DodoryuStateLv2NrvFindPos, DodoryuStateLv2, FindPos);
    NEW_NERVE(DodoryuStateLv2NrvHideAttack, DodoryuStateLv2, HideAttack);
    NEW_NERVE(DodoryuStateLv2NrvChaseMoreStart, DodoryuStateLv2, ChaseMoreStart);
    NEW_NERVE_ONEND(DodoryuStateLv2NrvChaseMore, DodoryuStateLv2, ChaseMore, ChaseMore);
    NEW_NERVE(DodoryuStateLv2NrvChaseMoreEnd, DodoryuStateLv2, ChaseMoreEnd);
};  // namespace

DodoryuStateLv2::DodoryuStateLv2(Dodoryu* pHost, DodoryuChaseParam* pChaseParam, const char* pName)
    : DodoryuStateBase(pHost, pName), _90(), _94(0.0f, 0.0f, 0.0f), _A0(0.0f, 0.0f, 0.0f), _AC(), _B0(1.0f), _B4(), _B8(), _BC(), mAppearCount(),
      _C4(), _C8(), _CC(), _D0(), _D4(), _D8(), mFindPosCounter(), _E0(), _E4(true), mChaseParam(pChaseParam), mPlayerStaggering() {
    initWithoutIter();
}

void DodoryuStateLv2::init(const JMapInfoIter& rIter) {
    MR::invalidateClipping(this);
    initNerve(&DodoryuStateLv2NrvStart::sInstance);
    MR::declareEventCameraProgrammable("DodoryuMini");
    makeActorAppeared();
}

void DodoryuStateLv2::control() {
    if (mPlayerStaggering) {
        if (!MR::isPlayerStaggering()) {
            mPlayerStaggering = false;
        }
    }
}

void DodoryuStateLv2::start() {
    setNerve(&DodoryuStateLv2NrvStart::sInstance);
    MR::invalidateShadowAll(mHost);
    mHost->setHillAppearNumMax();
    mHost->shiftMoveStateNull();
    mAppearCount = 0;
}

void DodoryuStateLv2::exeStart() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_CC);
        mHost->stopHillForce();
        mHost->shiftMoveStateNull();
    }
    if (MR::isBckOneTimeAndStopped(mHost)) {
        setNerve(&DodoryuStateLv2NrvFindPos::sInstance);
    }
}

void DodoryuStateLv2::endStart() {
}

void DodoryuStateLv2::exeReadyChase() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_E0);
        MR::showModel(mHost);
        mHost->stopHill();
        MR::invalidateHitSensors(mHost);
        MR::stopBck(mHost);
        MR::emitEffect(mHost, "AttackSign");
        MR::startSound(mHost, "SE_BM_DODORYU_SAND_SPLASH");
        MR::tryRumblePadMiddle(this, 0);
        MR::shakeCameraNormal();
        mHost->shiftMoveStateNull();
    }
    updateReadyChaseShake();
    if (MR::isGreaterEqualStep(this, mChaseParam->_0)) {
        startAnim(mChaseParam->_F4);
        setNerve(&DodoryuStateLv2NrvChaseHide::sInstance);
    }
}

void DodoryuStateLv2::endReadyChase() {
    MR::stopShakingCamera(this);
}

void DodoryuStateLv2::exeChaseHide() {
    if (MR::isFirstStep(this)) {
        shiftChaseUnderground();
        MR::validateHitSensors(mHost);
        mHost->invalidateStarPieceSensor();
    }
    if (MR::isBckOneTimeAndStopped(mHost)) {
        startAnim(mChaseParam->_F4);
        mHost->stopHill();
    }
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_U_DEEP");
    updateChaseHideShake();
    tryShiftToChaseAppear();
}

void DodoryuStateLv2::endChaseHide() {
    MR::stopShakingCamera(this);
}

void DodoryuStateLv2::exeReadyAppear() {
    if (MR::isFirstStep(this)) {
        MR::shakeCameraInfinity(this, 0.2f, 2.0f);
        mHost->startHill();
        _B8 = MR::getRandom(mChaseParam->_2C, mChaseParam->_30);
        shiftChaseExposingHead();
    }
    if (MR::isBckOneTimeAndStopped(mHost)) {
        startAnim(mChaseParam->_11C);
    }
    mHost->tryRumblePad();
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_U_SHALLOW");
    f32 dist = MR::calcDistanceToPlayer(mHost);
    if (dist >= mChaseParam->_34) {
        mHost->stopHill();
        startAnim(mChaseParam->_130);
        setNerve(&DodoryuStateLv2NrvChaseHide::sInstance);
    } else if (tryToShiftAppearSign()) {
        return;
    }
}

void DodoryuStateLv2::endReadyAppear() {
    MR::stopShakingCamera(this);
}

void DodoryuStateLv2::exeAppearSign() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_130);
        mHost->stopHill();
        shiftChaseExposingHead();
    }
    if (MR::isStep(this, 15)) {
        MR::emitEffect(mHost, "AttackSign");
    }
    if (MR::isBckOneTimeAndStopped(mHost)) {
        startAnim(mChaseParam->_F4);
    }
    if (MR::isGreaterEqualStep(this, mChaseParam->_18)) {
        setNerve(&DodoryuStateLv2NrvChaseAppearStart::sInstance);
    }
}

void DodoryuStateLv2::exeChaseAppearStart() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_144);
        mHost->startHill();
        MR::startSound(mHost, "SE_BM_DODORYU_APPEAR_HEAD");
        MR::startSound(mHost, "SE_BV_DODORYU_APPEAR_2");
        shiftChaseExposingBust();
        mHost->validateStarPieceSensor();
    }
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_GROUND");
    if (MR::isBckOneTimeAndStopped(mHost)) {
        setNerve(&DodoryuStateLv2NrvChaseAppear::sInstance);
    }
}

void DodoryuStateLv2::endChaseAppearStart() {
    mHost->invalidateStarPieceSensor();
}

void DodoryuStateLv2::exeChaseAppear() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_158);
        mHost->validateStarPieceSensor();
    }
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_SHOUT");
    tryShiftToChaseAppearEnd();
}

void DodoryuStateLv2::endChaseAppear() {
    mHost->invalidateStarPieceSensor();
}

void DodoryuStateLv2::exeChaseAppearEnd() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_16C);
        mAppearCount++;
    }
    updateHideShake();
    if (MR::isBckOneTimeAndStopped(mHost)) {
        startAnim(mChaseParam->_E0);
        s32 maxAppear = mChaseParam->_54;
        if (mAppearCount >= maxAppear) {
            setNerve(&DodoryuStateLv2NrvFindPos::sInstance);
        } else {
            setNerve(&DodoryuStateLv2NrvChaseHide::sInstance);
        }
    }
}

void DodoryuStateLv2::endChaseAppearEnd() {
    MR::stopShakingCamera(this);
    mHost->stopHill();
}

void DodoryuStateLv2::exeJumpOut() {
    if (MR::isFirstStep(this)) {
        mHost->stopHillForce();
        mHost->mBaseMtx.getTrans(_94);
        mHost->mVelocity.zero();
        startAnim(mChaseParam->_180);
        MR::startSound(mHost, "SE_BM_DODORYU_JUMP_OUT");
        MR::startSound(mHost, "SE_BV_DODORYU_CRY");
        MR::validateShadowAll(mHost);
        f32 jumpHeight = mChaseParam->_8C;
        mHost->shiftMoveStateJumpOut(jumpHeight, 50.0f);
    }
    if (MR::isBckOneTimeAndStopped(mHost)) {
        _C8 = mChaseParam->_58;
        _CC = mChaseParam->_74;
        setNerve(&DodoryuStateLv2NrvEscape::sInstance);
    }
}

void DodoryuStateLv2::exeEscape() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_194);
        f32 speed = mChaseParam->_68;
        f32 accel = mChaseParam->_6C;
        f32 brake = mChaseParam->_70;
        s32 turnMax = mChaseParam->_60;
        s32 turnMin = mChaseParam->_64;
        mHost->shiftMoveStateEscape(speed, turnMax, turnMin, accel, brake);
        mHost->validateStarPieceSensor();
    }
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_RUN");
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_PANT");
    if (_C8 > 0) {
        --_C8;
    }
    if (_CC > 0) {
        --_CC;
    }
    if (isEscapeEnd()) {
        setNerve(&DodoryuStateLv2NrvRecover::sInstance);
    }
}

void DodoryuStateLv2::endEscape() {
    mHost->invalidateStarPieceSensor();
}

void DodoryuStateLv2::exeEscapeJump() {
    if (MR::isFirstStep(this)) {
        TVec3f gravity(mHost->mGravity);
        gravity.scale(-10.0f);
        mHost->mVelocity.set(gravity);
        mHost->shiftMoveStateNull();
    }
    addVelocity(false);
    if (_CC > 0) {
        _CC--;
    }
    if (MR::isGreaterEqualStep(this, 49)) {
        setNerve(&DodoryuStateLv2NrvEscape::sInstance);
        mHost->snapToGround();
    } else {
        TVec3f gravity(mHost->mGravity);
        gravity.scale(0.5f);
        mHost->mVelocity.add(gravity);
    }
}

void DodoryuStateLv2::endEscapeJump() {
    mHost->mVelocity.zero();
}

void DodoryuStateLv2::exeRecover() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_1A8);
        mHost->mVelocity.zero();
        mHost->shiftMoveStateNull();
        MR::startSound(mHost, "SE_BV_DODORYU_GET_UP");
    }
    if (MR::isBckOneTimeAndStopped(mHost)) {
        setNerve(&DodoryuStateLv2NrvDive::sInstance);
    }
}

void DodoryuStateLv2::exeDive() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_1BC);
    }
    updateDiveShake();
    if (MR::isStep(this, 50)) {
        MR::invalidateShadowAll(mHost);
    }
    if (MR::isBckOneTimeAndStopped(mHost)) {
        setNerve(&DodoryuStateLv2NrvFindPos::sInstance);
    }
}

void DodoryuStateLv2::endDive() {
    MR::stopShakingCamera(this);
}

void DodoryuStateLv2::exeKnockDown() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_1D0);
        MR::startBlowHitSound(mHost);
        MR::startSound(mHost, "SE_BV_DODORYU_DAMAGED");
        mHost->shiftMoveStateNull();
        mHost->startSpinOutCamera();
    }
    s32 stopFrame = mChaseParam->_90;
    if (stopFrame >= 0 && MR::isGreaterEqualStep(this, stopFrame)) {
        mHost->mVelocity.zero();
    } else {
        knockOver();
    }
    s32 endFrame = mChaseParam->_94;
    if (endFrame >= 0 && MR::isGreaterEqualStep(this, endFrame)) {
        mHost->endSpinOutCamera();
        mHost->nextState();
    } else if (endFrame < 0 && MR::isBckOneTimeAndStopped(mHost)) {
        mHost->endSpinOutCamera();
        mHost->nextState();
    }
}

void DodoryuStateLv2::exeLaugh() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_1E4);
        mHost->mVelocity.zero();
        mHost->shiftMoveStateNull();
    }
    if (MR::isStep(this, 30)) {
        MR::startSound(mHost, "SE_BV_DODORYU_LAUGH");
    }
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_SHOUT");
    s32 laughTime = mChaseParam->_78;
    if (MR::isGreaterEqualStep(this, laughTime)) {
        setNerve(&DodoryuStateLv2NrvLaughEnd::sInstance);
    }
}

void DodoryuStateLv2::exeLaughEnd() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_1F8);
    }
    updateHideShake();
    if (MR::isBckOneTimeAndStopped(mHost)) {
        setNerve(&DodoryuStateLv2NrvFindPos::sInstance);
    }
}

void DodoryuStateLv2::endLaughEnd() {
    MR::stopShakingCamera(this);
}

void DodoryuStateLv2::exeFindPos() {
    if (MR::isFirstStep(this)) {
        s32 minTime = mChaseParam->_7C;
        s32 maxTime = mChaseParam->_80;
        mFindPosCounter = MR::getRandom(minTime, maxTime);
        MR::stopBck(mHost);
        MR::invalidateHitSensors(mHost);
        mHost->stopHillForce();
        mHost->startPlayerCenterCamera();
        MR::invalidateShadowAll(mHost);
        MR::hideModel(mHost);
        mHost->shiftMoveStateNull();
        mAppearCount = 0;
    }
    if (MR::isStep(this, 1)) {
        calcRandomVelocity(mFindPosCounter);
    }
    if (MR::isGreaterEqualStep(this, 1)) {
        keepVerticalizedVelocity();
        addVelocity(false);
        TVec3f offset;
        if (mHost->keepOffFromClosedArea(&offset)) {
            TPos3f mtx;
            u64* dst = reinterpret_cast< u64* >(&mtx);
            const u64* src = reinterpret_cast< const u64* >(&mHost->mBaseMtx);
            for (int i = 0; i < 6; ++i) {
                dst[i] = src[i];
            }
            TVec3f trans;
            mtx.getTrans(trans);
            TVec3f scaledOffset(offset);
            scaledOffset.scale(0.2f);
            trans.add(scaledOffset);
            mtx.setTrans(trans);
            mHost->setMtx(mtx);
        }
        mHost->snapToGround();
    }
    if (MR::isGreaterEqualStep(this, mFindPosCounter + 1)) {
        TVec3f toPlayer;
        MR::calcVecToPlayerH(&toPlayer, mHost, nullptr);
        turnToward(toPlayer, 1.0f);
        mHost->mVelocity.zero();
        mHost->endPlayerCenterCamera();
        setNerve(&DodoryuStateLv2NrvReadyChase::sInstance);
    }
}

void DodoryuStateLv2::exeHideAttack() {
    if (MR::isFirstStep(this)) {
        shiftHideAttack();
    }
    if (MR::isBckOneTimeAndStopped(mHost)) {
        startAnim(mChaseParam->_11C);
    }
    if (MR::calcDistanceToPlayer(mHost) > 1500.0f) {
        mHost->stopHill();
        startAnim(mChaseParam->_130);
        setNerve(&DodoryuStateLv2NrvChaseHide::sInstance);
        mHost->shiftMoveStateNull();
    }
}

void DodoryuStateLv2::exeChaseMoreStart() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_220);
        mHost->stopHillForce();
        mHost->shiftMoveStateNull();
        MR::startSound(mHost, "SE_BM_DODORYU_JUMP_OUT");
        TVec3f* playerPos = MR::getPlayerPos();
        TVec3f toPlayer(mHost->mPosition);
        toPlayer.sub(*playerPos);
        if (MR::isNearZero(toPlayer)) {
            MR::getPlayerFrontVec(&toPlayer);
        } else {
            MR::normalize(&toPlayer);
        }
        TVec3f vel(toPlayer);
        s16 bckFrames = MR::getBckCtrl(mHost)->mEnd;
        vel.scale(1500.0f / bckFrames);
        mHost->mVelocity.set(vel);
    }
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_SHOUT");
    if (MR::isStep(this, 40)) {
        MR::startSound(mHost, "SE_BM_DODORYU_LAND");
    }
    keepVerticalizedVelocity();
    addVelocity(true);
    TVec3f toPlayer;
    MR::calcVecToPlayerH(&toPlayer, mHost, nullptr);
    turnToward(toPlayer, 1.0f);
    if (MR::isBckOneTimeAndStopped(mHost)) {
        mHost->mVelocity.zero();
        setNerve(&DodoryuStateLv2NrvChaseMore::sInstance);
    }
}

void DodoryuStateLv2::exeChaseMore() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_234);
        determinChaseMoreAccelCounter();
        _E4 = true;
        mHost->startHill();
        shiftChaseInAnger();
        MR::shakeCameraInfinity(this, 0.3f, 0.1f);
        MR::startSound(mHost, "SE_BM_DODORYU_SAND_APPEAR");
    }
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_PANT");
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_RUN");
    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_CHASE");
    mHost->tryRumblePad();
    if (!_E4) {
        f32 velocityMag = mHost->mVelocity.length();
        f32 speedLimit = mChaseParam->_B8;
        if (velocityMag <= speedLimit) {
        }
    }
    _E0--;
    if (_E0 <= 0) {
        if (_E4) {
            determinChaseMoreBrakeCounter();
        } else {
            determinChaseMoreAccelCounter();
        }
        _E4 = !_E4;
    }
    s32 chaseTime = mChaseParam->_B0;
    if (MR::isGreaterEqualStep(this, chaseTime)) {
        setNerve(&DodoryuStateLv2NrvChaseMoreEnd::sInstance);
    }
}

void DodoryuStateLv2::endChaseMore() {
    MR::stopShakingCamera(this);
}

void DodoryuStateLv2::exeChaseMoreEnd() {
    if (MR::isFirstStep(this)) {
        startAnim(mChaseParam->_248);
        MR::startSound(mHost, "SE_BM_DODORYU_SAND_HIDE");
    }
    if (MR::isBckOneTimeAndStopped(mHost)) {
        setNerve(&DodoryuStateLv2NrvFindPos::sInstance);
    }
}

void DodoryuStateLv2::catchHipDrop() {
    if (!isEnableToCatchHipDrop()) {
        return;
    }
    mHost->mVelocity.zero();
    if (isNerve(&DodoryuStateLv2NrvChaseHide::sInstance)) {
        MR::emitEffect(mHost, "HipDropHit");
        MR::startSound(mHost, "SE_OJ_S_HIPDROP_HIT");
        return;
    }
    if (isNerve(&DodoryuStateLv2NrvReadyAppear::sInstance)) {
        hideJump();
        setNerve(&DodoryuStateLv2NrvChaseHide::sInstance);
        return;
    }
    if (isNerve(&DodoryuStateLv2NrvAppearSign::sInstance)) {
        hideJump();
        setNerve(&DodoryuStateLv2NrvChaseHide::sInstance);
        return;
    }
    if (isNerve(&DodoryuStateLv2NrvChaseAppearStart::sInstance)) {
        if (MR::isLessEqualStep(this, 15)) {
            hideJump();
            setNerve(&DodoryuStateLv2NrvChaseHide::sInstance);
            return;
        }
    }
    if (isNerve(&DodoryuStateLv2NrvEscape::sInstance)) {
        setNerve(&DodoryuStateLv2NrvEscapeJump::sInstance);
        return;
    }
    if (!isNerve(&DodoryuStateLv2NrvChaseMoreStart::sInstance) && !isNerve(&DodoryuStateLv2NrvChaseMore::sInstance) &&
        !isNerve(&DodoryuStateLv2NrvChaseMoreEnd::sInstance)) {
        bool useChaseMore = mChaseParam->_9C;
        if (useChaseMore) {
            mHost->reactJumpOutCommon();
            setNerve(&DodoryuStateLv2NrvChaseMoreStart::sInstance);
            return;
        }
    }
    mHost->reactJumpOutCommon();
    setNerve(&DodoryuStateLv2NrvJumpOut::sInstance);
}

void DodoryuStateLv2::catchAttackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayerOrRide(pReceiver)) {
        return;
    }

    if (isNerve(&DodoryuStateLv2NrvDive::sInstance)) {
        if (MR::isSensor(pSender, "RightHand") || MR::isSensor(pSender, "LeftHand") || MR::isSensor(pSender, "RightLeg") ||
            MR::isSensor(pSender, "LeftLeg") || MR::isSensor(pSender, "Head")) {
            return;
        }
    }

    if (!isAttackableNerve() || mPlayerStaggering) {
        if (mHost->isHeadNeedle(pSender, pReceiver)) {
            if (MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                return;
            }
        }

        MR::sendMsgPush(pReceiver, pSender);
    } else {
        attackStrongToDir(pSender, pReceiver);
    }
}

bool DodoryuStateLv2::catchPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isReflectSpinAttack()) {
        if (MR::isMsgPlayerSpinAttack(msg)) {
            if (MR::sendMsgEnemyAttackCounterSpin(pSender, pReceiver)) {
                mPlayerStaggering = true;
                return true;
            }
        }
    }
    if (!isDown()) {
        return false;
    }
    if (mHost->isHeadNeedle(pReceiver, pSender)) {
        return false;
    }
    if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
        return true;
    }
    if (MR::isMsgPlayerHitAll(msg)) {
        TVec3f attackDir;
        attackDir.set(pReceiver->mPosition);
        attackDir.sub(pSender->mPosition);
        _A0.set(attackDir);
        if (!calcVerticalizedDir(&_A0, _A0)) {
            TVec3f zDir;
            mHost->mBaseMtx.getZDir(zDir);
            _A0.negate();
        }
        TVec3f negDir(_A0);
        negDir.negate();
        turnToward(negDir, 1.0f);
        MR::tryRumblePadMiddle(this, 0);
        MR::stopScene(10);
        setNerve(&DodoryuStateLv2NrvKnockDown::sInstance);
        return true;
    }
    return true;
}

void DodoryuStateLv2::catchStartClipped() {
    MR::stopShakingCamera(mHost);
}

void DodoryuStateLv2::catchSendPlayerFlying(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isPlayerHipDropFalling()) {
        MR::sendMsgPush(pReceiver, pSender);
    } else {
        attackStrongToDir(pSender, pReceiver);
    }
}

void DodoryuStateLv2::knockOver() {
    TPos3f mtx(mHost->mBaseMtx);
    TVec3f trans;
    mtx.getTrans(trans);
    TVec3f knockDir(_A0);
    knockDir.scale(10.0f);
    trans.add(knockDir);
    mtx.setTrans(trans);
    mHost->setMtx(mtx);
    mHost->keepOffFromClosedArea(nullptr);
    mHost->snapToGround();
    calcVerticalizedDir(&_A0, _A0);
}

bool DodoryuStateLv2::isEnableToCatchHipDrop() const {
    return isNerve(&DodoryuStateLv2NrvChaseHide::sInstance) || isNerve(&DodoryuStateLv2NrvReadyAppear::sInstance) ||
           isNerve(&DodoryuStateLv2NrvChaseAppearStart::sInstance) || isNerve(&DodoryuStateLv2NrvChaseAppear::sInstance) ||
           isNerve(&DodoryuStateLv2NrvChaseAppearEnd::sInstance) || isNerve(&DodoryuStateLv2NrvEscape::sInstance) ||
           isNerve(&DodoryuStateLv2NrvChaseMoreStart::sInstance) || isNerve(&DodoryuStateLv2NrvChaseMore::sInstance) ||
           isNerve(&DodoryuStateLv2NrvChaseMoreEnd::sInstance);
}

bool DodoryuStateLv2::isDown() const {
    return isNerve(&DodoryuStateLv2NrvJumpOut::sInstance) || isNerve(&DodoryuStateLv2NrvEscape::sInstance) ||
           isNerve(&DodoryuStateLv2NrvEscapeJump::sInstance) || isNerve(&DodoryuStateLv2NrvRecover::sInstance) ||
           (isNerve(&DodoryuStateLv2NrvDive::sInstance) && MR::isLessEqualStep(this, 20));
}

bool DodoryuStateLv2::isAttackableNerve() const {
    return isNerve(&DodoryuStateLv2NrvChaseHide::sInstance) || isNerve(&DodoryuStateLv2NrvReadyAppear::sInstance) ||
           isNerve(&DodoryuStateLv2NrvChaseAppearStart::sInstance) || isNerve(&DodoryuStateLv2NrvChaseAppear::sInstance) ||
           isNerve(&DodoryuStateLv2NrvChaseAppearEnd::sInstance) || (isNerve(&DodoryuStateLv2NrvDive::sInstance) && MR::isGreaterStep(this, 20)) ||
           isNerve(&DodoryuStateLv2NrvChaseMoreStart::sInstance) || isNerve(&DodoryuStateLv2NrvChaseMore::sInstance) ||
           isNerve(&DodoryuStateLv2NrvChaseMoreEnd::sInstance);
}

bool DodoryuStateLv2::tryShiftToChaseAppear() {
    DodoryuChaseParam* pParam = mChaseParam;
    f32 dist = MR::calcDistanceToPlayer(mHost);
    f32 threshold = pParam->_14;
    if (dist < threshold) {
        startAnim(pParam->_108);
        setNerve(&DodoryuStateLv2NrvReadyAppear::sInstance);
        return true;
    }
    return false;
}

bool DodoryuStateLv2::tryShiftToChaseAppearEnd() {
    if (MR::isFirstStep(this)) {
        s32 minTime = mChaseParam->_4C;
        s32 maxTime = mChaseParam->_50;
        _BC = MR::getRandom(minTime, maxTime);
    }
    if (MR::isGreaterEqualStep(this, _BC)) {
        setNerve(&DodoryuStateLv2NrvChaseAppearEnd::sInstance);
        return true;
    }
    return false;
}

bool DodoryuStateLv2::isEscapeEnd() const {
    bool result = false;
    if (_CC <= 0) {
        f32 dist = MR::calcDistanceToPlayer(mHost);
        f32 threshold = mChaseParam->_5C;
        if (dist >= threshold || _C8 <= 0) {
            result = true;
        }
    }
    return result;
}

void DodoryuStateLv2::updateReadyChaseShake() {
    if (MR::isStep(this, 0)) {
        MR::shakeCameraInfinity(this, 0.3f, 0.1f);
    }
    if (MR::isGreaterEqualStep(this, 0)) {
        mHost->tryRumblePad();
    }
}

void DodoryuStateLv2::updateChaseHideShake() {
    if (MR::isStep(this, 0)) {
        MR::shakeCameraInfinity(this, 0.2f, 0.1f);
    }
    if (MR::isGreaterEqualStep(this, 0)) {
        mHost->tryRumblePad();
    }
    if (MR::isStep(this, 180)) {
        MR::stopShakingCamera(this);
    }
}

void DodoryuStateLv2::updateHideShake() {
    if (MR::isStep(this, 0)) {
        MR::shakeCameraInfinity(this, 0.3f, 0.05f);
    }
    if (MR::isGreaterEqualStep(this, 0)) {
        mHost->tryRumblePad();
    }
}

void DodoryuStateLv2::updateDiveShake() {
    if (MR::isStep(this, 40)) {
        MR::shakeCameraInfinity(this, 0.3f, 0.05f);
    }
    if (MR::isGreaterEqualStep(this, 40)) {
        mHost->tryRumblePad();
    }
}
void DodoryuStateLv2::calcRandomVelocity(s32 time) {
    TVec3f frontVec;
    MR::getPlayerFrontVec(&frontVec);
    f32 randomAngle = MR::getRandom() * 2.0f * PI;
    const TVec3f* gravityVec = MR::getPlayerGravity();
    TPos3f rotateMtx;
    rotateMtx.makeRotate(*gravityVec, randomAngle);
    rotateMtx.mult33(frontVec);
    f32 minDist = mChaseParam->_84;
    f32 maxDist = mChaseParam->_88;
    f32 dist = MR::getRandom(minDist, maxDist);
    f32 speed = dist / time;
    mHost->setMtx(MR::getPlayerBaseMtx());
    TVec3f vel(frontVec);
    vel.scale(speed);
    mHost->mVelocity.set(vel);
}
void DodoryuStateLv2::keepVerticalizedVelocity() {
    f32 velMag = mHost->mVelocity.length();
    TVec3f* pVel = &mHost->mVelocity;
    TVec3f* pGrav = &mHost->mGravity;
    f32 dotResult = pGrav->dot(*pVel);
    TVec3f newVel;
    JMAVECScaleAdd(pGrav, pVel, &newVel, -dotResult);
    newVel.setLength(velMag);
    mHost->mVelocity.set(newVel);
}

void DodoryuStateLv2::attackStrongToDir(HitSensor* pSender, HitSensor* pReceiver) {
    TVec3f dir;
    calcAttackDir(&dir, pSender->mPosition, pReceiver->mPosition);
    if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
        MR::shakeCameraNormal();
        if (isNerve(&DodoryuStateLv2NrvDive::sInstance)) {
            return;
        }
        bool isHideNerve = isNerve(&DodoryuStateLv2NrvChaseHide::sInstance) || isNerve(&DodoryuStateLv2NrvReadyAppear::sInstance) ||
                           isNerve(&DodoryuStateLv2NrvChaseAppearEnd::sInstance);
        if (isHideNerve) {
            setNerve(&DodoryuStateLv2NrvHideAttack::sInstance);
        } else {
            setNerve(&DodoryuStateLv2NrvLaugh::sInstance);
        }
    } else {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

void DodoryuStateLv2::hideJump() {
    startAnim(mChaseParam->_20C);
    MR::startSound(mHost, "SE_OJ_S_HIPDROP_HIT");
}

bool DodoryuStateLv2::tryToShiftAppearSign() {
    if (MR::isGreaterEqualStep(this, _B8)) {
        setNerve(&DodoryuStateLv2NrvAppearSign::sInstance);
        return true;
    }
    TPos3f mtx(mHost->mBaseMtx);
    TVec3f zDir;
    mtx.getZDir(zDir);
    f32 angle = mChaseParam->_38;
    if (!MR::isFaceToPlayerHorizontalDegree(mHost, zDir, angle)) {
        setNerve(&DodoryuStateLv2NrvAppearSign::sInstance);
        return true;
    }
    return false;
}

void DodoryuStateLv2::determinChaseMoreAccelCounter() {
    s32 minTime = mChaseParam->_BC;
    s32 maxTime = mChaseParam->_C0;
    _E0 = MR::getRandom(minTime, maxTime);
}

void DodoryuStateLv2::determinChaseMoreBrakeCounter() {
    s32 minTime = mChaseParam->_C4;
    s32 maxTime = mChaseParam->_C8;
    _E0 = MR::getRandom(minTime, maxTime);
}
void DodoryuStateLv2::turnToward(const TVec3f& rDir, f32 rate) {
    TVec3f yDir;
    mHost->mBaseMtx.getYDir(yDir);
    TVec3f zDir;
    mHost->mBaseMtx.getZDir(zDir);
    TVec3f trans;
    mHost->mBaseMtx.getTrans(trans);
    TPos3f mtx;
    calcLimitedRotateMtx(&mtx, zDir, rDir, rate);
    mtx.concat(mHost->mBaseMtx);
    mtx.setTrans(trans);
    mHost->setMtx(mtx);
}

void DodoryuStateLv2::addVelocity(bool snapToGround) {
    TPos3f mtx(mHost->mBaseMtx);
    TVec3f trans;
    mtx.getTrans(trans);
    trans.add(mHost->mVelocity);
    mtx.setTrans(trans);
    mHost->setMtx(mtx);
    if (snapToGround) {
        mHost->snapToGround();
    }
}

void DodoryuStateLv2::calcLimitedRotateMtx(TPos3f* pMtx, const TVec3f& rFrom, const TVec3f& rTo, f32 rate) {
    f32 maxAngle = rate * PI / 180.0f;
    TVec3f cross;
    PSVECCrossProduct(&rFrom, &rTo, &cross);
    f32 crossMag = cross.length();
    f32 dotResult = rFrom.dot(rTo);
    f32 angle = JMath::sAtanTable.atan2_(crossMag, dotResult);
    f32 absAngle = __fabsf(angle);
    f32 ratio = 1.0f;
    if (absAngle > maxAngle) {
        ratio = maxAngle / absAngle;
    }
    TQuat4f quat;
    quat.setRotate(rFrom, rTo, ratio);
    pMtx->makeQuat(quat);
}

bool DodoryuStateLv2::isReflectSpinAttack() const {
    return isNerve(&DodoryuStateLv2NrvChaseAppearStart::sInstance) || isNerve(&DodoryuStateLv2NrvChaseAppear::sInstance) ||
           isNerve(&DodoryuStateLv2NrvChaseAppearEnd::sInstance) || isNerve(&DodoryuStateLv2NrvChaseMoreStart::sInstance) ||
           isNerve(&DodoryuStateLv2NrvChaseMore::sInstance) || isNerve(&DodoryuStateLv2NrvChaseMoreEnd::sInstance);
}

void DodoryuStateLv2::shiftChaseUnderground() {
    f32 speed = mChaseParam->_8;
    f32 maxTurn = mChaseParam->_10;
    f32 accel = mChaseParam->_4;
    f32 brake = mChaseParam->_C;
    mHost->shiftMoveStateChase(speed, maxTurn, accel, brake);
}

void DodoryuStateLv2::shiftChaseExposingHead() {
    f32 speed = mChaseParam->_20;
    f32 maxTurn = mChaseParam->_28;
    f32 accel = mChaseParam->_1C;
    f32 brake = mChaseParam->_24;
    mHost->shiftMoveStateChase(speed, maxTurn, accel, brake);
}

void DodoryuStateLv2::shiftChaseExposingBust() {
    f32 speed = mChaseParam->_40;
    f32 maxTurn = mChaseParam->_48;
    f32 accel = mChaseParam->_3C;
    f32 brake = mChaseParam->_44;
    mHost->shiftMoveStateChase(speed, maxTurn, accel, brake);
}

void DodoryuStateLv2::shiftChaseInAnger() {
    f32 speed = mChaseParam->_A4;
    f32 maxTurn = mChaseParam->_AC;
    f32 accel = mChaseParam->_A0;
    f32 brake = mChaseParam->_A8;
    mHost->shiftMoveStateChase(speed, maxTurn, accel, brake);
}

void DodoryuStateLv2::shiftHideAttack() {
    f32 speed = mChaseParam->_8;
    f32 maxTurn = mChaseParam->_10;
    f32 accel = mChaseParam->_4;
    mHost->shiftMoveStateChase(speed, maxTurn, accel, 0.0f);
}

void DodoryuStateLv2::startAnim(const DodoryuAnimSet& rAnim) {
    if (rAnim.mBckName != nullptr) {
        MR::startBck(mHost, rAnim.mBckName, nullptr);
    }
    startBtk(rAnim);
    startBrk(rAnim);
    startBva(rAnim);
    startLeadHillBck(rAnim);
}

void DodoryuStateLv2::startBtk(const DodoryuAnimSet& rAnim) {
    if (rAnim.mBtkName != nullptr) {
        MR::startBtk(mHost, rAnim.mBtkName);
    }
}

void DodoryuStateLv2::startBrk(const DodoryuAnimSet& rAnim) {
    if (rAnim.mBrkName != nullptr) {
        MR::startBrk(mHost, rAnim.mBrkName);
    }
}

void DodoryuStateLv2::startBva(const DodoryuAnimSet& rAnim) {
    if (rAnim.mBvaName != nullptr) {
        MR::startBva(mHost, rAnim.mBvaName);
    }
}

void DodoryuStateLv2::startLeadHillBck(const DodoryuAnimSet& rAnim) {
    if (rAnim.mLeadHillBckName == nullptr) {
        mHost->killLeadHill();
    } else {
        mHost->startLeadHillBck(rAnim.mLeadHillBckName);
    }
}
