#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Player/MarioFpView.hpp"
#include "Game/Player/RushEndInfo.hpp"

void MarioActor::forceKill(u32 a1) {
    if (!mMario->mMovementStates._1F && isEnableNerveChange()) {
        _A6E = false;
        if (!MR::isDead(_994)) {
                _994->kill();
        }
        if (mMario->isStatusActive(0x12)) {
            mMario->mFpView->forceClose();
            mMario->closeStatus(nullptr);
        }
        if (_934) {
            if (_924 && _924->isType(0x6b)) {
                _B91 = true;
                _B90 = true;
            }
            if (_934) {
                if (!_924->receiveMessage(0x93, getSensor("body"))) {
                    _924->receiveMessage(0x94, getSensor("body"));
                }
                if (_934) {
                    RushEndInfo stack_14 = RushEndInfo(nullptr, 4, TVec3f(0.0f, 0.0f, 0.0f), false, 0);
                    endRush(&stack_14);
                }
            }
            _934 = false;
        }
        if (a1 == 0) {
            mMario->doAbyssDamage();
            return;
        }
        if (a1 == 0x3) {
            mMario->startPadVib(3);
        }
        if (a1 == 0x4) {
            mMario->startPadVib(3);
            initDarkMask();
            mMario->doDarkDamage();
            return;
        }
        switch (a1) {
            case 5:
            case 1:
            case 2:
                MR::deactivateDefaultGameLayout();
            default:
                break;
        }
        _39D = a1;
        mHealth = 0;
        setNerve(&NrvMarioActor::MarioActorNrvGameOver::sInstance);
    }
}

void MarioActor::exeGameOver() {
    mVelocity.zero();
    calcHeadPos();
    updateRealMtx();
    if (getNerveStep() == 0x20) {
        _A6E = false;
    }
    _468.y = 0;
    PSMTXIdentity(mMarioAnim->_7C);
    damageDropThrowMemoSensor();
    if (MR::isFirstStep(this)) {
        MR::setCubeBgmChangeInvalid();
        MR::clearBgmQueue();
        if (MR::getPlayerLeft() == 0) {
            MR::startPlayerEvent("ゲームオーバー");
            changeGameOverAnimation();
        }
        else {
            switch (_39D) {
                case 2:
                    MR::startPlayerEvent("ゴーストレース負け");
                    break;
                case 1:
                    MR::startPlayerEvent("レース負け");
                    changeGameOverAnimation();
                    break;
                case 5:
                    MR::startPlayerEvent("レース負け");
                    break;
                default:
                    MR::startPlayerEvent("マリオダウン");
                    break;
            }
        }
        MR::startGlobalEventCameraNoTarget("昇天カメラ", -1);
        MR::startStarPointerModeDemoMarioDeath(this);
    }
}

void MarioActor::exeGameOverAbyss() {
    mVelocity = getLastMove() * 0.99f + mCamDirZ * 35.0f * 0.01f;
    mLastMove = mVelocity;
    if (MR::isFirstStep(this)) {
        MR::setCubeBgmChangeInvalid();
        MR::clearBgmQueue();
        MR::startGlobalEventCameraNoTarget("奈落カメラ", -1);
        _F44 = false;
        if (MR::getPlayerLeft() == 0) {
            MR::startPlayerEvent("ゲームオーバー");
        }
        else {
            MR::startPlayerEvent("マリオ奈落");
        }
        MR::startStarPointerModeDemoMarioDeath(this);
        mMario->changeAnimationNonStop("奈落ダウン");
        _B90 = true;
    }
}

void MarioActor::exeGameOverFire() {
    mVelocity.zero();
    calcHeadPos();
    updateRealMtx();
    if (getNerveStep() == 0x78) {
        _A6E = false;
    }
    if (MR::isFirstStep(this)) {
        MR::setCubeBgmChangeInvalid();
        MR::clearBgmQueue();
        if (MR::getPlayerLeft() == 0) {
            MR::startPlayerEvent("ゲームオーバー");
            MR::startGlobalEventCameraNoTarget("昇天カメラ", -1);
        }
        else {
            MR::startPlayerEvent("マリオ炎ダウン");
            MR::startGlobalEventCameraNoTarget("奈落カメラ", -1);
        }
        MR::startStarPointerModeDemoMarioDeath(this);
    }
}

void MarioActor::exeGameOverSink() {
    mVelocity.zero();
    calcHeadPos();
    updateRealMtx();
    if (getNerveStep() == 0x78) {
        _A6E = false;
    }
    if (MR::isFirstStep(this)) {
        MR::setCubeBgmChangeInvalid();
        MR::clearBgmQueue();
        changeGameOverAnimation();
        if (MR::getPlayerLeft() == 0) {
            MR::startPlayerEvent("ゲームオーバー");
            MR::startGlobalEventCameraNoTarget("昇天カメラ", -1);
        }
        else {
            MR::startPlayerEvent("マリオダウン");
            MR::startGlobalEventCameraNoTarget("奈落カメラ", -1);
        }
        MR::startStarPointerModeDemoMarioDeath(this);
        if (mMario->_960 == 0x19 || mMario->_960 == 0x11) {
            playEffect("砂埋まり体");
            playEffect("砂埋まり手");
        }
        if (mMario->_960 == 0x1f) {
            playEffect("泥埋まり体");
            playEffect("泥埋まり手");
        }
    }
    if (mMario->_960 == 0x1f || mMario->_960 == 0x12) {
        playSound("沼死亡", -1);
        if (getNerveStep() == 0x5a) {
            playSound("声沼沈み死亡", -1);
        }
    }
    else {
        playSound("砂死亡", -1);
        if (getNerveStep() == 0x5a) {
            playSound("声砂沈み死亡", -1);
        }
    }
}

void MarioActor::exeGameOverNonStop() {
    if (MR::isFirstStep(this)) {
        MR::setCubeBgmChangeInvalid();
        MR::clearBgmQueue();
        if (MR::getPlayerLeft() == 0) {
            MR::startPlayerEvent("ゲームオーバー");
            MR::startGlobalEventCameraNoTarget("昇天カメラ", -1);
        }
        else {
            MR::startPlayerEvent("マリオ炎ダウン");
            MR::startGlobalEventCameraNoTarget("奈落カメラ", -1);
        }
        MR::startStarPointerModeDemoMarioDeath(this);
    }
}

void MarioActor::exeTimeWait() {
    if (_FB8 == getNerveStep()) {
        setNerve(_FB4);
        _FB4 = nullptr;
    }
}
