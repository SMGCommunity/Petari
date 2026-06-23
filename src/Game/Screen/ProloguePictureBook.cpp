#include "Game/Screen/ProloguePictureBook.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>

namespace {
    static const s32 sBookPageInfo[] = {0, 350, 700, 1050, 1400, 1748, -1};
};  // namespace

namespace NrvProloguePictureBook {
    NEW_NERVE(ProloguePictureBookActive, ProloguePictureBook, Active);
    NEW_NERVE(ProloguePictureBookPlaying, ProloguePictureBook, Playing);
    NEW_NERVE(ProloguePictureBookKeyWait, ProloguePictureBook, KeyWait);
    NEW_NERVE(ProloguePictureBookEnd, ProloguePictureBook, End);
};  // namespace NrvProloguePictureBook

ProloguePictureBook::ProloguePictureBook() : LayoutActor("プロローグの絵本", true), mIconAButton(), mPage() {
}

void ProloguePictureBook::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("PrologueDemo", 1);
    initNerve(&NrvProloguePictureBook::ProloguePictureBookPlaying::sInstance);

    mIconAButton = MR::createAndSetupIconAButton(this, true, false);

    kill();
}

void ProloguePictureBook::appear() {
    LayoutActor::appear();
    MR::requestMovementOn(mIconAButton);
    MR::startAnim(this, "Prologue", 0);
    setNerve(&NrvProloguePictureBook::ProloguePictureBookPlaying::sInstance);
}

void ProloguePictureBook::kill() {
    LayoutActor::kill();
    mIconAButton->kill();
}

void ProloguePictureBook::exeActive() {
    J3DFrameCtrl* animCtrl = MR::getAnimCtrl(this, 0);

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvProloguePictureBook::ProloguePictureBookEnd::sInstance);
    }
}

void ProloguePictureBook::exePlaying() {
    J3DFrameCtrl* animCtrl = MR::getAnimCtrl(this, 0);

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvProloguePictureBook::ProloguePictureBookEnd::sInstance);
    } else {
        int index = mPage + 1;

        if (::sBookPageInfo[index] < 0) {
            return;
        }

        MR::testSystemPadTriggerDecide();

        if (::sBookPageInfo[index] > static_cast< s32 >(animCtrl->getFrame())) {
            return;
        }

        animCtrl->setFrame(::sBookPageInfo[index] - 1.0f);

        setNerve(&NrvProloguePictureBook::ProloguePictureBookKeyWait::sInstance);
    }
}

void ProloguePictureBook::exeKeyWait() {
    J3DFrameCtrl* animCtrl = MR::getAnimCtrl(this, 0);

    if (MR::isFirstStep(this)) {
        mIconAButton->openWithoutMessage();
        animCtrl->setRate(0.0f);
    }

    if (MR::testCorePadTriggerA(WPAD_CHAN0)) {
        MR::startSystemSE("SE_SY_TALK_FOCUS_ITEM");
        mIconAButton->term();

        mPage++;
        animCtrl->setRate(1.0f);

        setNerve(&NrvProloguePictureBook::ProloguePictureBookPlaying::sInstance);
    }
}

void ProloguePictureBook::exeEnd() {
}

bool ProloguePictureBook::isEnd() const {
    return isNerve(&NrvProloguePictureBook::ProloguePictureBookEnd::sInstance);
}

void ProloguePictureBook::control() {
    MR::setLayoutScalePosAtPaneScaleTrans(mIconAButton, this, "AButtonPosition");
}
