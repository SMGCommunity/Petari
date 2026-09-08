#include "Game/RhythmLib/AudMeTrack.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/RhythmLib/AudChordInfo.hpp"
#include "Game/RhythmLib/AudMeChannelMgr.hpp"
#include "Game/RhythmLib/AudMeSeqCtrl.hpp"
#include "Game/RhythmLib/AudMeTrackCallback.hpp"
#include <JSystem/JAudio2/JASChannel.hpp>
#include <JSystem/JAudio2/JASCriticalSection.hpp>
#include <JSystem/JMath/random.hpp>

AudMeTrack::TList AudMeTrack::sTrackList = AudMeTrack::TList();

u8 AudMeTrack::sReg[AudMeTrack::NUM_REGS];

const JASOscillator::Point AudMeTrack::sAdsTable[4] = {{0, 0, 0x7fff}, {0, 0, 0x7fff}, {0, 0, 0}, {0xe, 0, 0}};

const JASOscillator::Data AudMeTrack::sEnvOsc = {0, 1.0f, 0, 0, 1.0f, 0.0f};

const JASOscillator::Data AudMeTrack::sPitchEnvOsc = {1, 1.0f, 0, 0, 1.0f, 0.0f};

AudMeTrack::AudMeTrack()
    : mSeqCtrl(), mDefaultChannelMgr(this), mNumChannelMgrs(1), mIsPaused(), mIsInStart(true), mStopAfterNote(), mNoStartRange(), mMeId(),
      mChordTableId(-1), mJumpFlag() {
    mChannelMgrs[0] = &mDefaultChannelMgr;
    for (s32 i = 1; i < CHANNEL_MGR_MAX; i++) {
        mChannelMgrs[i] = nullptr;
    }

    init();
}

AudMeTrack::~AudMeTrack() {
    if (mParent == nullptr) {
        sTrackList.Remove(this);
    }
    noteOffAll(0);

    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        if (mChildren[i] != nullptr) {
            delete mChildren[i];
        }
    }
}

void AudMeTrack::init() {
    mParent = nullptr;
    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        mChildren[i] = nullptr;
    }
    mBrotherNo = -1;
    mTrackInfo.init();
    initPlayParam();
    mBankTable = JASDefaultBankTable::getInstance();
    mBaseNote = 60;
    mBaseNotePrev = 60;

    mOscParam[0] = sEnvOsc;
    mOscParam[1] = sPitchEnvOsc;
    for (int i = 0; i < 4; i++) {
        mOscPoint[i] = sAdsTable[i];
    }

    mJumpFlag = false;
    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        _163[i] = 0;
    }
    mIsProcStopped = false;
    mStatus = STATUS_FREE;
}

void AudMeTrack::initPlayParam() {
    mVolume = 1.0f;
    mPitchBend = 0.0f;
    mPan = 0.5f;
    mFxMix = 0.0f;
    mDolby = 0.0f;
}

void AudMeTrack::inherit(AudMeTrack* pTrack) {
    mTrackInfo = pTrack->mTrackInfo;

    const JASOscillator::Data* params = pTrack->mOscParam;
    for (s32 i = 0; i < OSC_NUM; i++) {
        mOscParam[i] = params[i];
    }

    const JASOscillator::Point* points = pTrack->mOscPoint;
    for (s32 i = 0; i < 4; i++) {
        mOscPoint[i] = points[i];
    }

    mSeqCtrl.inherit(&pTrack->mSeqCtrl);

    mStopAfterNote = pTrack->mStopAfterNote;
    if (mStopAfterNote == true) {
        mSeqCtrl.mStopAfterNote = true;
    }

    mJumpFlag = pTrack->mJumpFlag;

    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        _163[i] = pTrack->_163[i];
    }
}

AudMeTrack* AudMeTrack::newChild(s32 childNo) {
    if (mChildren[childNo] != nullptr) {
        return nullptr;
    }

    mChildren[childNo] = new AudMeTrack();
    if (mChildren[childNo] == nullptr) {
        return nullptr;
    }

    mChildren[childNo]->inherit(this);
    mChildren[childNo]->mParent = this;
    mChildren[childNo]->mBrotherNo = childNo;
    return mChildren[childNo];
}

