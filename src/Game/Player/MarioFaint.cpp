#include "Game/Player/MarioFaint.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioAccess.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Util/MathUtil.hpp"
#include "revolution/types.h"

bool Mario::doFlipWeak(const TVec3f& rVec) {
    if (mMovementStates._1B) {
        return false;
    }

    mFaint->mNoDamage = true;

    if (faint(rVec)) {
        if (isStatusActive(MarioStatus_Swim)) {
            mSwim->mDamageType = 1;
            mFaint->mNoDamage = false;
        }
        return true;
    }

    mFaint->mNoDamage = false;
    forceStopTornado();
    return false;
}

bool Mario::faint(const TVec3f& rVec) {
    _7C4 = rVec;
    if (!isEnableAddDamage()) {
        return false;
    }

    if (getCurrentStatus() == MarioStatus_Damage) {
        return false;
    }

    mFaint->setVec(rVec);
    stopWalk();
    forceStopTornado();
    mActor->damageDropThrowMemoSensor();

    if (getCurrentStatus() != MarioStatus_Faint) {
        mMovementStates._27 = true;
        return true;
    }

    return mMovementStates._27;
}

MarioFaint::MarioFaint(MarioActor* pActor) : MarioState(pActor, MarioStatus_Faint), mTimer(), _14(), _16() {
    _18.zero();
    mNoDamage = false;
    mTookDamage = false;
}

void MarioFaint::setVec(const TVec3f& rVec) {
    MR::vecKillElement(rVec, mActor->_240, &_18);
    _18.setLength(mActor->getConst().getTable()->mSlideDistFaint);
    mTimer = 0;
    _16 = 0;

    if (getPlayer()->isStatusActive(MarioStatus_Hang)) {
        _18.zero();
    }
}

bool MarioFaint::update() {
    mTimer++;

    switch (_16) {
    case 0:
        addVelocity(_18);
        _18.scale(mActor->getConst().getTable()->mFaintFriction1);
        if (mTimer == mActor->getConst().getTable()->mFaintTimer1) {
            _16++;
        }
        break;
    case 1:
        if (!getPlayer()->getMovementStates()._1) {
            return false;
        }

        addVelocity(_18);
        _18.scale(mActor->getConst().getTable()->mFaintFriction2);

        if (checkTrgA()) {
            getPlayer()->tryJump();
            return false;
        }

        if (mTimer == mActor->getConst().getTable()->mFaintTimer1 + mActor->getConst().getTable()->mFaintTimer2) {
            return false;
        }

        break;
    }

    return true;
}

bool MarioFaint::start() {
    mTimer = 0;
    _16 = 0;
    getPlayer()->mMovementStates._B = false;
    getPlayer()->mMovementStates.jumping = false;

    if (_18.dot(getPlayer()->mFrontVec) > 0.0f) {
        getPlayer()->setFrontVecKeepUp(_18);
        changeAnimation("後方小ダメージ", static_cast< const char* >(nullptr));
    } else {
        getPlayer()->setFrontVecKeepUp(-_18);
        changeAnimation("前方小ダメージ", static_cast< const char* >(nullptr));
    }

    if (mNoDamage) {
        changeAnimation("ノーダメージ", static_cast< const char* >(nullptr));
    }

    playSound("声小ダメージ");
    playSound("ダメージ");
    playEffect("ダメージ");
    startPadVib(2);

    addVelocity(_18);

    mTookDamage = !mNoDamage;

    if (!mNoDamage) {
        mActor->decLife(0);
        mActor->resetPlayerModeOnDamage();

        if (mActor->mHealth == 0) {
            if (!getPlayer()->getMovementStates()._1) {
                mActor->forceGameOverNonStop();
            } else {
                mActor->forceGameOver();
            }
        }

        return true;
    }

    mNoDamage = false;
    mActor->resetPlayerModeOnNoDamage();
    return true;
}

bool MarioFaint::close() {
    if(getPlayer()->getMovementStates()._1) {
        stopAnimation("後方小ダメージ");
        stopAnimation("前方小ダメージ","基本");
    }

    if(mTookDamage) {
        _14 = 120;
    }

    return true;
}
