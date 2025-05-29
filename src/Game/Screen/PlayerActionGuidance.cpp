#include "Game/AreaObj/AreaObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/PlayerActionGuidance.hpp"
#include "Game/Screen/SimpleLayout.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sSuspendFrame = 60;
    static const s32 sChangeSoundFrame = 40;
};

namespace MR {
    PlayerActionGuidance* getPlayerActionGuidance() {
        return MR::getSceneObj<PlayerActionGuidance*>(SceneObj_PlayerActionGuidance);
    }
};

namespace {
    NEW_NERVE(PlayerActionGuidanceWaitFocusIn, PlayerActionGuidance, WaitFocusIn);
    NEW_NERVE(PlayerActionGuidanceFadein, PlayerActionGuidance, Fadein);
    NEW_NERVE(PlayerActionGuidanceDisplay, PlayerActionGuidance, Display);
    NEW_NERVE(PlayerActionGuidanceSuspend, PlayerActionGuidance, Suspend);
    NEW_NERVE(PlayerActionGuidanceFadeout, PlayerActionGuidance, Fadeout);
};

PlayerActionGuidance::PlayerActionGuidance() :
    LayoutActor("プレイヤーアクションガイダンス", true),
    mSpinLayout(NULL),
    mTamakoroLayout(NULL),
    mCurrentLayout(NULL),
    mGuidanceState(3),
    mGuidancePrevState(3),
    _34(false),
    _35(false),
    _36(false),
    _37(false)
{
    
}

void PlayerActionGuidance::control() {
    mGuidancePrevState = mGuidanceState;

    if (mSpinLayout != NULL && isInVolumePlayer("SpinGuidanceCube", false)) {
        mCurrentLayout = mSpinLayout;
        _35 = true;
    }
    else if (mTamakoroLayout != NULL && (_36 || isInVolumePlayer("TamakoroMoveGuidanceCube", true))) {
        mCurrentLayout = mTamakoroLayout;
        mGuidanceState = 3;
        _35 = true;
    }
    else if (mTamakoroLayout != NULL && (_36 || isInVolumePlayer("TamakoroJumpGuidanceCube", true))) {
        mCurrentLayout = mTamakoroLayout;
        mGuidanceState = 4;
        _35 = true;
    }
    else {
        _35 = false;
    }

    _37 = _36;
    _36 = false;
}

void PlayerActionGuidance::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayoutMovement(this);
    initNerve(&PlayerActionGuidanceWaitFocusIn::sInstance);
    startAnimAllLayout("Appear");
    setAnimFrameAndStopAllLayout(0.0f);
    hideAllLayout();

    mCurrentLayout = this;

    appear();
}

void PlayerActionGuidance::createSpinLayout() {
    if (mSpinLayout != NULL) {
        return;
    }

    mSpinLayout = new SimpleLayout("スピンガイダンス", "SpinGuidance", 1, -1);
    mSpinLayout->appear();
}

void PlayerActionGuidance::createTamakoroLayout() {
    if (mTamakoroLayout != NULL) {
        return;
    }

    mTamakoroLayout = new SimpleLayout("たまころガイダンス", "BallGuidance", 1, -1);
    mTamakoroLayout->appear();
}

void PlayerActionGuidance::exeWaitFocusIn() {
    if (MR::isFirstStep(this)) {
        hideAllLayout();
    }

    if (_35) {
        setNerve(&PlayerActionGuidanceSuspend::sInstance);
    }
}

void PlayerActionGuidance::exeFadein() {
    if (MR::isFirstStep(this)) {
        hideAllLayout();
        MR::showLayout(mCurrentLayout);
        startWaitAnimAllLayout();
        setAnimFrameAndStopAllLayout(0.0f);
        calcAnim();
        startAnimAllLayout("Appear");
    }

    if (MR::isAnimStopped(mCurrentLayout, 0)) {
        setNerve(&PlayerActionGuidanceDisplay::sInstance);
    }
}

