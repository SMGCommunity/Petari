#include "Game/Screen/EncouragePal60Window.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 DISPLAY_MIN_SECOND = 2;
};  // namespace

namespace {
    NEW_NERVE(EncouragePal60WindowAppear, EncouragePal60Window, Appear);
    NEW_NERVE(EncouragePal60WindowDisplay, EncouragePal60Window, Display);
    NEW_NERVE(EncouragePal60WindowDisappear, EncouragePal60Window, Disappear);
};  // namespace

EncouragePal60Window::EncouragePal60Window() : LayoutActor("PAL60推奨画面", true) {
}

void EncouragePal60Window::init(const JMapInfoIter& rIter) {
    initLayoutManager("SysPALInfo", 1);
    MR::connectToSceneLayout(this);

    mAButtonIcon = MR::createAndSetupIconAButton(this, true, false);

    initNerve(GET_NERVE_ANON(EncouragePal60WindowAppear));
    kill();
}

void EncouragePal60Window::appear() {
    LayoutActor::appear();
    setNerve(GET_NERVE_ANON(EncouragePal60WindowAppear));
}

void EncouragePal60Window::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(GET_NERVE_ANON(EncouragePal60WindowDisplay));
    }
}

void EncouragePal60Window::exeDisplay() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
    }

    if (MR::isStep(this, ::DISPLAY_MIN_SECOND * 60)) {
        mAButtonIcon->openWithoutMessage();
    }

    if (MR::isGreaterStep(this, ::DISPLAY_MIN_SECOND * 60) && MR::testCorePadTriggerAnyWithoutHome(WPAD_CHAN0)) {
        MR::startSystemSE("SE_SY_TALK_FOCUS_ITEM");
        setNerve(GET_NERVE_ANON(EncouragePal60WindowDisappear));
    }
}

void EncouragePal60Window::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}
