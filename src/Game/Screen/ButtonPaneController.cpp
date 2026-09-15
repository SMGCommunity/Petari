#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    const f32 cDecidedToDisappearAnimRate = 1.5f;
};  // namespace

namespace NrvButtonPaneController {
    NEW_NERVE(ButtonPaneControllerNrvHidden, ButtonPaneController, Hidden);
    NEW_NERVE(ButtonPaneControllerNrvAppear, ButtonPaneController, Appear);
    NEW_NERVE(ButtonPaneControllerNrvWait, ButtonPaneController, Wait);
    NEW_NERVE(ButtonPaneControllerNrvPointing, ButtonPaneController, Pointing);
    NEW_NERVE(ButtonPaneControllerNrvNotPointing, ButtonPaneController, NotPointing);
    NEW_NERVE(ButtonPaneControllerNrvDecided, ButtonPaneController, Decided);
    NEW_NERVE(ButtonPaneControllerNrvDecidedWait, ButtonPaneController, DecidedWait);
    NEW_NERVE(ButtonPaneControllerNrvDecidedToDisappear, ButtonPaneController, DecidedToDisappear);
    NEW_NERVE(ButtonPaneControllerNrvDisappear, ButtonPaneController, Disappear);
};  // namespace NrvButtonPaneController

ButtonPaneController::ButtonPaneController(LayoutActor* pHost, const char* pPaneName, const char* pBoundingPaneName, u32 animIndex, bool param5)
    : NerveExecutor(pPaneName), mHost(pHost), mPaneName(pPaneName), mBoundingPaneName(pBoundingPaneName), mAnimIndex(animIndex), _18(param5),
      mPointingAnimStartFrame(0.0f), mIsSelected(false), mIsPointing(false), _22(true), _23(false), mAppearAnimName("ButtonAppear"),
      mWaitAnimName("ButtonWait"), mDecideAnimName("ButtonDecide"), mPointingAnimName("ButtonSelectIn"), mNotPointingAnimName("ButtonSelectOut"),
      mDisappearAnimName("ButtonEnd") {
    if (!MR::isExistPaneCtrl(pHost, pPaneName)) {
        MR::createAndAddPaneCtrl(pHost, pPaneName, 1);
    }

    MR::hidePane(mHost, mPaneName);
    initNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvHidden));
}

void ButtonPaneController::update() {
    if (_24) {
        updateNerve();
    }

    mIsPointing = false;
}

void ButtonPaneController::appear() {
    mIsSelected = false;
    mPointingAnimStartFrame = 0.0f;
    mIsPointing = false;
    _24 = true;

    if (mAppearAnimName != nullptr) {
        setNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvAppear));
    } else {
        MR::showPane(mHost, mPaneName);
        setNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvWait));
    }
}

void ButtonPaneController::disappear() {
    _24 = true;

    setNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDisappear));
}

bool ButtonPaneController::trySelect() {
    if (!_24) {
        return false;
    }

    mIsPointing = true;

    if (mDecideAnimName != nullptr && isPointing() && MR::testDPDMenuPadDecideTrigger()) {
        mIsSelected = true;

        setNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDecided));

        return true;
    }

    return false;
}

bool ButtonPaneController::isHidden() const {
    return isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvHidden));
}

bool ButtonPaneController::isPointing() const {
    return isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvPointing));
}

bool ButtonPaneController::isPointingTrigger() const {
    return isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvPointing)) && MR::isFirstStep(this);
}

bool ButtonPaneController::isTimingForSelectedSe() const {
    return isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDecided)) && MR::isStep(this, 1);
}

void ButtonPaneController::invalidateDecide() {
    mDecideAnimName = nullptr;
}

void ButtonPaneController::invalidateAppearance() {
    mAppearAnimName = nullptr;
    mDisappearAnimName = nullptr;
    _22 = false;
}

bool ButtonPaneController::isAppearing() const {
    return isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvAppear));
}

bool ButtonPaneController::isDisappearing() const {
    return isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDisappear));
}

void ButtonPaneController::forceToWait() {
    mIsSelected = false;
    mPointingAnimStartFrame = 0.0f;
    mIsPointing = false;

    setNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvWait));
}

void ButtonPaneController::forceToHide() {
    setNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvHidden));
}

bool ButtonPaneController::isFirstStepWait() const {
    return isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvWait)) && MR::isFirstStep(this);
}

bool ButtonPaneController::isWait() const {
    return isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvWait));
}

bool ButtonPaneController::isDecidedWait() const {
    return isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDecidedWait));
}

bool ButtonPaneController::onPointing() {
    mIsPointing = true;

    return tryPointing(true);
}

void ButtonPaneController::decide() {
    mIsSelected = true;

    setNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDecided));
}

