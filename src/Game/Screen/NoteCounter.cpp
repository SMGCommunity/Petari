#include "Game/Screen/NoteCounter.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"

namespace {
    static const s32 sStepShowWait = 180;
    static const s32 sStepMove = 10;
    static const f32 sTransMinY = -100.0f;
    static const f32 sTransMaxY = 0.0f;
};  // namespace

namespace NrvNoteCounter {
    NEW_NERVE(NoteCounterNrvShow, NoteCounter, Show);
    NEW_NERVE(NoteCounterNrvShowWait, NoteCounter, ShowWait);
    NEW_NERVE(NoteCounterNrvShowToHide, NoteCounter, ShowToHide);
    NEW_NERVE(NoteCounterNrvHideToShow, NoteCounter, HideToShow);
    NEW_NERVE(NoteCounterNrvHide, NoteCounter, Hide);
};  // namespace NrvNoteCounter

NoteCounter::NoteCounter(const char* pName)
    : LayoutActor(pName, true), mNoteNum(0), mNoteAddNum(0), mNoteMissNum(0), mMelodyNo(-1), mRailLength(0.0f), mHost(nullptr) {}

void NoteCounter::init(const JMapInfoIter& rIter) {
    initNerve(&NrvNoteCounter::NoteCounterNrvHide::sInstance);
    appear();
}

void NoteCounter::declareNoteNumMaxAndMelody(LiveActor* pHost, s32 noteNum, s32 melodyNo, f32 railLength) {
    mHost = pHost;
    mNoteNum = noteNum;
    mNoteAddNum = 0;
    mNoteMissNum = 0;
    mMelodyNo = melodyNo;
    mRailLength = railLength;

    if (isNerve(&NrvNoteCounter::NoteCounterNrvHide::sInstance) || isNerve(&NrvNoteCounter::NoteCounterNrvShowToHide::sInstance)) {
        setNerve(&NrvNoteCounter::NoteCounterNrvHideToShow::sInstance);
    } else if (isNerve(&NrvNoteCounter::NoteCounterNrvShowWait::sInstance)) {
        setNerve(&NrvNoteCounter::NoteCounterNrvShow::sInstance);
    }
}

// NoteCounter::add

void NoteCounter::exeShow() {}

void NoteCounter::exeShowWait() {
    if (MR::isStep(this, sStepShowWait)) {
        setNerve(&NrvNoteCounter::NoteCounterNrvShowToHide::sInstance);
    }
}

void NoteCounter::exeShowToHide() {
    // FIXME: Construction of `trans` is not correct.
    TVec2f trans(getTrans().x, MR::getEaseOutValue(getNerveStep() / 10.0f, sTransMaxY, sTransMinY, 1.0f));

    setTrans(trans);

    if (MR::isStep(this, sStepMove)) {
        setNerve(&NrvNoteCounter::NoteCounterNrvHide::sInstance);
    }
}

void NoteCounter::exeHideToShow() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
    }

    // FIXME: Construction of `trans` is not correct.
    TVec2f trans(getTrans().x, MR::getEaseOutValue(getNerveStep() / 10.0f, sTransMinY, sTransMaxY, 1.0f));

    setTrans(trans);

    if (MR::isStep(this, sStepMove)) {
        setNerve(&NrvNoteCounter::NoteCounterNrvShow::sInstance);
    }
}

void NoteCounter::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::hideLayout(this);
    }
}

void NoteCounter::tryEndDisp() {
    if (mNoteAddNum + mNoteMissNum < mNoteNum) {
        return;
    }

    setNerve(&NrvNoteCounter::NoteCounterNrvShowWait::sInstance);
}

namespace MR {
    void declareNoteNumMaxAndMelodyToCounter(LiveActor* pHost, s32 noteNum, s32 melodyNo, f32 railLength) {
        NoteCounter* pNoteCounter = MR::getGameSceneLayoutHolder()->mNoteCounter;

        pNoteCounter->declareNoteNumMaxAndMelody(pHost, noteNum, melodyNo, railLength);
    }

    void addNoteToCounter() {
        NoteCounter* pNoteCounter = MR::getGameSceneLayoutHolder()->mNoteCounter;

        pNoteCounter->add();
    }

    void failureNoteCollection() {
        NoteCounter* pNoteCounter = MR::getGameSceneLayoutHolder()->mNoteCounter;

        pNoteCounter->mNoteMissNum++;
        pNoteCounter->tryEndDisp();
    }
};  // namespace MR
