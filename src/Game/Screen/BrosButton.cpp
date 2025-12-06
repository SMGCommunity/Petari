#include "Game/Screen/BrosButton.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    NEW_NERVE(BrosButtonNrvAppear, BrosButton, Appear);
    NEW_NERVE(BrosButtonNrvSelect, BrosButton, Select);
    NEW_NERVE(BrosButtonNrvDecide, BrosButton, Decide);
    NEW_NERVE(BrosButtonNrvDisappear, BrosButton, Disappear);
};  // namespace

BrosButton::BrosButton(const char* pName) : LayoutActor(pName, true), mIsSelectedMario(true), mPaneCtrl(nullptr) {}

void BrosButton::init(const JMapInfoIter& rIter) {
    initLayoutManager("BrosButton", 1);

    mPaneCtrl = new ButtonPaneController(this, "BrosButton", "BoxBButton", 0, true);
    mPaneCtrl->_22 = false;

    MR::connectToSceneLayout(this);
    initNerve(&BrosButtonNrvAppear::sInstance);
}

void BrosButton::appear(bool isSelectedMario) {
    appear();
    setNerve(&BrosButtonNrvAppear::sInstance);

    mIsSelectedMario = isSelectedMario;

    mPaneCtrl->appear();
}

void BrosButton::disappear() {
    setNerve(&BrosButtonNrvDisappear::sInstance);
}

bool BrosButton::isSelected() const {
    return isNerve(&BrosButtonNrvDecide::sInstance);
}

bool BrosButton::isSelectedMario() const {
    return mIsSelectedMario;
}

void BrosButton::resume() {
    mPaneCtrl->forceToWait();
    setNerve(&BrosButtonNrvSelect::sInstance);
}

void BrosButton::exeAppear() {
    if (mPaneCtrl->isAppearing()) {
        setNerve(&BrosButtonNrvSelect::sInstance);
    }
}

void BrosButton::exeSelect() {
    mPaneCtrl->trySelect();

    if (!mPaneCtrl->mIsSelected) {
        return;
    }

    mIsSelectedMario = !mIsSelectedMario;

    if (mIsSelectedMario == true) {
        MR::stopSystemSE("SE_SY_FILE_SEL_SW_LUIGI", 0);
        MR::startSystemSE("SE_SY_FILE_SEL_SW_MARIO", -1, -1);
    } else {
        MR::stopSystemSE("SE_SY_FILE_SEL_SW_MARIO", 0);
        MR::startSystemSE("SE_SY_FILE_SEL_SW_LUIGI", -1, -1);
    }

    setNerve(&BrosButtonNrvDecide::sInstance);
}

void BrosButton::exeDecide() {}

void BrosButton::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mPaneCtrl->disappear();
    }

    if (mPaneCtrl->isHidden()) {
        kill();
    }
}

void BrosButton::appear() {
    LayoutActor::appear();
}

void BrosButton::control() {
    mPaneCtrl->update();

    if (mPaneCtrl->isPointingTrigger()) {
        MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON", -1, -1);
    }
}
