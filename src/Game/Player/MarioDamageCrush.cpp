#include "Game/Player/MarioCrush.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"

bool Mario::requestCrush() {
    _10._18 = 1;
    return true;
}

bool Mario::tryCrush() {
    if (mMovementStates._1F) {
        return false;
    }
    if (isInvincible()) {
        return false;
    }
    if (mMovementStates._0 || !mMovementStates._1) {
        setTrans(mShadowPos, nullptr);
    }
    if (getCurrentStatus() == 0xf) {
        return false;
    }
    mActor->damageDropThrowMemoSensor();
    mActor->resetPlayerModeOnDamage();
    stopJump();
    stopWalk();
    changeStatus(mCrush);
    return true;
}

MarioCrush::MarioCrush(MarioActor *pActor) : MarioState(pActor, 0xf), _12(0), _14(0) {

}

bool MarioCrush::start() {
    changeAnimationNonStop("しびれ");
    startPadVib("マリオ[しびれ]");
    playSound("声しびれ", -1);
    getPlayer()->mMovementStates._3C = 1;
    mActor->mScale.set(1.0f, 0.2f, 1.0f);
    startPadVib(0x3);
    mActor->decLifeLarge();
    if (mActor->mHealth == 0) {
        mActor->forceGameOver();
    }
    _14 = 0xb4;
    _12 = 0;
    return true;
}

bool MarioCrush::close() {
    getPlayer()->mMovementStates._3C = 1;
    mActor->mScale.set(1.0f, 1.0f, 1.0f);
    stopAnimation("しびれ", (char*)nullptr);
    Mario* pMario = getPlayer();
    u16 temp = 0x78;
    pMario->_41E = temp;
    return true;
}

bool MarioCrush::update() {
    if (_14) {
        _14--;
    }
    if (_14 == 0) {
        if (_12) {
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
            changeAnimation("しびれ回復", (char*)nullptr);
        }
    }
    if (_12) {
        if (mActor->isRequestRush() || checkTrgA()) {
            stopAnimation(nullptr, (char*)nullptr);
            if (checkTrgA()) {
                getPlayer()->tryJump();
            }
            return false;
        }
    }
    return true;
}
