#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/GalaxyConfirmLayout.hpp"
#include "Game/Screen/BackButton.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvGalaxyConfirmLayout {
    NEW_NERVE(GalaxyConfirmLayoutNrvSelecting, GalaxyConfirmLayout, Selecting);
    NEW_NERVE(GalaxyConfirmLayoutNrvSelectedBackButton, GalaxyConfirmLayout, SelectedBackButton);
    NEW_NERVE(GalaxyConfirmLayoutNrvSelected, GalaxyConfirmLayout, Selected);
};

GalaxyConfirmLayout::GalaxyConfirmLayout() :
    LayoutActor("ギャラクシー確認レイアウト", true),
    mPaneCtrl(nullptr),
    mBackButton(nullptr)
{
    
}

void GalaxyConfirmLayout::init(const JMapInfoIter& rIter) {
    initLayoutManager("GalaxySelect", 1);
    MR::connectToSceneLayout(this);

    mPaneCtrl = new ButtonPaneController(this, "StartButton", "BoxStartButton", 0, true);
    mBackButton = new BackButton("戻るボタン", true);
    mBackButton->initWithoutIter();

    initNerve(&NrvGalaxyConfirmLayout::GalaxyConfirmLayoutNrvSelecting::sInstance);
}

void GalaxyConfirmLayout::appear() {
    mPaneCtrl->appear();
    mBackButton->appear();
    LayoutActor::appear();
    setNerve(&NrvGalaxyConfirmLayout::GalaxyConfirmLayoutNrvSelecting::sInstance);
}

bool GalaxyConfirmLayout::isSelected() const {
    return isNerve(&NrvGalaxyConfirmLayout::GalaxyConfirmLayoutNrvSelected::sInstance)
        || isNerve(&NrvGalaxyConfirmLayout::GalaxyConfirmLayoutNrvSelectedBackButton::sInstance);
}

bool GalaxyConfirmLayout::isSelectedYes() const {
    return mPaneCtrl->mIsSelected;
}

void GalaxyConfirmLayout::requestMovementOn() {
    MR::requestMovementOn(this);
    MR::requestMovementOn(mBackButton);
}

void GalaxyConfirmLayout::control() {
    mPaneCtrl->update();
}

void GalaxyConfirmLayout::exeSelecting() {
    if (mBackButton->_24) {
        setNerve(&NrvGalaxyConfirmLayout::GalaxyConfirmLayoutNrvSelectedBackButton::sInstance);
    }
    else {
        if (mPaneCtrl->isPointingTrigger()) {
            MR::startSystemSE("SE_SY_BUTTON_CURSOR_ON", -1, -1);
        }

        if (mPaneCtrl->trySelect()) {
            MR::startSystemSE("SE_SY_GALAXY_DECIDE_OK", -1, -1);
            mBackButton->disappear();
            setNerve(&NrvGalaxyConfirmLayout::GalaxyConfirmLayoutNrvSelected::sInstance);
        }
        else if (!mBackButton->isAppearing() && !mPaneCtrl->isAppearing() && MR::testCorePadTriggerB(0)) {
            MR::startSystemSE("SE_SY_GALAXY_DECIDE_CANCEL", -1, -1);
            mPaneCtrl->disappear();
            mBackButton->disappear();
            setNerve(&NrvGalaxyConfirmLayout::GalaxyConfirmLayoutNrvSelected::sInstance);
        }
    }
}

void GalaxyConfirmLayout::exeSelectedBackButton() {
    if (mBackButton->isDisappearing()) {
        mPaneCtrl->disappear();
        setNerve(&NrvGalaxyConfirmLayout::GalaxyConfirmLayoutNrvSelected::sInstance);
    }
}

void GalaxyConfirmLayout::exeSelected() {
    if (mBackButton->isHidden() && mPaneCtrl->isHidden()) {
        kill();
    }
}
