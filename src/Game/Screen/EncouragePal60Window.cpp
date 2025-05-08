#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/EncouragePal60Window.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    NEW_NERVE(EncouragePal60WindowAppear, EncouragePal60Window, Appear);
    NEW_NERVE(EncouragePal60WindowDisplay, EncouragePal60Window, Display);
    NEW_NERVE(EncouragePal60WindowDisappear, EncouragePal60Window, Disappear);
};

EncouragePal60Window::EncouragePal60Window() :
    LayoutActor("PAL60推奨画面", true)
{}

void EncouragePal60Window::init(const JMapInfoIter& rIter) {
    initLayoutManager("SysPALInfo", 1);
    MR::connectToSceneLayout(this);

    mAButtonIcon = MR::createAndSetupIconAButton(this, true, false);

    initNerve(&EncouragePal60WindowAppear::sInstance);
    kill();
}

void EncouragePal60Window::appear() {
    LayoutActor::appear();
    setNerve(&EncouragePal60WindowAppear::sInstance);
}

void EncouragePal60Window::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&EncouragePal60WindowDisplay::sInstance);
    }
}

void EncouragePal60Window::exeDisplay() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
    }

    if (MR::isStep(this, 120)) {
        mAButtonIcon->openWithoutMessage();
    }

    if (MR::isGreaterStep(this, 120) && MR::testCorePadTriggerAnyWithoutHome(0)) {
        MR::startSystemSE("SE_SY_TALK_FOCUS_ITEM", -1, -1);
        setNerve(&EncouragePal60WindowDisappear::sInstance);
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

EncouragePal60Window::~EncouragePal60Window() {
    
}
