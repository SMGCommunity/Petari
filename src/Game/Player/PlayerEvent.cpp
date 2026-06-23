#include "Game/Player/PlayerEvent.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Player/PlayerEventAbyss.hpp"
#include "Game/Player/PlayerEventDown.hpp"
#include "Game/Player/PlayerEventFireDown.hpp"
#include "Game/Player/PlayerEventGameOver.hpp"
#include "Game/Player/PlayerEventGhostRaceDown.hpp"
#include "Game/Player/PlayerEventRaceDown.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"

EventSequencer::EventSequencer() : NameObj("イベントシーケンサー"), mSequence(nullptr) {
    MR::connectToSceneMapObjMovement(this);
    mHashTable = new HashSortTable(16);
}

void EventSequencer::init(const JMapInfoIter& rIter) {
    addEventSequence< EventDown >("マリオダウン");
    addEventSequence< EventAbyss >("マリオ奈落");
    addEventSequence< EventFireDown >("マリオ炎ダウン");
    addEventSequence< EventRaceDown >("レース負け");
    addEventSequence< EventGhostRaceDown >("ゴーストレース負け");
    addEventSequence< EventGameOver >("ゲームオーバー");

    mHashTable->sort();
}

void EventSequencer::movement() {
    if (mSequence != nullptr) {
        if (mSequence->checkAndRun(mSequenceFrame)) {
            mSequence = nullptr;
        }

        mSequenceFrame++;
    }
}

void EventSequencer::startEvent(const char* pName) {
    EventSequence* sequence;
    if (mHashTable->search(pName, reinterpret_cast< u32* >(&sequence))) {
        mSequence = sequence;
        mSequenceFrame = 0;
        sequence->clearFlag();
    }
}

EventSequence::EventSequence(u32 eventNumMax) : mEventNumMax(eventNumMax) {
    mEventNum = 0;

    mEventFuncs = new EventFunc1[eventNumMax];
    mEventCheckFuncs = new EventFunc2[eventNumMax];
    mStartFrameOrModes = new u16[eventNumMax];
    mStopFrameOrPhases = new u16[eventNumMax];
    mEventFrames = new u16[eventNumMax];

    clearFlag();
}

void EventSequence::clearFlag() {
    for (u32 i = 0; i < mEventNumMax; i++) {
        mEventFrames[i] = 0;
    }

    mPhase = 0;
    mFrameInPhase = 0;
    mNextPhase = false;
}

void EventSequence::addEventOnTime(const char* pName, EventFunc1 eventFn, u16 frame) {
    mEventFuncs[mEventNum] = eventFn;
    mStartFrameOrModes[mEventNum] = frame;
    mStopFrameOrPhases[mEventNum] = frame;
    mEventNum++;
}

void EventSequence::addEventInTime(const char* pName, EventFunc1 eventFn, u16 startFrame, u16 stopFrame) {
    mEventFuncs[mEventNum] = eventFn;
    mStartFrameOrModes[mEventNum] = startFrame;
    mStopFrameOrPhases[mEventNum] = stopFrame;
    mEventNum++;
}

void EventSequence::addEventInStatus(const char* pName, EventFunc1 eventFn, EventFunc2 eventCheckFn) {
    mEventFuncs[mEventNum] = eventFn;
    mStartFrameOrModes[mEventNum] = 0xFFFF;
    mStopFrameOrPhases[mEventNum] = 0xFFFF;
    mEventCheckFuncs[mEventNum] = eventCheckFn;
    mEventNum++;
}

void EventSequence::addEventInPhase(const char* pName, EventFunc1 eventFn, u16 phase) {
    mEventFuncs[mEventNum] = eventFn;
    mStartFrameOrModes[mEventNum] = 0xFFFE;
    mStopFrameOrPhases[mEventNum] = phase;
    mEventCheckFuncs[mEventNum] = nullptr;
    mEventNum++;
}

bool EventSequence::checkAndRun(u32 sequenceFrame) {
    mStopSequence = false;

    updateBefore();

    for (u32 i = 0; i < mEventNum; i++) {
        u16 minFrame = mStartFrameOrModes[i];
        if (minFrame == 0xFFFF) {  // EventInStatus
            if ((mStopFrameOrPhases[i] == 0 && mEventFrames[i] != 0)) {
                continue;
            }
            if (mEventCheckFuncs[i] && !(this->*mEventCheckFuncs[i])(sequenceFrame)) {
                continue;
            }
        } else if (minFrame == 0xFFFE) {  // EventInPhase
            if (mPhase != mStopFrameOrPhases[i]) {
                continue;
            }
        } else {  // EventOnTime or EventInTime
            if (minFrame > sequenceFrame || sequenceFrame > mStopFrameOrPhases[i]) {
                continue;
            }
        }

        (this->*mEventFuncs[i])(mEventFrames[i], sequenceFrame);
        mEventFrames[i]++;
        if (mStopSequence) {
            break;
        }
    }

    updateAfter();

    if (mNextPhase) {
        mPhase++;
        mFrameInPhase = 0;
        mNextPhase = false;
    } else {
        mFrameInPhase++;
    }

    return mStopSequence;
}

void EventSequence::playSound(const char* pName) {
    MR::startSoundPlayerJ(pName);
}

void EventSequence::playAnimation(const char* pName) {
    MR::startBckPlayerJ(pName);
}

bool EventSequence::isMissLayoutClosed(u16 sequenceFrame) {
    return (MR::isMissLayoutAnimEnd() && sequenceFrame > 15);
}

void EventSequence::setSpot(f32 param1, u32 param2) {
    MR::setPlayerSpot(param1, param2);
}

void EventSequence::decLeft(u16 eventFrame, u16 sequenceFrame) {
    MR::decPlayerLeft();
}

void EventSequence::closeDefaultLayout(u16 eventFrame, u16 sequenceFrame) {
    MR::deactivateDefaultGameLayout();
}

void EventSequence::doCloseWipe(u16 eventFrame, u16 sequenceFrame) {
    if (eventFrame == 0) {
        MR::startDownWipe();
    } else if (!MR::isWipeActive()) {
        mPhase = 2;
    }
}

void EventSequence::doWaitAfterWipe(u16 eventFrame, u16 sequenceFrame) {
    if (eventFrame == 30) {
        MR::requestEndMissDemo();
        stopSequence();
    }
}

EventSequencer::~EventSequencer() {
}

void EventSequence::updateBefore() {};
void EventSequence::updateAfter() {};
