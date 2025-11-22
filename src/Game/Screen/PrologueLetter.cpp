#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/PrologueLetter.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    NEW_NERVE(PrologueLetterNrvAppear, PrologueLetter, Appear);
    NEW_NERVE(PrologueLetterNrvWait, PrologueLetter, Wait);
    NEW_NERVE(PrologueLetterNrvDisappear, PrologueLetter, Disappear);
}; // namespace

PrologueLetter::PrologueLetter(const char* pName)
    : LayoutActor(pName, true),
      mAButtonIcon(nullptr) {}

void PrologueLetter::init(const JMapInfoIter& rIter) {
    initLayoutManager("PeachLetterMini", 1);
    MR::createAndAddPaneCtrl(this, "AButtonPosition", 1);

    mAButtonIcon = MR::createAndSetupIconAButton(this, false, false);

    MR::connectToSceneLayoutDecoration(mAButtonIcon);
    initNerve(&PrologueLetterNrvAppear::sInstance);
    MR::connectToSceneLayout(this);
}

void PrologueLetter::appear() {
    LayoutActor::appear();
    setNerve(&PrologueLetterNrvAppear::sInstance);
}

void PrologueLetter::kill() {
    LayoutActor::kill();

    if (MR::isDead(mAButtonIcon)) {
        return;
    }

    mAButtonIcon->kill();
}

void PrologueLetter::pauseOff() {
    MR::requestMovementOn(this);
    MR::requestMovementOn(mAButtonIcon);
}

void PrologueLetter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&PrologueLetterNrvWait::sInstance);
    }
}

void PrologueLetter::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
        mAButtonIcon->openWithoutMessage();
    }

    if (MR::testSystemPadTriggerDecide()) {
        MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
        setNerve(&PrologueLetterNrvDisappear::sInstance);
    }
}

void PrologueLetter::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
        mAButtonIcon->term();
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}
