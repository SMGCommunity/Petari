#include "Game/Player/MarioFlip.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioDamage.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Util/MathUtil.hpp"

bool Mario::doFlipJump(const TVec3f& rVec) {
    // FIXME: regswap
    if (isAnimationRun("壁はじき")) {
        return false;
    }

    stopWalk();
    TVec3f vec(rVec);
    vec.setLength(mActor->mConst->getTable()->mWallSpinFlipGround);
    vec -= getGravityVec()->scaleInline(mActor->mConst->getTable()->mWallSpinHopGround);
    tryForcePowerJump(vec, true);

    changeAnimation("壁はじき", static_cast< const char* >(nullptr));

    stopPunch();
    _430 = 3;
    mMovementStates._2B = true;
    startPadVib(2);

    return true;
}

bool Mario::doFlipBackRoll(const TVec3f& rVec) {
    TVec3f vec20;
    MR::vecKillElement(rVec, getAirGravityVec(), &vec20);

    if (doFlipLarge(vec20)) {
        checkDamage();
        changeAnimationNonStop("後転ふっとび");

        mDamage->setStrings("後転ふっとび空中", "後転ふっとび着地");

        TVec3f vec28;
        MR::vecKillElement(rVec, getAirGravityVec(), &vec28);
        setFrontVecKeepUp(-vec28, static_cast< u32 >(12l));

        return true;
    }

    return false;
}

bool MarioFlip::start() {
    _12 = 0;
    _14 = 0;
    _24 = 0.0f;
    _28 = -0.75f;

    changeAnimation("はねとばされ", static_cast< const char* >(nullptr));

    playSound("声小ダメージ", -1);
    playSound("壁衝突", -1);
    playEffect("ダメージ");
    startPadVib(2);

    addVelocity(_18);

    return true;
}

// bool MarioFlip::update() {}

bool Mario::doFlipRot(const TVec3f& rVec) {
    getCurrentStatus();
    if (getCurrentStatus() == MarioStatus_FireDamage) {
        return false;
    }

    if (mMovementStates._1B) {
        return false;
    }

    mFlip->setVec(rVec);

    stopJump();
    stopWalk();
    forceStopTornado();

    changeStatus(mFlip);

    return true;
}

bool Mario::doFlipLarge(const TVec3f& rVec) {
    if (getCurrentStatus() == MarioStatus_Damage) {
        return false;
    }

    if (getCurrentStatus() == MarioStatus_Flip) {
        return false;
    }

    if (getCurrentStatus() == MarioStatus_FireDamage) {
        return false;
    }

    if (mMovementStates._1B) {
        return false;
    }

    return flipLarge(rVec);
}

MarioFlip::MarioFlip(MarioActor* pActor) : MarioState(pActor, MarioStatus_Flip), _12(), _14() {
    _18.zero();
    _24 = 0.0f;
    _28 = 0.0f;
}

void MarioFlip::setVec(const TVec3f& rVec) {
    MR::vecKillElement(rVec, mActor->_240, &_18);

    if (mActor->_3E5) {
        _18.setLength(mActor->mConst->getTable()->mSlideDistFlipSpin);
    } else {
        _18.setLength(mActor->mConst->getTable()->mSlideDistFlip);
    }

    addVelocity(_18, 5.0f);

    _12 = 0;
    _14 = 0;
    _28 = -0.75f;

    if (getPlayer()->isStatusActive(MarioStatus_Hang)) {
        _18.zero();
        return;
    }

    getPlayer()->setFrontVecKeepUp(-_18);
}

bool MarioFlip::close() {
    setYangleOffset(0.0f);

    stopAnimation("はねとばされ");
    stopAnimation("はねとばされ終了");

    if (mActor->mHealth == 0) {
        if (!getPlayer()->getMovementStates()._1) {
            mActor->forceGameOverNonStop();
        } else {
            mActor->forceGameOver();
        }
    }

    return true;
}