bool AudMeTrack::deleteChild(s32 childNo) {
    // FIXME: reg alloc
    // https://decomp.me/scratch/um0oF

    if (mChildren[childNo] == nullptr) {
        return false;
    }

    mChildren[childNo]->noteOffAll(0);
    delete mChildren[childNo];
    mChildren[childNo] = nullptr;
    return true;
}

bool AudMeTrack::isHaveChild() const {
    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        if (mChildren[i] != nullptr) {
            return true;
        }
    }
    return false;
}

void AudMeTrack::setSeqData(void* pData, u32 meId) {
    mSeqCtrl.start(pData, meId);
    mMeId = meId;
    mStatus = STATUS_READY;
}

void AudMeTrack::startSeq() {
    JASCriticalSection crit;
    sTrackList.append(this);
    mStatus = STATUS_RUN;
}

void AudMeTrack::stopSeq() {
    JASCriticalSection crit;
    noteOffAll(0);
    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        if (mChildren[i] != nullptr) {
            mChildren[i]->noteOffAll(0);
        }
    }
    sTrackList.Remove(this);
    mStatus = STATUS_FREE;
}

void AudMeTrack::suspendSeq() {
    JASCriticalSection crit;
    noteOffAll(0);
    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        if (mChildren[i] != nullptr) {
            mChildren[i]->noteOffAll(0);
        }
    }
    mStatus = STATUS_FREE;
}

void AudMeTrack::stopAtCurrentNoteEnd() {
    JASCriticalSection crit;
    mStopAfterNote = true;
    mSeqCtrl.mStopAfterNote = true;

    if (mParent != nullptr) {
        return;
    }

    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        if (mChildren[i] != nullptr) {
            mChildren[i]->stopAtCurrentNoteEnd();
        }
    }
}

s32 AudMeTrack::rhythmProc(s32 time, s32 a2) {
    if (mParent == nullptr && mStatus == STATUS_FREE) {
        return -1;
    }

    updateChannelParams();
    if (mParent != nullptr) {
        return 0;
    }

    mChordTableId = AudSystem::get()->getChordInfo()->mTableId;

    if (!mIsProcStopped) {
        s32 ret = mSeqCtrl.rhythmProc(this, time);
        if (ret == 1 && mStopAfterNote == true) {
            mIsProcStopped = true;
        }

        if (ret < 0) {
            mIsProcStopped = true;
        }
    }

    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        if (mChildren[i] != nullptr) {
            s32 ret = mChildren[i]->getSeqCtrl()->rhythmProc(mChildren[i], time);
            if (ret == 1 && mStopAfterNote == true) {
                deleteChild(i);
            }

            if (ret < 0) {
                deleteChild(i);
            }
        }
    }

    if (!isHaveChild() && mIsProcStopped) {
        mStatus = STATUS_FREE;
        return -1;
    }

    return 0;
}

void AudMeTrack::setPauseFlag(bool pause) {
    for (s32 i = 0; i < mNumChannelMgrs; i++) {
        if (mChannelMgrs[i] != nullptr) {
            mChannelMgrs[i]->setPauseFlag(pause);
        }
    }

    mIsPaused = pause;

    if (mParent != nullptr) {
        return;
    }

    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        if (mChildren[i] != nullptr) {
            mChildren[i]->setPauseFlag(pause);
        }
    }
}

JASChannel* AudMeTrack::channelStart(AudMeChannelMgr* pChannelMgr, u32 pitch, u32 velocity, u32 time) {
    JASBank* bank = nullptr;
    if (mBankTable != nullptr) {
        bank = mBankTable->getBank(mTrackInfo.mBankNumber);
    }

    JASChannel* channel = JASBank::noteOn(bank, mTrackInfo.mProgNumber, pitch, velocity, mTrackInfo.mReleasePriority << 8 | mTrackInfo.mPlayPriority,
                                          AudMeTrack::channelUpdateCallback, pChannelMgr);

    if (channel == nullptr) {
        return nullptr;
    }

    channel->mPauseFlag = mIsPaused;
    channel->mUpdateTimer = time;
    channel->setParams(pChannelMgr->getChannelParams());
    overwriteOsc(channel);

    if (getRelease() != 0) {
        channel->mOscillators[Oscillator_Env].setDirectRelease(getRelease());
    }

    return channel;
}

