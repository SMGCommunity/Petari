#include "Game/Screen/PictureBookCloseButton.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvPictureBookCloseButton {
    NEW_NERVE(PictureBookCloseButtonNrvSelect, PictureBookCloseButton, Select);
    NEW_NERVE(PictureBookCloseButtonNrvSelected, PictureBookCloseButton, Selected);
    NEW_NERVE(PictureBookCloseButtonNrvNotSelected, PictureBookCloseButton, NotSelected);
};  // namespace NrvPictureBookCloseButton

PictureBookCloseButton::PictureBookCloseButton(bool canCloseWithPad)
    : LayoutActor("絵本閉じるボタン", true), mPaneCtrl(nullptr), mCanCloseWithPad(canCloseWithPad) {
}

void PictureBookCloseButton::init(const JMapInfoIter& rIter) {
    initLayoutManager("BackButton", 1);
    MR::connectToSceneLayoutDecoration(this);

    mPaneCtrl = new ButtonPaneController(this, "Back", "BoxButton", 0, true);

    initNerve(GET_NERVE(PictureBookCloseButton, PictureBookCloseButtonNrvSelect));
}

void PictureBookCloseButton::appear() {
    LayoutActor::appear();
    mPaneCtrl->appear();
    MR::requestMovementOn(this);
    setNerve(GET_NERVE(PictureBookCloseButton, PictureBookCloseButtonNrvSelect));
}

void PictureBookCloseButton::disappear() {
    mPaneCtrl->disappear();
    setNerve(GET_NERVE(PictureBookCloseButton, PictureBookCloseButtonNrvNotSelected));
}

bool PictureBookCloseButton::trySelect() {
    if (mPaneCtrl->trySelect()) {
        MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL");
        MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
        setNerve(GET_NERVE(PictureBookCloseButton, PictureBookCloseButtonNrvSelected));

        return true;
    }

    if (!mPaneCtrl->isAppearing() && MR::testCorePadTriggerB(WPAD_CHAN0) && mCanCloseWithPad) {
        mPaneCtrl->disappear();
        MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL");
        MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
        setNerve(GET_NERVE(PictureBookCloseButton, PictureBookCloseButtonNrvSelected));

        return true;
    }

    return false;
}

bool PictureBookCloseButton::isSelected() const {
    return isNerve(GET_NERVE(PictureBookCloseButton, PictureBookCloseButtonNrvSelected));
}

void PictureBookCloseButton::control() {
    mPaneCtrl->update();

    if (mPaneCtrl->isPointingTrigger()) {
        MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON");
    }
}

void PictureBookCloseButton::exeSelect() {
}

void PictureBookCloseButton::exeSelected() {
    if (mPaneCtrl->isHidden()) {
        kill();
    }
}

void PictureBookCloseButton::exeNotSelected() {
    if (mPaneCtrl->isHidden()) {
        kill();
    }
}