bool ButtonPaneController::isPointingPaneDirect() const {
    return MR::isStarPointerPointingPane(mHost, mBoundingPaneName, 0, true, "弱");
}

bool ButtonPaneController::startAnimAtFirstStep(const char* pAnimName) {
    if (MR::isFirstStep(this)) {
        MR::startPaneAnim(mHost, mPaneName, pAnimName, mAnimIndex);

        return true;
    }

    return false;
}

bool ButtonPaneController::startPointingAnimAtFirstStep(const char* pAnimName) {
    if (startAnimAtFirstStep(pAnimName)) {
        MR::setPaneAnimFrame(mHost, mPaneName, mPointingAnimStartFrame, mAnimIndex);

        return true;
    }

    return false;
}

void ButtonPaneController::setNerveAtAnimStopped(const Nerve* pNerve) {
    if (MR::isPaneAnimStopped(mHost, mPaneName, mAnimIndex)) {
        setNerve(pNerve);
    }
}

bool ButtonPaneController::isPointingPane() const {
    return mIsPointing && MR::isStarPointerPointingPane(mHost, mBoundingPaneName, 0, true, "弱");
}

bool ButtonPaneController::tryPointing(bool param1) {
    if (!param1) {
        return false;
    }

    if (isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvWait)) ||
        isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvNotPointing))) {
        if (isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvNotPointing))) {
            mPointingAnimStartFrame = calcPointingAnimStartFrame();
        } else {
            mPointingAnimStartFrame = 0.0f;
        }

        setNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvPointing));

        return true;
    }

    return false;
}

bool ButtonPaneController::tryNotPointing(bool param1) {
    if (param1) {
        return false;
    }

    if (isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvPointing))) {
        if (isNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvPointing))) {
            mPointingAnimStartFrame = calcPointingAnimStartFrame();
        } else {
            mPointingAnimStartFrame = 0.0f;
        }

        setNerve(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvNotPointing));

        return true;
    }

    return false;
}

f32 ButtonPaneController::calcPointingAnimStartFrame() const {
    if (MR::isPaneAnimStopped(mHost, mPaneName, mAnimIndex)) {
        return 0.0f;
    }

    f32 animFrame = MR::getPaneAnimFrame(mHost, mPaneName, mAnimIndex);
    f32 animFrameMax = MR::getAnimFrameMax(mHost, mPointingAnimName);

    animFrame = animFrameMax - animFrame;

    if (animFrame < 0.0f) {
        return 0.0f;
    }

    if (animFrame > animFrameMax) {
        return animFrameMax;
    }

    return animFrame;
}

void ButtonPaneController::exeHidden() {
    if (MR::isFirstStep(this)) {
        MR::hidePane(mHost, mPaneName);
    }
}

void ButtonPaneController::exeAppear() {
    if (startAnimAtFirstStep(mAppearAnimName)) {
        MR::showPane(mHost, mPaneName);
    }

    setNerveAtAnimStopped(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvWait));
}

void ButtonPaneController::exeWait() {
    startAnimAtFirstStep(mWaitAnimName);

    if (!_23) {
        if (tryPointing(isPointingPane())) {
            return;
        }
    }
}

void ButtonPaneController::exePointing() {
    startPointingAnimAtFirstStep(mPointingAnimName);

    if (!_23) {
        if (tryNotPointing(isPointingPane())) {
            return;
        }
    } else if (tryNotPointing(mIsPointing)) {
        return;
    }
}

void ButtonPaneController::exeNotPointing() {
    startPointingAnimAtFirstStep(mNotPointingAnimName);

    if (!_23) {
        if (tryPointing(isPointingPane())) {
            return;
        }
    }

    setNerveAtAnimStopped(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvWait));
}

void ButtonPaneController::exeDecided() {
    startAnimAtFirstStep(mDecideAnimName);

    if (_18) {
        setNerveAtAnimStopped(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDecidedToDisappear));
    } else if (_22) {
        setNerveAtAnimStopped(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDisappear));
    } else {
        setNerveAtAnimStopped(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDecidedWait));
    }
}

void ButtonPaneController::exeDecidedWait() {
}

void ButtonPaneController::exeDecidedToDisappear() {
    if (startAnimAtFirstStep(mNotPointingAnimName)) {
        MR::setPaneAnimRate(mHost, mPaneName, ::cDecidedToDisappearAnimRate, mAnimIndex);
    }

    if (_22) {
        setNerveAtAnimStopped(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDisappear));
    } else {
        setNerveAtAnimStopped(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvDecidedWait));
    }
}

void ButtonPaneController::exeDisappear() {
    startAnimAtFirstStep(mDisappearAnimName);
    setNerveAtAnimStopped(GET_NERVE(ButtonPaneController, ButtonPaneControllerNrvHidden));
}