bool AudMeTrack::noteOn(u32 channelNo, u32 pitch, u32 velocity) {
    bool success = true;
    for (s32 i = 0; i < mNumChannelMgrs; i++) {
        if (mChannelMgrs[i] != nullptr) {
            mChannelMgrs[i]->noteOff(channelNo, 0);
            JASChannel* channel = channelStart(mChannelMgrs[i], pitch, velocity, 0);
            if (channel == nullptr) {
                success = false;
            }
            mChannelMgrs[i]->mChannels[channelNo] = channel;
        }
    }
    return success;
}

bool AudMeTrack::noteOff(u32 channelNo, u16 directRelease) {
    bool success = true;
    for (s32 i = 0; i < mNumChannelMgrs; i++) {
        if (mChannelMgrs[i] != nullptr) {
            if (!mChannelMgrs[i]->noteOff(channelNo, directRelease)) {
                success = false;
            }
        }
    }
    return success;
}

void AudMeTrack::noteOffAll(u16 directRelease) {
    for (u8 i = 0; i < MAX_CHILDREN; i++) {
        noteOff(i, directRelease);
    }
}

void AudMeTrack::startNote() {
    noteOffAll(0);
    if (mBaseNote != 0x7F) {
        adjustInStartNoteRange(mBaseNote);
        adjustInNoteRange(mBaseNote);
        noteOn(1, mBaseNote, getVelocity());
        sReg[mTrackInfo.mNoteReg] = mBaseNote;
    }
    mBaseNotePrev = mBaseNote;

    if (mIsInStart) {
        AudMeTrack* parent = mParent;
        if (parent != nullptr) {
            AudMeTrack* track = sTrackList.getSameME(parent, parent->mMeId);
            if (track != nullptr && track != nullptr) {
                track->suspendSeq();
            }
            parent->mStatus = STATUS_START;
        }
        mStatus = STATUS_START;
    }
    mIsInStart = false;
}

void AudMeTrack::channelUpdateCallback(u32 callbackType, JASChannel* pChannel, JASDsp::TChannel* pDspChannel, void* mCallbackData) {
    AudMeChannelMgr* mgr = reinterpret_cast< AudMeChannelMgr* >(mCallbackData);

    switch (callbackType) {
    case JASChannel::CB_PLAY:
    case JASChannel::CB_START:
        pChannel->setParams(mgr->getChannelParams());
        break;
    case JASChannel::CB_TIMER:
        pChannel->release(0);
        pChannel->free();
        mgr->mChannels[0] = nullptr;
        break;
    case JASChannel::CB_STOP:
        for (s32 i = 0; i < 8; i++) {
            if (pChannel == mgr->mChannels[i]) {
                mgr->mChannels[i] = nullptr;
                return;
            }
        }
        break;
    }
}

void AudMeTrack::initRegister() {
    for (s32 i = 0; i < NUM_REGS; i++) {
        sReg[i] = 60;
    }
}

void AudMeTrack::overwriteOsc(JASChannel* pChannel) {
    for (u32 i = 0; i < OSC_NUM; i++) {
        if (mOscParam[i].mTable != nullptr) {
            pChannel->setOscInit(i, &mOscParam[i]);
        }
    }
}

void AudMeTrack::setOscAdsr(s16 attackTime, s16 decayTime, s16 sustainTime, s16 sustainLevel, u16 release) {
    mOscParam[Oscillator_Env] = sEnvOsc;
    mOscParam[Oscillator_Env].mTable = &mOscPoint[0];
    mOscPoint[OscillatorPoint_Attack]._2 = attackTime;
    mOscPoint[OscillatorPoint_Decay]._2 = decayTime;
    mOscPoint[OscillatorPoint_Systain]._2 = sustainTime;
    mOscPoint[OscillatorPoint_Systain]._4 = sustainLevel;
    setRelease(release);
}

