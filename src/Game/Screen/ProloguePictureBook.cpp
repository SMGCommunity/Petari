#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/ProloguePictureBook.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DAnimation.hpp"

namespace {
    static const s32 sBookPageInfo[] = {0, 350, 700, 1050, 1400, 1748, -1};
};

namespace NrvProloguePictureBook {
    NEW_NERVE(ProloguePictureBookActive, ProloguePictureBook, Active);
    NEW_NERVE(ProloguePictureBookPlaying, ProloguePictureBook, Playing);
    NEW_NERVE(ProloguePictureBookKeyWait, ProloguePictureBook, KeyWait);
    NEW_NERVE(ProloguePictureBookEnd, ProloguePictureBook, End);
};

ProloguePictureBook::ProloguePictureBook() :
    LayoutActor("プロローグの絵本", true),
    mAButtonIcon(NULL),
    mPage(0)
{}

void ProloguePictureBook::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("PrologueDemo", 1);
    initNerve(&NrvProloguePictureBook::ProloguePictureBookPlaying::sInstance);

    mAButtonIcon = MR::createAndSetupIconAButton(this, true, false);

    kill();
}

void ProloguePictureBook::appear() {
    LayoutActor::appear();
    MR::requestMovementOn(mAButtonIcon);
    MR::startAnim(this, "Prologue", 0);
    setNerve(&NrvProloguePictureBook::ProloguePictureBookPlaying::sInstance);
}

void ProloguePictureBook::kill() {
    LayoutActor::kill();
    mAButtonIcon->kill();
}

void ProloguePictureBook::exeActive() {
    J3DFrameCtrl* pAnimCtrl = MR::getAnimCtrl(this, 0);

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvProloguePictureBook::ProloguePictureBookEnd::sInstance);
    }
}

void ProloguePictureBook::exePlaying() {
    J3DFrameCtrl* pAnimCtrl = MR::getAnimCtrl(this, 0);

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvProloguePictureBook::ProloguePictureBookEnd::sInstance);
    }
    else {
        int index = mPage + 1;

        if (sBookPageInfo[index] < 0) {
            return;
        }

        MR::testSystemPadTriggerDecide();

        if (sBookPageInfo[index] > static_cast<s32>(pAnimCtrl->mCurrentFrame)) {
            return;
        }

        pAnimCtrl->mCurrentFrame = sBookPageInfo[index] - 1.0f;

        setNerve(&NrvProloguePictureBook::ProloguePictureBookKeyWait::sInstance);
    }
}

void ProloguePictureBook::exeKeyWait() {
    J3DFrameCtrl* pAnimCtrl = MR::getAnimCtrl(this, 0);

    if (MR::isFirstStep(this)) {
        mAButtonIcon->openWithoutMessage();
        pAnimCtrl->mSpeed = 0.0f;
    }

    if (MR::testCorePadTriggerA(0)) {
        MR::startSystemSE("SE_SY_TALK_FOCUS_ITEM", -1, -1);
        mAButtonIcon->term();

        mPage++;
        pAnimCtrl->mSpeed = 1.0f;

        setNerve(&NrvProloguePictureBook::ProloguePictureBookPlaying::sInstance);
    }
}

void ProloguePictureBook::exeEnd() {
    
}

bool ProloguePictureBook::isEnd() const {
    return isNerve(&NrvProloguePictureBook::ProloguePictureBookEnd::sInstance);
}

void ProloguePictureBook::control() {
    MR::setLayoutScalePosAtPaneScaleTrans(mAButtonIcon, this, "AButtonPosition");
}

ProloguePictureBook::~ProloguePictureBook() {
    
}
