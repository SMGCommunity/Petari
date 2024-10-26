#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
ButtonPaneController::ButtonPaneController(LayoutActor* pHost, const char* pPaneName, const char* pBoxButtonName, u32 u, bool b) : NerveExecutor(pPaneName) {
    mHost = pHost;
    mPaneName = pPaneName;
    mBoxButtonName = pBoxButtonName;
    _14 = u;
    _18 = b;
    mPointingAnimStartFrame = 0.0f;
    _20 = false;
    _21 = false;
    _22 = true;
    _23 = false;
    mButtonAppearStr = "ButtonAppear";
    mButtonWaitStr = "ButtonWait";
    mButtonDecideStr = "ButtonDecide";
    mButtonSelectInStr = "ButtonSelectIn";
    mButtonSelectOutStr = "ButtonSelectOut";
    mButtonEndStr = "ButtonEnd";

    if (!MR::isExistPaneCtrl(pHost, pPaneName)) {
        MR::createAndAddPaneCtrl(pHost, pPaneName, 1);
    }

    MR::hidePane(mHost, mPaneName);
    initNerve(&NrvButtonPaneController::ButtonPaneControllerNrvHidden::sInstance);
}

void ButtonPaneController::update() {
    if (_24)
        updateNerve();

    _21 = false;
}

void ButtonPaneController::appear() {
    _20 = false;
    mPointingAnimStartFrame = 0.0f;
    _21 = false;
    _24 = true;

    if (mButtonAppearStr) {
        setNerve(&NrvButtonPaneController::ButtonPaneControllerNrvAppear::sInstance);
    }
    else {
        MR::showPane(mHost, mPaneName);
        setNerve(&NrvButtonPaneController::ButtonPaneControllerNrvWait::sInstance);
    }
}

void ButtonPaneController::disappear() {
    _24 = true;
    setNerve(&NrvButtonPaneController::ButtonPaneControllerNrvDisappear::sInstance);
}

bool ButtonPaneController::trySelect() {
    if (!_24)
        return false;
    
    _21 = true;

    if (mButtonDecideStr && isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvPointing::sInstance) && MR::testDPDMenuPadDecideTrigger()) {
        _20 = true;
        setNerve(&NrvButtonPaneController::ButtonPaneControllerNrvDecided::sInstance);
        return true;
    }

    return false;
}

bool ButtonPaneController::isHidden() const {
    return isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvHidden::sInstance);
}

bool ButtonPaneController::isPointing() const {
    return isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvPointing::sInstance);
}

bool ButtonPaneController::isPointingTrigger() const {
    return isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvPointing::sInstance) && MR::isFirstStep(this);
}

bool ButtonPaneController::isTimingForSelectedSe() const {
    return isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvDecided::sInstance) && MR::isStep(this, 1);
}

void ButtonPaneController::invalidateDecide() {
    mButtonDecideStr = nullptr;
}

void ButtonPaneController::invalidateAppearance() {
    mButtonAppearStr = nullptr;
    mButtonEndStr = nullptr;
    _22 = false;
}

bool ButtonPaneController::isAppearing() const {
    return isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvAppear::sInstance);
}

bool ButtonPaneController::isDisappearing() const {
    return isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvDisappear::sInstance);
}

void ButtonPaneController::forceToWait() {
    _20 = false;
    mPointingAnimStartFrame = 0.0f;
    _21 = false;
    setNerve(&NrvButtonPaneController::ButtonPaneControllerNrvWait::sInstance);
}

void ButtonPaneController::forceToHide() {
    setNerve(&NrvButtonPaneController::ButtonPaneControllerNrvHidden::sInstance);
}

bool ButtonPaneController::isFirstStepWait() const {
    return isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvWait::sInstance) && MR::isFirstStep(this);
}

bool ButtonPaneController::isWait() const {
    return isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvWait::sInstance);
}

bool ButtonPaneController::isDecidedWait() const {
    return isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvDecidedWait::sInstance);
}

bool ButtonPaneController::onPointing() {
    _21 = true;
    return tryPointing(true);
}

void ButtonPaneController::decide() {
    _20 = true;
    setNerve(&NrvButtonPaneController::ButtonPaneControllerNrvDecided::sInstance);
}

bool ButtonPaneController::isPointingPaneDirect() const {
    return MR::isStarPointerPointingPane(mHost, mBoxButtonName, 0, 1, "弱");
}

bool ButtonPaneController::startAnimAtFirstStep(const char* pStr) {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(mHost, mPaneName, pStr, _14);
        return true;
    }
    return false;
}

bool ButtonPaneController::startPointingAnimAtFirstStep(const char* pStr) {
    if (startAnimAtFirstStep(pStr)) {
        MR::setPaneAnimFrameAndStop(mHost, mPaneName, mPointingAnimStartFrame, _14);
        return true;
    }
    return false;
}

void ButtonPaneController::setNerveAtAnimStopped(const Nerve* pNerve) {
    if (MR::isPaneAnimStopped(mHost, mPaneName, _14)) {
        setNerve(pNerve);
    }
}

bool ButtonPaneController::isPointingPane() const {
    return _21 && MR::isStarPointerPointingPane(mHost, mBoxButtonName, 0, 1, "弱");
}