void AudMeTrack::updateChannelParams() {
    // FIXME: float regswaps
    // https://decomp.me/scratch/WLyeN

    for (s32 i = 0; i < mNumChannelMgrs; i++) {
        AudMeChannelMgr* mgr = mChannelMgrs[i];
        if (mgr == nullptr) {
            continue;
        }

        f32 third = (1.0f / 3.0f);

        f32 bend, vol, pitch, pan, dolby, fxmix;
        vol = mVolume;
        vol *= vol;
        vol *= mgr->mParams.getVolume();

        pan = mPan - 0.5f;
        pitch = 1.0f;
        dolby = mDolby;
        fxmix = mFxMix;

        bend = mPitchBend * mTrackInfo.getBendSense() * third;

        pan += (mgr->mParams.mPan - 0.5f);
        pitch *= mgr->mParams.getPitch();

        dolby += mgr->mParams.getDolby();
        fxmix += mgr->mParams.getFxMix();

        if (isMainTrack()) {
            mgr->mChannelParams.mVolume = vol;
            mgr->mChannelParams.mPitch = pitch;
            mgr->mChannelParams.mPan = pan;
            mgr->mChannelParams.mFxMix = fxmix;
            mgr->mChannelParams.mDolby = dolby;
            mgr->mChannelParams._8 = bend;
        } else {
            AudMeChannelMgr* pmgr = mParent->mChannelMgrs[i];
            if (pmgr == nullptr) {
                pmgr = mParent->mChannelMgrs[0];
            }
            mgr->mChannelParams.mVolume = pmgr->mChannelParams.mVolume * vol;
            mgr->mChannelParams.mPitch = pmgr->mChannelParams.mPitch * pitch;
            mgr->mChannelParams.mPan = (pmgr->mChannelParams.mPan - 0.5f) + pan;
            mgr->mChannelParams.mFxMix = pmgr->mChannelParams.mFxMix + fxmix;
            mgr->mChannelParams.mDolby = pmgr->mChannelParams.mDolby + dolby;
            mgr->mChannelParams._8 = pmgr->mChannelParams._8 + bend;
        }
        mgr->mChannelParams.mPan += 0.5f;
    }
}

bool AudMeTrack::gframeProc() {
    updateChannelParams();
    if (mParent != nullptr) {
        return false;
    }

    for (s32 i = 0; i < MAX_CHILDREN; i++) {
        if (mChildren[i] != nullptr) {
            mChildren[i]->updateChannelParams();
        }
    }

    return false;
}

void AudMeTrack::setBaseNoteFromChord(u8* pChordData, s32 numNotes, bool random) {
    // FIXME: regswap
    // https://decomp.me/scratch/TRQbR

    u8 chordNote;

    AudChordInfo* chordInfo = AudSystem::get()->getChordInfo();
    if (!chordInfo->isAvailable()) {
        mBaseNote = 0x7F;
        return;
    }

    s32 ctrlList[16];
    s32 chordNotes[8];

    makeRandomSearchList(ctrlList, numNotes, random == false);
    makeRandomSearchList(chordNotes, 8, false);  // always randomized

    for (s32 i = 0; i < numNotes; i++) {
        u8 ctrl = pChordData[ctrlList[i]];  // allowed chord tones
        for (s32 j = 0; j < 8; j++) {
            chordNote = chordInfo->mChordNoteList[chordNotes[j]];
            if (chordNote == 0x7F) {
                continue;
            }

            if ((ctrl & 0x01) != 0 && chordInfo->isBass(chordNote)) {
                mBaseNote = chordNote;
                return;
            }
            if ((ctrl & 0x02) != 0 && chordInfo->isRoot(chordNote)) {
                mBaseNote = chordNote;
                return;
            }
            if ((ctrl & 0x04) != 0 && chordInfo->isThird(chordNote)) {
                mBaseNote = chordNote;
                return;
            }
            if ((ctrl & 0x08) != 0 && chordInfo->isFifth(chordNote)) {
                mBaseNote = chordNote;
                return;
            }
            if ((ctrl & 0x10) != 0 && chordInfo->isSixth(chordNote)) {
                mBaseNote = chordNote;
                return;
            }
            if ((ctrl & 0x20) != 0 && chordInfo->isSeventh(chordNote)) {
                mBaseNote = chordNote;
                return;
            }
            if ((ctrl & 0x40) != 0 && chordInfo->isNinth(chordNote)) {
                mBaseNote = chordNote;
                return;
            }
            if ((ctrl & 0x80) != 0 && chordInfo->isEleventh(chordNote)) {
                mBaseNote = chordNote;
                return;
            }
        }
    }
    mBaseNote = 0x7F;
}

