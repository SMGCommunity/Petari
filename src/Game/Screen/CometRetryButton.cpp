#include "Game/Screen/CometRetryButton.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/PlayerMissLeft.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace NrvCometRetryButton {
    NEW_NERVE(HostTypeNrvMissLeftWait, CometRetryButton, MissLeftWait);
    NEW_NERVE(HostTypeNrvWait, CometRetryButton, Wait);
    NEW_NERVE(HostTypeNrvDecided, CometRetryButton, Decided);
    NEW_NERVE(HostTypeNrvEndRetry, CometRetryButton, End);
    NEW_NERVE(HostTypeNrvEndGoBackAstroDome, CometRetryButton, End);
};  // namespace NrvCometRetryButton

CometRetryButton::CometRetryButton(const char* pName)
    : LayoutActor(pName, true), mButtonYesPaneCtrl(nullptr), mButtonNoPaneCtrl(nullptr), mPlayerMissLeft(nullptr) {}

// FIXME: There are a lot of register swaps.
void CometRetryButton::init(const JMapInfoIter& rIter) {
    initLayoutManager("RetryButton", 1);
    MR::connectToSceneLayout(this);
    initNerve(&NrvCometRetryButton::HostTypeNrvMissLeftWait::sInstance);
    MR::createAndAddPaneCtrl(this, "NBackUp", 1);
    MR::createAndAddPaneCtrl(this, "NBackDown", 1);

    mButtonYesPaneCtrl = new ButtonPaneController(this, "NBackUp", "BoxButton1", 0, true);
    mButtonNoPaneCtrl = new ButtonPaneController(this, "NBackDown", "BoxButton2", 0, true);
    mButtonYesPaneCtrl->_22 = false;
    mButtonNoPaneCtrl->_22 = false;
    mButtonYesPaneCtrl->mAppearAnimName = "ButtonAppear";
    mButtonYesPaneCtrl->mWaitAnimName = "ButtonWait";
    mButtonYesPaneCtrl->mPointingAnimName = "ButtonSelectIn";
    mButtonYesPaneCtrl->mDecideAnimName = "ButtonDecide";
    mButtonYesPaneCtrl->mNotPointingAnimName = "ButtonSelectOut";
    mButtonYesPaneCtrl->mDisappearAnimName = "ButtonEnd";
    mButtonNoPaneCtrl->mAppearAnimName = "ButtonAppear";
    mButtonNoPaneCtrl->mWaitAnimName = "ButtonWait";
    mButtonNoPaneCtrl->mPointingAnimName = "ButtonSelectIn";
    mButtonNoPaneCtrl->mDecideAnimName = "ButtonDecide";
    mButtonNoPaneCtrl->mNotPointingAnimName = "ButtonSelectOut";
    mButtonNoPaneCtrl->mDisappearAnimName = "ButtonEnd";

    mPlayerMissLeft = new PlayerMissLeft("ミス時のプレイヤー残機表示");
    mPlayerMissLeft->initWithoutIter();
    mPlayerMissLeft->kill();

    kill();
}

void CometRetryButton::appear() {
    LayoutActor::appear();
    setNerve(&NrvCometRetryButton::HostTypeNrvMissLeftWait::sInstance);
    MR::deactivateDefaultGameLayout();
}

void CometRetryButton::kill() {
    LayoutActor::kill();
    MR::activateDefaultGameLayout();
    MR::forceCloseWipeCircle();
}

void CometRetryButton::control() {
    mButtonYesPaneCtrl->update();
    mButtonNoPaneCtrl->update();

    if (mButtonYesPaneCtrl->isPointingTrigger() || mButtonNoPaneCtrl->isPointingTrigger()) {
        MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON", -1, -1);
    }

    if (mPlayerMissLeft != nullptr) {
        mPlayerMissLeft->movement();
        mPlayerMissLeft->calcAnim();
    }
}

void CometRetryButton::exeMissLeftWait() {
    if (MR::isFirstStep(this)) {
        mPlayerMissLeft->appear();
    }

    if (MR::isDead(mPlayerMissLeft)) {
        mButtonYesPaneCtrl->appear();
        mButtonNoPaneCtrl->appear();
        setNerve(&NrvCometRetryButton::HostTypeNrvWait::sInstance);
    }
}

void CometRetryButton::exeWait() {
    MR::requestStarPointerModePauseMenu(this);

    if (mButtonYesPaneCtrl->trySelect() || mButtonNoPaneCtrl->trySelect()) {
        if (mButtonYesPaneCtrl->mIsSelected) {
            MR::startSystemSE("SE_SY_TALK_SELECT_YES", -1, -1);
        } else if (mButtonNoPaneCtrl->mIsSelected) {
            MR::startSystemSE("SE_SY_TALK_SELECT_NO", -1, -1);
        }

        setNerve(&NrvCometRetryButton::HostTypeNrvDecided::sInstance);
    }
}

void CometRetryButton::exeDecided() {
    MR::requestStarPointerModePauseMenu(this);

    if (mButtonYesPaneCtrl->mIsSelected) {
        if (mButtonYesPaneCtrl->isDecidedWait()) {
            setNerve(&NrvCometRetryButton::HostTypeNrvEndRetry::sInstance);
        }
    } else if (mButtonNoPaneCtrl->mIsSelected) {
        if (mButtonNoPaneCtrl->isDecidedWait()) {
            setNerve(&NrvCometRetryButton::HostTypeNrvEndGoBackAstroDome::sInstance);
        }
    }
}

void CometRetryButton::exeEnd() {
    MR::requestStarPointerModePauseMenu(this);

    if (MR::isFirstStep(this)) {
        mButtonYesPaneCtrl->disappear();
        mButtonNoPaneCtrl->disappear();
    }

    if (mButtonYesPaneCtrl->isHidden() && mButtonNoPaneCtrl->isHidden()) {
        if (isNerve(&NrvCometRetryButton::HostTypeNrvEndRetry::sInstance)) {
            MR::requestChangeStageAfterMiss();
        } else {
            MR::requestChangeStageGoBackAstroDome();
        }

        kill();
    }
}

void CometRetryButton::draw() const {
    LayoutActor::draw();

    if (mPlayerMissLeft != nullptr) {
        mPlayerMissLeft->draw();
    }
}
