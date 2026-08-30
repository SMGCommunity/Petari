#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioFreeze.hpp"

bool Mario::doFreeze() {
    if (mMovementStates._1F) {
        return false;
    }

    if (isInvincible()) {
        return false;
    }

    if (getCurrentStatus() == MarioStatus_Freeze) {
        return false;
    }

    if (mFreeze->_1C != 0) {
        return false;
    }

    mActor->resetPlayerModeOnDamage();

    stopJump();
    stopWalk();

    mActor->damageDropThrowMemoSensor();
    cancelSquatMode();

    stopAnimationUpper(nullptr, nullptr);
    changeStatus(mFreeze);
    return true;
}

MarioFreeze::MarioFreeze(MarioActor* pActor) : MarioState(pActor, MarioStatus_Freeze), mIsFrozen(), _14(), _18(), mFreezeTimer(), _1C() {
}

bool MarioFreeze::close() {
    _1C = 120;

    if (mIsFrozen) {
        playSound("氷ダメージ終了");
        mActor->hideFreezeModel();
    }

    return true;
}

bool MarioFreeze::notice() {
    if (mActor->mHealth == 0) {
        if (getNoticedStatus() == MarioStatus_Swim) {
            mActor->_B90 = true;
            mActor->forceGameOver();
        }
        return true;
    }
    return false;
}

bool MarioFreeze::start() {
    changeAnimationNonStop("氷結");

    playSound("声氷ダメージ");
    playSound("氷ダメージ");
    playSound("ダメージ");

    startPadVib(3);
    mActor->decLife(0);

    mFreezeTimer = 180;
    _18 = 0;
    _14 = 0.0f;

    getPlayer()->resetInline();

    mActor->showFreezeModel();

    mIsFrozen = true;

    return true;
}

bool MarioFreeze::update() {
    if (mFreezeTimer != 0) {
        mFreezeTimer--;

        if (_18 != 0) {
            if (getPlayer()->getMovementStates()._1) {
                addVelocity(getFrontVec(), -1.0f);
            }
        } else if (mFreezeTimer < 120 && mActor->mHealth != 0 && mActor->isRequestSpin()) {
            addVelocity(getFrontVec(), -10.0f);
            changeAnimation("地上ひねり", static_cast< const char* >(nullptr));

            playSound("声スピン");
            playSound("スピンジャンプ");
            playSound("氷ダメージ終了");

            mActor->hideFreezeModel();
            mIsFrozen = false;
            return false;
        }
    }

    if (mFreezeTimer == 0) {
        if (_18 != 0) {
            if (mActor->mHealth != 0) {
                playSound("声氷ダメージ終了");
                return false;
            }
        } else {
            _18 = 1;
            if (!getPlayer()->getMovementStates()._1) {
                mFreezeTimer = 10;
            } else {
                mFreezeTimer = 20;
            }

            if (mActor->mHealth == 0) {
                if (!getPlayer()->getMovementStates()._1) {
                    mActor->forceGameOverNonStop();
                } else {
                    mActor->forceGameOver();
                }
            } else if (getPlayer()->getMovementStates()._1) {
                changeAnimation("氷結解除", static_cast< const char* >(nullptr));
                playSound("氷ダメージ終了");
                mActor->hideFreezeModel();
                mIsFrozen = false;
            }
        }
    }

    if (mFreezeTimer < 150) {
        if (!getPlayer()->getMovementStates()._1 || _14 < 0.0f) {
            getPlayer()->setJumpVec(getGravityVec() * _14);
            addVelocity(getGravityVec() * _14);
            _14 += 1.5f;
            if (_14 < 0.0f) {
                getPlayer()->mMovementStates._1 = false;
            }
        } else if (_14 > 0.0f) {
            if (_14 < 8.0f) {
                _14 = 0.0f;
            } else {
                _14 = 0.4f * -_14;
                getPlayer()->mMovementStates._1 = false;
                getPlayer()->setJumpVec(getGravityVec() * _14);
            }
        }
    }

    return true;
}