void AudMeTrack::setBaseNoteFromScale(u8* pScaleData, s32 numNotes, bool random) {
    AudChordInfo* chordInfo = AudSystem::get()->getChordInfo();
    if (!chordInfo->isAvailable()) {
        mBaseNote = 0x7F;
        return;
    }

    s32 ctrlList[16];

    makeRandomSearchList(ctrlList, numNotes, random == false);

    for (s32 i = 0; i < numNotes; i++) {
        u8 ctrl = pScaleData[ctrlList[i]];  // scale tone offset
        u8 note;
        if ((ctrl & 0x80) != 0) {
            note = chordInfo->mCurScale->up[ctrl & 0x7F];  // getScaleNoteUp
        } else {
            note = chordInfo->mCurScale->down[ctrl];  // getScaleNoteDown
        }
        if (note != 0x7F) {
            mBaseNote = note;
            return;
        }
    }
    mBaseNote = 0x7F;
}

void AudMeTrack::setBaseNoteFromChromatic(u8* pChromData, s32 numNotes, bool random) {
    AudChordInfo* chordInfo = AudSystem::get()->getChordInfo();
    if (!chordInfo->isAvailable()) {
        mBaseNote = 0x7F;
        return;
    }

    s32 ctrlList[12];

    makeRandomSearchList(ctrlList, numNotes, random == false);

    mBaseNote = pChromData[ctrlList[0]];
}

void AudMeTrack::setBaseNoteFromOtherPart(s32 trackNo) {
    mBaseNote = getBrother(trackNo)->mBaseNote;
}

void AudMeTrack::setBaseNoteFromUpperPart() {
    mBaseNote = getBrother(getMyOrderInBrother() - 1)->mBaseNote;
}

void AudMeTrack::setBaseNoteFix(u8 note) {
    mBaseNote = note;
}

void AudMeTrack::setBaseNoteFromRegister(s32 reg) {
    mBaseNote = sReg[reg];
}

void AudMeTrack::adjustNote(s32 direction, s32 type) {
    if (mBaseNote == 0x7F) {
        return;
    }

    AudChordInfo* chordInfo = AudSystem::get()->getChordInfo();
    if (!chordInfo->isAvailable()) {
        return;
    }

    u8 note = mBaseNote;
    switch (type) {
    case ModifyType_Chord: {
        switch (direction) {
        case ModifyDir_Up:
            note = chordInfo->getNearestChordNoteDir(note, true);
            break;
        case ModifyDir_Down:
            note = chordInfo->getNearestChordNoteDir(note, false);
            break;
        case ModifyDir_None:
            note = chordInfo->getNearestChordNote(note);
            break;
        }
        break;
    }
    case ModifyType_Scale: {
        switch (direction) {
        case ModifyDir_Up:
            note = chordInfo->getNearestScaleNoteDir(note, true);
            break;
        case ModifyDir_Down:
            note = chordInfo->getNearestScaleNoteDir(note, false);
            break;
        case ModifyDir_None:
            note = chordInfo->getNearestScaleNote(note);
            break;
        }
        break;
    }
    }

    mBaseNote = note;
}

