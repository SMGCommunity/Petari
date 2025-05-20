#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/GalaxySelectBackButton.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvGalaxySelectBackButton {
    NEW_NERVE(GalaxySelectBackButtonNrvAppear, GalaxySelectBackButton, Appear);
    NEW_NERVE(GalaxySelectBackButtonNrvDisappear, GalaxySelectBackButton, Disappear);
    NEW_NERVE(GalaxySelectBackButtonNrvDecide, GalaxySelectBackButton, Decide);
};

GalaxySelectBackButton::GalaxySelectBackButton() :
    LayoutActor("ギャラクシー選択戻るボタン", true)
{
    
}

void GalaxySelectBackButton::init(const JMapInfoIter&) {
    initLayoutManager("BackButton", 1);
    MR::connectToSceneLayout(this);

    mPaneCtrl = new ButtonPaneController(this, "Back", "BoxButton", 0, true);
    mPaneCtrl->_23 = true;

    initNerve(&NrvGalaxySelectBackButton::GalaxySelectBackButtonNrvAppear::sInstance);
}

void GalaxySelectBackButton::appear() {
    LayoutActor::appear();
    setNerve(&NrvGalaxySelectBackButton::GalaxySelectBackButtonNrvAppear::sInstance);
}

void GalaxySelectBackButton::disappear() {
    setNerve(&NrvGalaxySelectBackButton::GalaxySelectBackButtonNrvDisappear::sInstance);
}

void GalaxySelectBackButton::decide() {
    setNerve(&NrvGalaxySelectBackButton::GalaxySelectBackButtonNrvDecide::sInstance);
}

bool GalaxySelectBackButton::isPointing() const {
    return mPaneCtrl->isPointingPaneDirect();
}

bool GalaxySelectBackButton::tryPointing() {
    return mPaneCtrl->onPointing();
}

bool GalaxySelectBackButton::isAppearing() const {
    return mPaneCtrl->isAppearing();
}

bool GalaxySelectBackButton::isDisappearing() const {
    return mPaneCtrl->isDisappearing();
}

void GalaxySelectBackButton::control() {
    mPaneCtrl->update();
}

void GalaxySelectBackButton::exeAppear() {
    if (MR::isFirstStep(this)) {
        mPaneCtrl->appear();
    }
}

void GalaxySelectBackButton::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mPaneCtrl->disappear();
    }

    if (mPaneCtrl->isHidden()) {
        kill();
    }
}

void GalaxySelectBackButton::exeDecide() {
    if (MR::isFirstStep(this)) {
        mPaneCtrl->decide();
    }

    if (mPaneCtrl->isHidden()) {
        kill();
    }
}
