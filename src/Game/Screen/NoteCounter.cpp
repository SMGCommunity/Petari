#include "Game/Screen/NoteCounter.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void NoteCounter_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)10.0f;
    (void)-100.0f;
}

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
    : LayoutActor(pName, true), mNoteNum(), mNoteAddNum(), mNoteMissNum(), mMelodyNo(-1), mRailLength(), mHost() {
}

void NoteCounter::init(const JMapInfoIter& rIter) {
    initNerve(GET_NERVE(NoteCounter, NoteCounterNrvHide));
    appear();
}

void NoteCounter::declareNoteNumMaxAndMelody(LiveActor* pHost, s32 noteNum, s32 melodyNo, f32 railLength) {
    mHost = pHost;
    mNoteNum = noteNum;
    mNoteAddNum = 0;
    mNoteMissNum = 0;
    mMelodyNo = melodyNo;
    mRailLength = railLength;

    if (isNerve(GET_NERVE(NoteCounter, NoteCounterNrvHide)) || isNerve(GET_NERVE(NoteCounter, NoteCounterNrvShowToHide))) {
        setNerve(GET_NERVE(NoteCounter, NoteCounterNrvHideToShow));
    } else if (isNerve(GET_NERVE(NoteCounter, NoteCounterNrvShowWait))) {
        setNerve(GET_NERVE(NoteCounter, NoteCounterNrvShow));
    }
}

void NoteCounter::add() {
    if (mMelodyNo == -2) {
        s32 note = mNoteAddNum;
        while (note >= 96) {
            note -= 4;
        }

        s32 pitchGroup = note / 4;
        MR::startSystemSE("SE_SY_FLOWER_GET_COMBO", pitchGroup + (note - pitchGroup * 4));
    } else if (mMelodyNo == -1) {
        s32 note = mNoteAddNum + 5;
        while (note > 36) {
            note -= 12;
        }

        MR::startSystemSE("SE_SY_FLOWER_GET_COMBO", note);
    } else if (mMelodyNo >= 0) {
        MR::startRemixSound(mMelodyNo, mNoteAddNum, mRailLength);
    }

    mNoteAddNum++;
    if (mNoteAddNum >= mNoteNum) {
        mHost->receiveMessage(0x66, MR::getMessageSensor(), MR::getMessageSensor());
    }

    tryEndDisp();
}

void NoteCounter::exeShow() {
}

void NoteCounter::exeShowWait() {
    if (MR::isStep(this, ::sStepShowWait)) {
        setNerve(GET_NERVE(NoteCounter, NoteCounterNrvShowToHide));
    }
}

void NoteCounter::exeShowToHide() {
    f32 y = MR::getEaseOutValue(getNerveStep() / 10.0f, ::sTransMaxY, ::sTransMinY, 1.0f);
    setTrans(TVec2f(getTrans().x, y));

    if (MR::isStep(this, ::sStepMove)) {
        setNerve(GET_NERVE(NoteCounter, NoteCounterNrvHide));
    }
}

void NoteCounter::exeHideToShow() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
    }

    f32 y = MR::getEaseOutValue(getNerveStep() / 10.0f, ::sTransMinY, ::sTransMaxY, 1.0f);
    setTrans(TVec2f(getTrans().x, y));

    if (MR::isStep(this, ::sStepMove)) {
        setNerve(GET_NERVE(NoteCounter, NoteCounterNrvShow));
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

    setNerve(GET_NERVE(NoteCounter, NoteCounterNrvShowWait));
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
