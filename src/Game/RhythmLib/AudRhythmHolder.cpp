#include "Game/RhythmLib/AudRhythmHolder.hpp"
#include "Game/RhythmLib/AudRhythmSeqParser.hpp"
#include "JSystem/JAudio2/JASCriticalSection.hpp"
#include "revolution/os/OSTime.h"

AudRhythmHolder::AudRhythmHolder() : mIsPaused(), mSeqParser(), _168() {
    init();
}

void AudRhythmHolder::movement() {
    for (s32 index = 0; index < NUM_RHYTHM_INFO; index++) {
        mInfos[index].mIsActive = false;
    }
}

void AudRhythmHolder::copyInfoFromParser() {
    if (mSeqParser == nullptr) {
        return;
    }

    JASCriticalSection crit;
    mModifiedTempo = mSeqParser->getModifiedTempo();
    mTempo = mSeqParser->getTempo();

    u32 velocity, noteOff, channel;
    s64 time;

    if (mSeqParser->checkNoteOnKey(mSeqParser->mRhythmType0, velocity, noteOff, time, channel)) {
        copyInfoFromParserOne(AudRhythmSeqParser::RhythmType_0, velocity, noteOff, time);
        mIsRhythmActive = true;
    }
    if (mSeqParser->checkNoteOnKey(mSeqParser->mRhythmType1, velocity, noteOff, time, channel)) {
        copyInfoFromParserOne(AudRhythmSeqParser::RhythmType_1, velocity, noteOff, time);
        mIsRhythmActive = true;
    }
    if (mSeqParser->checkNoteOnKey(mSeqParser->mRhythmType2, velocity, noteOff, time, channel)) {
        copyInfoFromParserOne(AudRhythmSeqParser::RhythmType_2, velocity, noteOff, time);
        mIsRhythmActive = true;
    }
    if (mSeqParser->checkNoteOnKey(mSeqParser->mRhythmType3, velocity, noteOff, time, channel)) {
        copyInfoFromParserOne(AudRhythmSeqParser::RhythmType_3, velocity, noteOff, time);
        mIsRhythmActive = true;
    }
    if (mSeqParser->checkNoteOnKey(mSeqParser->mRhythmType4, velocity, noteOff, time, channel)) {
        copyInfoFromParserOne(AudRhythmSeqParser::RhythmType_4, velocity, noteOff, time);
        mIsRhythmActive = true;
    }

    s64 currTime = OSGetTime();
    for (s32 i = 0; i < 6; i++) {
        if (mIsPaused) {
            mInfos[i].mPausedTime = (currTime - mInfos[i].mLastMeasuredTime) + mInfos[i].mPausedTime;
        } else {
            mInfos[i].mPlayTime = (currTime - mInfos[i].mStartTime) - mInfos[i].mPausedTime;
        }
        mInfos[i].mLastMeasuredTime = currTime;
    }
}

void AudRhythmHolder::init() {
    for (s32 i = 0; i < 6; i++) {
        mInfos[i].mIsActive = false;
        mInfos[i]._4 = 0;
        mInfos[i]._8 = 0;
        mInfos[i]._C = 0.0f;
        mInfos[i].mStartTime = 0;
        mInfos[i].mPlayTime = 0;
        mInfos[i]._30 = 0;
        mInfos[i].mLastMeasuredTime = 0;
        mInfos[i].mPausedTime = 0;
    }

    mModifiedTempo = 120.0f;
    mTempo = 120.0f;
    _8 = false;
    mIsRhythmActive = false;
}

void AudRhythmHolder::copyInfoFromParserOne(u32 type, u32 velocity, u32 noteOff, s64 time) {
    mInfos[type].mIsActive = true;
    if (velocity < 100) {
        mInfos[type]._8 = velocity - 10;
        mInfos[type]._4 = 0;
    } else {
        mInfos[type]._4++;
    }
    mInfos[type].mStartTime = time;
    mInfos[type]._C = noteOff / 120.0f * (60.0f / mModifiedTempo);
    mInfos[type]._30++;
    mInfos[type].mPausedTime = 0;
}
