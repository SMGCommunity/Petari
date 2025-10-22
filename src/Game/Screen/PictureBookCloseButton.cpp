#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/PictureBookCloseButton.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvPictureBookCloseButton {
    NEW_NERVE(PictureBookCloseButtonNrvSelect, PictureBookCloseButton, Select);
    NEW_NERVE(PictureBookCloseButtonNrvSelected, PictureBookCloseButton, Selected);
    NEW_NERVE(PictureBookCloseButtonNrvNotSelected, PictureBookCloseButton, NotSelected);
};

PictureBookCloseButton::PictureBookCloseButton(bool canCloseWithPad) :
    LayoutActor("絵本閉じるボタン", true),
    mPaneCtrl(nullptr),
    mCanCloseWithPad(canCloseWithPad)
{}

void PictureBookCloseButton::init(const JMapInfoIter& rIter) {
    initLayoutManager("BackButton", 1);
    MR::connectToSceneLayoutDecoration(this);

    mPaneCtrl = new ButtonPaneController(this, "Back", "BoxButton", 0, true);

    initNerve(&NrvPictureBookCloseButton::PictureBookCloseButtonNrvSelect::sInstance);
}

void PictureBookCloseButton::appear() {
    LayoutActor::appear();
    mPaneCtrl->appear();
    MR::requestMovementOn(this);
    setNerve(&NrvPictureBookCloseButton::PictureBookCloseButtonNrvSelect::sInstance);
}

void PictureBookCloseButton::disappear() {
    mPaneCtrl->disappear();
    setNerve(&NrvPictureBookCloseButton::PictureBookCloseButtonNrvNotSelected::sInstance);
}

bool PictureBookCloseButton::trySelect() {
    if (mPaneCtrl->trySelect()) {
        MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL", -1, -1);
        MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
        setNerve(&NrvPictureBookCloseButton::PictureBookCloseButtonNrvSelected::sInstance);

        return true;
    }

    if (!mPaneCtrl->isAppearing() && MR::testCorePadTriggerB(0) && mCanCloseWithPad) {
        mPaneCtrl->disappear();
        MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL", -1, -1);
        MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
        setNerve(&NrvPictureBookCloseButton::PictureBookCloseButtonNrvSelected::sInstance);

        return true;
    }

    return false;
}

bool PictureBookCloseButton::isSelected() const {
    return isNerve(&NrvPictureBookCloseButton::PictureBookCloseButtonNrvSelected::sInstance);
}

void PictureBookCloseButton::control() {
    mPaneCtrl->update();

    if (mPaneCtrl->isPointingTrigger()) {
        MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON", -1, -1);
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
