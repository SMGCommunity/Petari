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

BrosButton::BrosButton(const char* pName) : LayoutActor(pName, true), mIsSelectedMario(true), mPaneCtrl(nullptr) {
}

void BrosButton::init(const JMapInfoIter& rIter) {
    initLayoutManager("BrosButton", 1);

    mPaneCtrl = new ButtonPaneController(this, "BrosButton", "BoxBButton", 0, true);
    mPaneCtrl->_22 = false;

    MR::connectToSceneLayout(this);
    initNerve(GET_NERVE_ANON(BrosButtonNrvAppear));
}

void BrosButton::appear(bool isSelectedMario) {
    appear();
    setNerve(GET_NERVE_ANON(BrosButtonNrvAppear));

    mIsSelectedMario = isSelectedMario;

    mPaneCtrl->appear();
}

void BrosButton::disappear() {
    setNerve(GET_NERVE_ANON(BrosButtonNrvDisappear));
}

bool BrosButton::isSelected() const {
    return isNerve(GET_NERVE_ANON(BrosButtonNrvDecide));
}

bool BrosButton::isSelectedMario() const {
    return mIsSelectedMario;
}

void BrosButton::resume() {
    mPaneCtrl->forceToWait();
    setNerve(GET_NERVE_ANON(BrosButtonNrvSelect));
}

void BrosButton::exeAppear() {
    if (mPaneCtrl->isAppearing()) {
        setNerve(GET_NERVE_ANON(BrosButtonNrvSelect));
    }
}

void BrosButton::exeSelect() {
    mPaneCtrl->trySelect();

    if (!mPaneCtrl->mIsSelected) {
        return;
    }

    mIsSelectedMario = !mIsSelectedMario;

    if (mIsSelectedMario == true) {
        MR::stopSystemSE("SE_SY_FILE_SEL_SW_LUIGI");
        MR::startSystemSE("SE_SY_FILE_SEL_SW_MARIO");
    } else {
        MR::stopSystemSE("SE_SY_FILE_SEL_SW_MARIO");
        MR::startSystemSE("SE_SY_FILE_SEL_SW_LUIGI");
    }

    setNerve(GET_NERVE_ANON(BrosButtonNrvDecide));
}

void BrosButton::exeDecide() {
}

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
        MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON");
    }
}
