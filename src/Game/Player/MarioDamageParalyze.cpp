#include "Game/Player/MarioParalyze.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "JSystem/JMath/JMath.hpp"

bool Mario::doParalyze() {
    if (mMovementStates._1F) {
        return false;
    }
    if (isInvincible()) {
        return false;
    }
    if (getCurrentStatus() == 0xb) {
        return false;
    }
    if (getCurrentStatus() == 0x8) {
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
    else {
        mActor->damageDropThrowMemoSensor();
        mActor->resetPlayerModeOnDamage();
        stopJump();
        stopWalk();
        changeStatus(mParalyze);
        return true;
    }
}

MarioParalyze::MarioParalyze(MarioActor *pActor) : MarioState(pActor, 0xb), _12(0), _14(0), _16(0), _18(false) {

}

bool MarioParalyze::start() {
    changeAnimationNonStop("電気ダメージ");
    playSound("ダメージ", -1);
    playSound("電気ダメージ", -1);
    playSound("声電気ダメージ", -1);
    playEffect("ビリビリ");
    startPadVib(3);
    if (!_18) {
        mActor->decLifeLarge();
    }
    _18 = false;
    _14 = 0x3c;
    _12 = 0;
    return true;
}

bool MarioParalyze::close() {
    if (mActor->mHealth == 0) {
        if (!getPlayer()->mMovementStates._1) {
            mActor->forceGameOverNonStop();
        }
        else {
            mActor->forceGameOver();
        }
        mActor->changeGameOverAnimation();
    }
    stopAnimation("電気ダメージ", (char*)nullptr);
    stopEffect("ビリビリ");
    _16 = 0x78;
    if (!getPlayer()->isStatusActive(6) && !getPlayer()->mMovementStates._1) {
        float scale = -10.0f;
        TVec3f stack_8; // bad instruction position
        JMathInlineVEC::PSVECCopy(&getFrontVec(), &stack_8);
        stack_8.mult(scale);
        getPlayer()->tryFreeJump(stack_8, true);
    }
    return true;
}

bool MarioParalyze::update() {
    if (_14 != 0) {
        _14--;
        if (_12 != 0 && getPlayer()->mMovementStates._1) {
            addVelocity(getFrontVec(), -3.0f);
        }
    }
    if (_14 == 0) {
        if (_12 != 0) {
            return false;
        }
        _12 = 1;
        if (!getPlayer()->mMovementStates._1) {
            _14 = 0xa;
        }
        else {
            _14 = 0x1e;
        }
        if (getPlayer()->mMovementStates._1) {
            changeAnimation("電気ダメージ終了", (char*)nullptr);
            playSound("声電気ダメージ終了", -1);
        }
        if (mActor->mHealth == 0) {
            if (!getPlayer()->mMovementStates._1) {
                mActor->forceGameOverNonStop();
            }
            else {
                mActor->forceGameOver();
            }
            mActor->changeGameOverAnimation();
        }
    }
    return true;
}
