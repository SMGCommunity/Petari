#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioParalyze.hpp"
#include "Game/Player/MarioState.hpp"
#include "revolution/types.h"

bool Mario::doParalyze() {
    if (mMovementStates._1F) {
        return false;
    }

    if (isInvincible()) {
        return false;
    }

    if (getCurrentStatus() == MarioStatus_Paralyze) {
        return false;
    }

    if (getCurrentStatus() == MarioStatus_FireDamage) {
        return false;
    }

    if (mMovementStates._1B) {
        return false;
    }

    if (getDamageAfterTimer() != 0) {
        return false;
    }

    if (isDamaging()) {
        return false;
    }

    mActor->damageDropThrowMemoSensor();
    mActor->resetPlayerModeOnDamage();

    stopJump();
    stopWalk();

    changeStatus(mParalyze);
    return true;
}

MarioParalyze::MarioParalyze(MarioActor* pActor) : MarioState(pActor, MarioStatus_Paralyze), _12(), mTimer(), _16(), mNotDecLife() {
}

bool MarioParalyze::close() {
    if (mActor->mHealth == 0) {
        if (!getPlayer()->mMovementStates._1) {
            mActor->forceGameOverNonStop();
        } else {
            mActor->forceGameOver();
        }
        mActor->changeGameOverAnimation();
    }

    stopAnimation("電気ダメージ");
    stopEffect("ビリビリ");

    _16 = 120;

    if (!getPlayer()->isStatusActive(MarioStatus_Swim) && !getPlayer()->getMovementStates()._1) {
        getPlayer()->tryFreeJump(getFrontVec() * -10.0f, true);
    }

    return true;
}

bool MarioParalyze::start() {
    changeAnimationNonStop("電気ダメージ");
    playSound("ダメージ");
    playSound("電気ダメージ");
    playSound("声電気ダメージ");
    playEffect("ビリビリ");

    startPadVib(3);

    if (!mNotDecLife) {
        mActor->decLifeLarge();
    }

    mNotDecLife = false;
    mTimer = 60;
    _12 = 0;

    return true;
}

bool MarioParalyze::update() {
    if (mTimer != 0) {
        mTimer--;
        if (_12 != 0 && getPlayer()->getMovementStates()._1) {
            addVelocity(getFrontVec(), -3.0f);
        }
    }

    if (mTimer == 0) {
        if (_12 != 0) {
            return false;
        }

        _12 = 1;

        if (!getPlayer()->getMovementStates()._1) {
            mTimer = 10;
        } else {
            mTimer = 30;
        }

        if (getPlayer()->getMovementStates()._1) {
            changeAnimation("電気ダメージ終了", static_cast< const char* >(nullptr));
            playSound("声電気ダメージ終了");
        }

        if (mActor->mHealth == 0) {
            if (!getPlayer()->mMovementStates._1) {
                mActor->forceGameOverNonStop();
            } else {
                mActor->forceGameOver();
            }
            mActor->changeGameOverAnimation();
        }
    }

    return true;
}