bool ButtonPaneController::tryPointing(bool b) {
    if (!b) 
        return false;
    
    if (isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvWait::sInstance) || isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvPointing::sInstance)) {
        if (isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvNotPointing::sInstance))
            mPointingAnimStartFrame = calcPointingAnimStartFrame();
        else
            mPointingAnimStartFrame = 0.0f;

        setNerve(&NrvButtonPaneController::ButtonPaneControllerNrvPointing::sInstance);
        return true;
    }
    
    return false;
}

bool ButtonPaneController::tryNotPointing(bool b) {
    if (b)
        return false;

    if (isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvPointing::sInstance)) {
        if (isNerve(&NrvButtonPaneController::ButtonPaneControllerNrvPointing::sInstance))
            mPointingAnimStartFrame = calcPointingAnimStartFrame();
        else
            mPointingAnimStartFrame = 0.0f;
        
        setNerve(&NrvButtonPaneController::ButtonPaneControllerNrvPointing::sInstance);
        return true;
    }
    return false;
}

f32 ButtonPaneController::calcPointingAnimStartFrame() const {
    if (MR::isPaneAnimStopped(mHost, mPaneName, _14))
        return 0.0f;
        
    f32 animFrame = MR::getPaneAnimFrame(mHost, mPaneName, _14);
    f32 animFrameMax = MR::getAnimFrameMax(mHost, mButtonSelectInStr);
    animFrame = animFrameMax-animFrame;

    if (animFrame < 0.0f)
        return 0.0f;

    if (animFrame > animFrameMax)
        return animFrameMax;
    else
        return animFrame;

    return 0.0f;
}

void ButtonPaneController::exeHidden() {
    if (MR::isFirstStep(this))
        MR::hidePane(mHost, mPaneName);
}

void ButtonPaneController::exeAppear() {
    if (startAnimAtFirstStep(mButtonAppearStr))
        MR::showPane(mHost, mPaneName);

    setNerve(&NrvButtonPaneController::ButtonPaneControllerNrvWait::sInstance);
}

void ButtonPaneController::exeWait() {
    startAnimAtFirstStep(mButtonWaitStr);

    if (!_23 && tryPointing(isPointingPane()))
        return;
}

void ButtonPaneController::exePointing() {
    startPointingAnimAtFirstStep(mButtonSelectInStr);

    if (!_23 ) {
        if (tryNotPointing(isPointingPane()))
            return;
    }
    else {
        if (tryNotPointing(_21))
            return;
    }
}

void ButtonPaneController::exeNotPointing() {
    startPointingAnimAtFirstStep(mButtonSelectOutStr);

    if (!_23) {
        if (tryPointing(isPointingPane()))
            return;
    }

    setNerveAtAnimStopped(&NrvButtonPaneController::ButtonPaneControllerNrvWait::sInstance);
}

void ButtonPaneController::exeDecided() {
    startAnimAtFirstStep(mButtonDecideStr);
    if (_18)
        setNerveAtAnimStopped(&NrvButtonPaneController::ButtonPaneControllerNrvDecidedToDisappear::sInstance);
    else {
        if (_22)
            setNerveAtAnimStopped(&NrvButtonPaneController::ButtonPaneControllerNrvDisappear::sInstance);
        else
            setNerveAtAnimStopped(&NrvButtonPaneController::ButtonPaneControllerNrvDecidedWait::sInstance);
    }
}

void ButtonPaneController::exeDecidedToDisappear() {
    if (startAnimAtFirstStep(mButtonSelectOutStr))
        MR::setPaneAnimRate(mHost, mPaneName, 1.5f, _14);

    if (_22)
        setNerveAtAnimStopped(&NrvButtonPaneController::ButtonPaneControllerNrvDisappear::sInstance);
    else
        setNerveAtAnimStopped(&NrvButtonPaneController::ButtonPaneControllerNrvDecidedWait::sInstance);
}

ButtonPaneController::~ButtonPaneController() {
    
}

namespace NrvButtonPaneController {
    void ButtonPaneControllerNrvDisappear::execute(Spine* pSpine) const {
        ButtonPaneController* pExecutor = (ButtonPaneController*)pSpine->mExecutor;
        pExecutor->startAnimAtFirstStep(pExecutor->mButtonEndStr);
        pExecutor->setNerveAtAnimStopped(&NrvButtonPaneController::ButtonPaneControllerNrvHidden::sInstance);
    }
    
    INIT_NERVE(ButtonPaneControllerNrvDisappear);
    INIT_NERVE(ButtonPaneControllerNrvDecidedToDisappear);
    INIT_NERVE(ButtonPaneControllerNrvDecidedWait);
    INIT_NERVE(ButtonPaneControllerNrvDecided);
    INIT_NERVE(ButtonPaneControllerNrvNotPointing);
    INIT_NERVE(ButtonPaneControllerNrvPointing);
    INIT_NERVE(ButtonPaneControllerNrvWait);
    INIT_NERVE(ButtonPaneControllerNrvAppear);
    INIT_NERVE(ButtonPaneControllerNrvHidden);
};