void AudMeTrack::modifyNote(s32 direction, s32 type, s32 steps) {
    if (mBaseNote == 0x7F) {
        return;
    }

    if (direction == ModifyDir_Rand) {
        direction = getRandom(2) != 0;
    }

    AudChordInfo* chordInfo = AudSystem::get()->getChordInfo();
    if (!chordInfo->isAvailable()) {
        return;
    }

    switch (type) {
    case ModifyType_Chord: {
        switch (direction) {
        case ModifyDir_Up:
            mBaseNote = chordInfo->getUpperNoteOnChord(mBaseNote, steps);
            break;
        case ModifyDir_Down:
            mBaseNote = chordInfo->getLowerNoteOnChord(mBaseNote, steps);
            break;
        }
        break;
    }
    case ModifyType_Scale: {
        switch (direction) {
        case ModifyDir_Up:
            mBaseNote = chordInfo->getUpperNoteOnScale(mBaseNote, steps);
            break;
        case ModifyDir_Down:
            mBaseNote = chordInfo->getLowerNoteOnScale(mBaseNote, steps);
            break;
        }
        break;
    }
    case ModifyType_Steps: {
        if (direction == ModifyDir_Up) {
            mBaseNote += steps;
        } else {
            mBaseNote -= steps;
        }
        break;
    }
    case ModifyType_Octave: {
        if (direction == ModifyDir_Up) {
            mBaseNote += steps * 12;
        } else {
            mBaseNote -= steps * 12;
        }
        break;
    }
    }

    if (mBaseNote != 0x7F) {
        adjustInNoteRange(mBaseNote);
    }
}

void AudMeTrack::modifyNoteRnd(s32 direction, s32 type, s32 range) {
    modifyNote(direction, type, getRandom(range));
}

u32 AudMeTrack::getRandom(u32 range) {
    static JMath::TRandom_fast_ oRandom(0);
    return oRandom.getRand(range);
}

void AudMeTrack::makeRandomSearchList(s32* pList, s32 numItems, bool ordered) {
    for (s32 i = 0; i < numItems; i++) {
        pList[i] = i;
    }

    if (!ordered) {
        for (u32 i = 0; i < numItems; i++) {
            s32 idxA = getRandom(numItems);
            s32 idxB = getRandom(numItems);
            std::swap(pList[idxA], pList[idxB]);
        }
    }
}

void AudMeTrack::adjustInNoteRange(u8& rNote) {
    while (rNote > getNoteRangeUpper()) {
        rNote -= 12;
    }

    while (rNote < getNoteRangeLower()) {
        rNote += 12;
    }
}

void AudMeTrack::adjustInStartNoteRange(u8& rNote) {
    if (!mIsInStart) {
        return;
    }

    if (mNoStartRange) {
        return;
    }

    while (rNote > getNoteRangeStart() + 12) {
        rNote -= 12;
    }

    while (rNote < getNoteRangeStart()) {
        rNote += 12;
    }
}

s32 AudMeTrack::getMyOrderInBrother() {
    if (mParent == nullptr) {
        return -1;
    }

    return mBrotherNo;
}

AudMeTrack* AudMeTrack::getBrother(s32 brotherNo) {
    if (isMainTrack()) {
        return nullptr;
    }

    return getParent()->getChild(brotherNo);
}

void AudMeTrack::TTrackInfo::init() {
    mVelocity = 100;
    mBankNumber = 0;
    mProgNumber = 0xF0;
    mBendSense = 12;
    mRelease = 0;
    mPlayPriority = 64;
    mReleasePriority = 0;
    mNoteReg = 0;
    mNoteRangeLower = 0;
    mNoteRangeUpper = 12 * 10;
    mNoteRangeStart = 12 * 5;
}

s32 AudMeTrack::TList::cbSeqMain(s32 time, s32 a2, void* self) {
    ((AudMeTrack::TList*)self)->seqMain(time, a2);
    return 0;
}

void AudMeTrack::TList::append(AudMeTrack* track) {
    if (!mCallbackRegistered) {
        if (!AudMeTrackCallback::regist(cbSeqMain, this)) {
            return;
        }
        track->initRegister();
        mCallbackRegistered = true;
    }
    Push_front(track);
}

void AudMeTrack::TList::seqMain(s32 time, s32 a2) {
    for (iterator it = begin(); it != end();) {
        AudMeTrack* track = *it;
        ++it;
        if (track->rhythmProc(time, a2) < 0) {
            Remove(track);
        }
    }
}

AudMeTrack* AudMeTrack::TList::getSameME(AudMeTrack* pTrack, u32 meId) {
    for (iterator it = begin(); it != end();) {
        AudMeTrack* track = *it;
        ++it;
        if (track != pTrack && meId == track->mMeId) {
            return track;
        }
    }
    return nullptr;
}