void PlayerActionGuidance::startWaitAnimTamakoro() {
    if (mTamakoroLayout == NULL) {
        return;
    }

    if (isNerve(&PlayerActionGuidanceFadein::sInstance)) {
        MR::startAnim(mTamakoroLayout, "WaitRotation", 0);
    }

    switch (mGuidanceState) {
    case 0:
        MR::startAnim(mTamakoroLayout, "WaitRaise", 0);
        break;
    case 1:
        MR::startAnim(mTamakoroLayout, "WaitRaiseHold", 0);
        break;
    case 2:
        MR::startAnim(mTamakoroLayout, "WaitRaiseOK", 0);
        break;
    case 3:
        MR::startAnim(mTamakoroLayout, "WaitRotation", 0);
        break;
    case 4:
        MR::startAnim(mTamakoroLayout, "WaitAButton", 0);
        break;
    }
}

void PlayerActionGuidance::exeDisplay() {
    if (MR::isFirstStep(this)) {
        startWaitAnimAllLayout();

        if (mSpinLayout != NULL) {
            MR::startCSSound("CS_NOTICE_USE_DPD", "SE_SY_CS_NOTICE_USE_DPD", 0);
        }

        if (mTamakoroLayout != NULL && mGuidanceState == 0) {
            MR::startSystemSE("SE_SY_CTRL_GUIDE_APPEAR", -1, -1);
        }
    }

    if (mGuidanceState == 0) {
        if (!MR::isFirstStep(this) && MR::getAnimFrame(mCurrentLayout, 0) == 0.0f) {
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE", 500, -1);
        }

        if (MR::getAnimFrame(mCurrentLayout, 0) == sChangeSoundFrame) {
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE2", 1000, -1);
        }
    }

    if (mGuidancePrevState != mGuidanceState) {
        startWaitAnimTamakoro();
    }

    if (MR::isAnimStopped(mCurrentLayout, 0) || !_35) {
        setNerve(&PlayerActionGuidanceFadeout::sInstance);
    }
}

void PlayerActionGuidance::exeSuspend() {
    if (MR::isStep(this, sSuspendFrame)) {
        if (!_35) {
            hideAllLayout();
            setNerve(&PlayerActionGuidanceWaitFocusIn::sInstance);
        }
        else {
            setNerve(&PlayerActionGuidanceFadein::sInstance);
        }
    }
}

void PlayerActionGuidance::exeFadeout() {
    if (MR::isFirstStep(this)) {
        startAnimAllLayout("End");
    }

    if (MR::isAnimStopped(mCurrentLayout, 0)) {
        if (_35) {
            setNerve(&PlayerActionGuidanceSuspend::sInstance);
        }
        else {
            hideAllLayout();
            setNerve(&PlayerActionGuidanceWaitFocusIn::sInstance);
        }
    }
}

void PlayerActionGuidance::startAnimAllLayout(const char* pAnimName) {
    if (mSpinLayout != NULL) {
        MR::startAnim(mSpinLayout, pAnimName, 0);
    }

    if (mTamakoroLayout != NULL) {
        MR::startAnim(mTamakoroLayout, pAnimName, 0);
    }
}

void PlayerActionGuidance::startWaitAnimAllLayout() {
    if (mSpinLayout != NULL) {
        MR::startAnim(mSpinLayout, "Wait", 0);
    }

    if (mTamakoroLayout != NULL && mCurrentLayout == mTamakoroLayout) {
        startWaitAnimTamakoro();
    }
}

void PlayerActionGuidance::setAnimFrameAndStopAllLayout(f32 animFrame) {
    if (mSpinLayout != NULL) {
        MR::setAnimFrameAndStop(mSpinLayout, animFrame, 0);
    }

    if (mTamakoroLayout != NULL) {
        MR::setAnimFrameAndStop(mTamakoroLayout, animFrame, 0);
    }
}

void PlayerActionGuidance::hideAllLayout() {
    if (mSpinLayout != NULL) {
        MR::hideLayout(mSpinLayout);
    }

    if (mTamakoroLayout != NULL) {
        MR::hideLayout(mTamakoroLayout);
    }
}

bool PlayerActionGuidance::isInVolumePlayer(const char* pObjName, bool param2) const {
    if (_34) {
        return false;
    }

    AreaObj* pObj = MR::getAreaObj(pObjName, *MR::getPlayerPos());

    if (pObj == NULL) {
        return false;
    }

    if (pObj->isValidSwitchA()) {
        if (pObj->isValidSwitchB() && pObj->isOnSwitchB()) {
            return false;
        }

        if (param2) {
            return pObj->isOnSwitchA() == true;
        }
        else {
            return pObj->isOnSwitchA() == false;
        }
    }

    return true;
}
