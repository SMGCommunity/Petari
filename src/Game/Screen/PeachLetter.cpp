#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/PeachLetter.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvPeachLetter {
    NEW_NERVE(PeachLetterNrvAppear, PeachLetter, Appear);
    NEW_NERVE(PeachLetterNrvWait, PeachLetter, Wait);
    NEW_NERVE(PeachLetterNrvEnd, PeachLetter, End);
};

PeachLetter::PeachLetter(const char* pName) :
    LayoutActor(pName, true),
    mAButtonIcon(NULL)
{
    
}

void PeachLetter::init(const JMapInfoIter&) {
    initLayoutManager("PeachLetter", 1);

    mAButtonIcon = new IconAButton(false, false);
    mAButtonIcon->initWithoutIter();

    mAButtonIcon->setFollowActorPane(this, "AButtonPosition");
    MR::connectToSceneTalkLayoutNoMovement(this);
    MR::connectToSceneTalkLayoutNoMovement(mAButtonIcon);
    initNerve(&NrvPeachLetter::PeachLetterNrvAppear::sInstance);
    MR::setTextBoxGameMessageRecursive(this, "Body", "PeachLetter_001");
    kill();
}

void PeachLetter::movement() {
    LayoutActor::movement();
    mAButtonIcon->movement();
}

void PeachLetter::appear() {
    LayoutActor::appear();
    setNerve(&NrvPeachLetter::PeachLetterNrvAppear::sInstance);
}

void PeachLetter::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::startSystemSE("SE_SY_LETTER_APPEAR", -1, -1);
    }

    if (MR::isAnimStopped(this, 0)) {
        MR::startSystemSE("SE_SV_PEACH_MAIL", -1, -1);
        setNerve(&NrvPeachLetter::PeachLetterNrvWait::sInstance);
    }
}

void PeachLetter::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
        mAButtonIcon->openWithoutMessage();
    }

    if (mAButtonIcon->isWait() && MR::testCorePadTriggerA(0)) {
        MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
        setNerve(&NrvPeachLetter::PeachLetterNrvEnd::sInstance);
    }
}

void PeachLetter::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
        mAButtonIcon->term();
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

void PeachLetter::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    pCollector->addArchive("PeachLetter");
}
