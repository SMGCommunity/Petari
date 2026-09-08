#include "Game/RhythmLib/AudRhythmMeSystem.hpp"
#include "Game/RhythmLib/AudMePlayer.hpp"
#include "Game/RhythmLib/AudMeTrack.hpp"
#include "Game/RhythmLib/AudRhythmHolder.hpp"
#include "Game/RhythmLib/AudRhythmSeqParser.hpp"
#include <JSystem/JAudio2/JAISound.hpp>
#include <JSystem/JAudio2/JASCriticalSection.hpp>
#include <JSystem/JAudio2/JASTrack.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

AudRhythmMeSystem::AudRhythmMeSystem(JKRHeap* pHeap, u32 numSeqParsers, bool hasMgr)
    : JASGlobalInstance< AudRhythmMeSystem >(true), mBgmIdx(), _C(), mMeMgr() {
    mSeqParsers = new (pHeap, 0) AudRhythmSeqParser*[numSeqParsers];
    mRhythmHolders = new (pHeap, 0) AudRhythmHolder[numSeqParsers];

    for (s32 i = 0; i < numSeqParsers; i++) {
        mSeqParsers[i] = new (pHeap, 0) AudRhythmSeqParser();
        mRhythmHolders[i].mSeqParser = mSeqParsers[i];
    }

    mNumSeqParsers = numSeqParsers;

    if (numSeqParsers != 0) {
        setUsingRhythmParser(0);
    }

    if (hasMgr) {
        mMeMgr = new (pHeap, 0) AudMeMgr();
    }
}

bool AudRhythmMeSystem::setSeq(JAISoundHandle& rHandle, s32 parserNo) {
    // FIXME: reg alloc
    if (!rHandle.isSoundAttached()) {
        return false;
    }

    {
        JASCriticalSection crit;
        JASTrack* track = rHandle->getTrack();
        if (track == nullptr) {
            return false;
        }

        track->mSeqCtrl.mParser = mSeqParsers[parserNo];
        if (track->getChild(0) == nullptr) {
            return false;
        }

        track = track->getChild(_C);
        if (track == nullptr) {
            return false;
        }

        track->mSeqCtrl.mParser = mSeqParsers[parserNo];
        track->mute(true);
    }
    mSeqParsers[parserNo]->activate();
    mRhythmHolders[parserNo].init();
    return true;
}

void AudRhythmMeSystem::rejectSeq(s32 parserNo) {
    mSeqParsers[parserNo]->init();
    mRhythmHolders[parserNo].init();
}

void AudRhythmMeSystem::rhythmProc() {
    for (s32 i = 0; i < mNumSeqParsers; i++) {
        mRhythmHolders[i].copyInfoFromParser();
    }
}

void AudRhythmMeSystem::update() {
    for (s32 i = 0; i < mNumSeqParsers; i++) {
        mRhythmHolders[i].movement();
    }

    if (mMeMgr != nullptr) {
        mMeMgr->update();
    }
}

void AudRhythmMeSystem::setIdle(bool idle) {
    for (s32 i = 0; i < mNumSeqParsers; i++) {
        mRhythmHolders[i].mIsPaused = idle;
    }

    if (mMeMgr != nullptr) {
        mMeMgr->setPauseFlagAll(idle);
    }
}

void AudRhythmMeSystem::setUsingRhythmParser(u32 parserNo) {
    for (s32 i = 0; i < mNumSeqParsers; i++) {
        if (i == parserNo) {
            mSeqParsers[i]->mIsUsed = true;
            mSeqParsers[i]->flushChordScale();
        } else {
            mSeqParsers[i]->mIsUsed = false;
        }
    }
    mBgmIdx = parserNo;
}
