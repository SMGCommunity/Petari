#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/GalaxyMapSelectButton.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    NEW_NERVE(GalaxyMapSelectButtonHide, GalaxyMapSelectButton, Hide);
    NEW_NERVE(GalaxyMapSelectButtonAstroMapWait, GalaxyMapSelectButton, AstroMapWait);
    NEW_NERVE(GalaxyMapSelectButtonStarListWait, GalaxyMapSelectButton, StarListWait);
    NEW_NERVE(GalaxyMapSelectButtonBetweenAstroMapAndGalaxyMapFadein, GalaxyMapSelectButton, BetweenAstroMapAndGalaxyMapFadein);
    NEW_NERVE(GalaxyMapSelectButtonBetweenListToMapFadeout, GalaxyMapSelectButton, BetweenListToMapFadeout);
    NEW_NERVE(GalaxyMapSelectButtonBetweenListToMapFadein, GalaxyMapSelectButton, BetweenListToMapFadein);
    NEW_NERVE(GalaxyMapSelectButtonBetweenMapToListFadeout, GalaxyMapSelectButton, BetweenMapToListFadeout);
    NEW_NERVE(GalaxyMapSelectButtonBetweenMapToListFadein, GalaxyMapSelectButton, BetweenMapToListFadein);
};

GalaxyMapSelectButton::GalaxyMapSelectButton() :
    LayoutActor("選択用ボタン", true),
    _20(NULL),
    mPaneCtrl(NULL)
{
    
}

void GalaxyMapSelectButton::init(const JMapInfoIter& rIter) {
    initLayoutManager("MapButton", 1);
    MR::createAndAddPaneCtrl(this, "MapList", 1);

    mPaneCtrl = new ButtonPaneController(this, "MapList", "BoxLButton", 0, true);
    mPaneCtrl->_22 = false;

    initNerve(&GalaxyMapSelectButtonHide::sInstance);
    kill();
}

void GalaxyMapSelectButton::appear() {
    LayoutActor::appear();
    mPaneCtrl->forceToHide();
}

void GalaxyMapSelectButton::startAstroMap() {
    appear();

    _20 = &GalaxyMapSelectButtonAstroMapWait::sInstance;

    setNerve(&GalaxyMapSelectButtonBetweenAstroMapAndGalaxyMapFadein::sInstance);
}

bool GalaxyMapSelectButton::isPointingAnything() const {
    return mPaneCtrl->isPointing();
}

bool GalaxyMapSelectButton::isDecidedList() const {
    return mPaneCtrl->mIsSelected != NULL && mPaneCtrl->isDecidedWait();
}

void GalaxyMapSelectButton::changeToStarList() {
    _20 = &GalaxyMapSelectButtonStarListWait::sInstance;
}

void GalaxyMapSelectButton::exeHide() {
    
}

void GalaxyMapSelectButton::exeAstroMapWait() {
    if (_20 != NULL && _20 == &GalaxyMapSelectButtonStarListWait::sInstance) {
        setNerve(&GalaxyMapSelectButtonBetweenMapToListFadeout::sInstance);
    }
    else if (mPaneCtrl->trySelect()) {
        MR::startSystemSE("SE_SY_GALAMAP_DECIDE", -1, -1);
    }
}

void GalaxyMapSelectButton::exeStarListWait() {
    if (_20 != NULL && _20 == &GalaxyMapSelectButtonAstroMapWait::sInstance) {
        setNerve(&GalaxyMapSelectButtonBetweenListToMapFadeout::sInstance);
    }
}

void GalaxyMapSelectButton::exeBetweenAstroMapAndGalaxyMapFadein() {
    if (MR::isFirstStep(this)) {
        mPaneCtrl->appear();
    }

    if (mPaneCtrl->isAppearing()) {
        return;
    }

    setNerve(_20);

    _20 = NULL;
}

void GalaxyMapSelectButton::exeBetweenListToMapFadeout() {
    setNerve(&GalaxyMapSelectButtonBetweenListToMapFadein::sInstance);
}

void GalaxyMapSelectButton::exeBetweenListToMapFadein() {
    if (MR::isFirstStep(this)) {
        mPaneCtrl->appear();
    }

    if (mPaneCtrl->isAppearing()) {
        return;
    }

    setNerve(_20);

    _20 = NULL;
}

void GalaxyMapSelectButton::exeBetweenMapToListFadeout() {
    if (MR::isFirstStep(this)) {
        mPaneCtrl->disappear();
    }

    if (mPaneCtrl->isDisappearing()) {
        return;
    }

    mPaneCtrl->forceToHide();
    setNerve(&GalaxyMapSelectButtonBetweenMapToListFadein::sInstance);
}

void GalaxyMapSelectButton::exeBetweenMapToListFadein() {
    setNerve(_20);

    _20 = NULL;
}

void GalaxyMapSelectButton::control() {
    mPaneCtrl->update();

    if (mPaneCtrl->isPointingTrigger()) {
        MR::startSystemSE("SE_SY_GALAMAP_CURSOR_ON", -1, -1);
    }
}
