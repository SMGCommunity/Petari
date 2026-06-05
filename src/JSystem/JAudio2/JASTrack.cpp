#include "JSystem/JAudio2/JASTrack.hpp"
#include "JSystem/JAudio2/JASAiCtrl.hpp"
#include "JSystem/JAudio2/JASChannel.hpp"
#include "JSystem/JAudio2/JASCriticalSection.hpp"
#include "JSystem/JAudio2/JASDriverIF.hpp"
#include "JSystem/JAudio2/JASLfo.hpp"
#include "JSystem/JAudio2/JASSoundParams.hpp"

#include <cstring>
#include <revolution/os.h>

static const JASOscillator::Point sDefaultAdsr[4] = {{0, 0, 0x7fff}, {0, 0, 0x7fff}, {0, 0, 0}, {0xe, 0, 0}};

const JASOscillator::Data JASTrack::sEnvOsc = {0, 1.0f, 0, 0, 1.0f, 0.0f};

const JASOscillator::Data JASTrack::sPitchEnvOsc = {1, 1.0f, 0, 0, 1.0f, 0.0f};

JASDefaultBankTable JASTrack::sDefaultBankTable;

JASTrack::TList JASTrack::sTrackList;

JASTrack::JASTrack() : mSeqCtrl(), mRegisterParam(), mDefaultChannelMgr(this), mChannelMgrCount(1), mStatus(0), mNode() {
    mChannelMgrs[0] = &mDefaultChannelMgr;
    for (u32 i = 1; i < 4; i++) {
        mChannelMgrs[i] = nullptr;
    }
    init();
}

JASTrack::~JASTrack() {
    for (int i = 1; i < 4; i++) {
        delete mChannelMgrs[i];
    }
}

void JASTrack::setChannelMgrCount(u32 count) {
    mChannelMgrCount = 1;
    for (u32 i = 1; i < count; i++, mChannelMgrCount++) {
        if (mChannelMgrs[i])
            continue;
        if (!(mChannelMgrs[i] = new TChannelMgr(this)))
            return;
    }
    for (u32 i = mChannelMgrCount; i < 4; i++) {
        if (mChannelMgrs[i]) {
            delete mChannelMgrs[i];
            mChannelMgrs[i] = nullptr;
        }
    }
}

void JASTrack::init() {
    mSeqCtrl.init();
    mTrackPort.init();
    initTimed();
    mRegisterParam.init();

    memcpy(mOscParam, &sEnvOsc, 0x18);
    memcpy(mOscParam + 1, &sPitchEnvOsc, 0x18);
    for (int i = 0; i < 4; i++) {
        mOscPoint[i] = sDefaultAdsr[i];
    }

    mParent = nullptr;

    for (u32 i = 0; i < 0x10; i++) {
        mChildren[i] = nullptr;
    }

    mChannelMgrs[0]->init();
    mChannelMgrCount = 1;
    for (int i = 1; i < 4; i++) {
        if (mChannelMgrs[i]) {
            delete mChannelMgrs[i];
            mChannelMgrs[i] = nullptr;
        }
    }

    mBankTable = &sDefaultBankTable;

    mPlaytime = 1.0f;
    mSampleInterval = 1.0f;

    mVibDepth = 0.0f;
    mVibPitch = 1.0f;
    mTremDepth = 0.0f;
    mTremPitch = 1.0f;

    mVibDelay = 0;
    mTremDelay = 0;

    mPanPower = 1.0f;
    mDirectRelease = 0;
    mSkipSample = 0;
    mTempo = 120;
    mTimebase = 48;
    mTempoRate = 1.0f;
    updateTempo();

    mTranspose = 0;
    mPitch = 60;

    mBankNumber = 0;
    mProgNumber = 0xF0;
    mBendSense = 0xC;
    mNoteOnPrio = 0x40;
    mReleasePrio = 0;
    mGateRate = 100;

    mMixConfig[0] = 0x150;
    mMixConfig[1] = 0x210;
    mMixConfig[2] = 0x352;
    mMixConfig[3] = 0x412;
    mMixConfig[4] = 0;
    mMixConfig[5] = 0;

    for (u32 i = 0; i < 8; i++) {
        mFIR[i] = 0;
    }
    mFIR[0] = 0x7fff;
    for (u32 i = 0; i < 8; i++) {
        mIIR[i] = 0;
    }
    mIIR[0] = 0x7fff;
    mFilterMode = 0;

    mPauseFlag = false;
    mIsMute = false;
    mIsDirectlyPlayed = true;
    mInvalidateSeq = true;
    mIsOwnedByParent = false;
    mReadyToPlay = false;
    mIsStopped = false;
    byteRepr = byteRepr;
    mStatus = 0;
}

void JASTrack::initTimed() {
    mMoveParam.array[0].mValue = 1.0f;
    mMoveParam.array[1].mValue = 0.0f;
    mMoveParam.array[3].mValue = 0.5f;
    mMoveParam.array[2].mValue = 0.0f;
    mMoveParam.array[4].mValue = 0.0f;
    mMoveParam.array[5].mValue = 0.0f;

    for (u32 i = 0; i < 6; i++) {
        mMoveParam.array[i].mCount = 0;
        mMoveParam.array[i].mTarget = mMoveParam.array[i].mValue;
    }
}

void JASTrack::inherit(const JASTrack& rParent) {
    mIsDirectlyPlayed = rParent.mIsDirectlyPlayed;

    mBankNumber = rParent.mBankNumber;
    mProgNumber = rParent.mProgNumber;

    mNoteOnPrio = rParent.mNoteOnPrio;
    mReleasePrio = rParent.mReleasePrio;

    mSkipSample = rParent.mSkipSample;

    for (u32 i = 0; i < 8; i++) {
        mFIR[i] = rParent.mFIR[i];
    }
    for (u32 i = 0; i < 8; i++) {
        mIIR[i] = rParent.mIIR[i];
    }
    mFilterMode = rParent.mFilterMode;

    for (u32 i = 0; i < 6; i++) {
        mMixConfig[i] = rParent.mMixConfig[i];
    }
}

void JASTrack::assignExtBuffer(u32 mgr, JASSoundParams* buffer) {
    mChannelMgrs[mgr]->mSoundParams = buffer;
}

void JASTrack::setSeqData(void* data, u32 offset) {
    mSeqCtrl.start(data, offset);
}

void JASTrack::startSeq() {
    JASCriticalSection lock;
    sTrackList.append(this);
    mStatus = STATUS_RUN;
}

void JASTrack::stopSeq() {
    JASCriticalSection lock;
    mIsStopped = true;
}

void JASTrack::start() {
    mStatus = STATUS_RUN;
}

void JASTrack::close() {
    for (s32 i = 0; i < 16; i++) {
        JASTrack* currChild = mChildren[i];
        if (currChild != nullptr) {
            currChild->close();
            if (currChild->mIsOwnedByParent) {
                delete currChild;
                mChildren[i] = nullptr;
            }
        }
    }

    for (s32 i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i] != nullptr) {
            mChannelMgrs[i]->releaseAll();
        }
    }

    mParent = nullptr;
    mStatus = STATUS_STOPPED;
}

bool JASTrack::connectChild(u32 idx, JASTrack* child) {
    if (mChildren[idx] != nullptr) {
        return false;
    }
    child->mParent = this;
    mChildren[idx] = child;
    return true;
}

void JASTrack::closeChild(u32 idx) {
    JASTrack* child = mChildren[idx];
    if (child != nullptr) {
        getRootTrack()->updateSeq(false, 1.0f);
        child->close();
        if (child->mIsOwnedByParent) {
            delete child;
            mChildren[idx] = nullptr;
        }
    }
}

JASTrack* JASTrack::openChild(u32 idx) {
    JASTrack* child = mChildren[idx];
    if (child != nullptr) {
        switch (child->mStatus) {
        case 0:
            break;
        case 1:
            getRootTrack()->updateSeq(false, 1.0f);
            child->close();
        case 2:
            bool tmp = child->mIsOwnedByParent;
            child->init();
            child->mIsOwnedByParent = tmp;
            mChildren[idx] = nullptr;
            connectChild(idx, child);
            break;
        }
    } else {
        if (!(child = new JASTrack()))
            return nullptr;
        child->mIsOwnedByParent = true;
        connectChild(idx, child);
    }
    child->setChannelMgrCount(mChannelMgrCount);
    child->inherit(*this);
    return child;
}

void JASTrack::connectBus(int idx, int bus) {
    mMixConfig[idx] = bus;
}

void JASTrack::setLatestKey(u8 key) {
    mPitch = key;
    mPitch += getTransposeTotal();
}

bool JASTrack::noteOn(u32 channelNum, u32 pitch, u32 velocity) {
    if (isMute())
        return false;
    bool ret = true;
    pitch += getTransposeTotal();
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i]) {
            mChannelMgrs[i]->noteOff(channelNum, 0);
            JASChannel* channel;
            if (!(channel = channelStart(mChannelMgrs[i], pitch, velocity, 0)))
                ret = false;
            mChannelMgrs[i]->mChannels[channelNum] = channel;
        }
    }
    return ret;
}

bool JASTrack::gateOn(u32 transposedPitch, u32 velocity, f32 seqTime, u32 flags) {
    transposedPitch += getTransposeTotal();
    if (mGateRate != 100)
        seqTime *= mGateRate / 100.0f;
    s32 pitchDelta;
    u32 isSweep;
    u32 dspTime = seqTimeToDspTime(seqTime);
    isSweep = flags & 1;

    // We only can schedule future notes if we do not want to immediately play the next note?
    u32 updateTimer = (flags & 6) != 0 ? 0 : dspTime;

    u32 pitch;
    if (isSweep)
        pitch = mPitch;
    else
        pitch = transposedPitch;
    pitchDelta = transposedPitch - pitch;
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr* mgr = mChannelMgrs[i];
        if (mgr) {
            if (!mReadyToPlay) {
                mgr->noteOff(0, 0);
                if (!isMute()) {
                    mgr->mChannels[0] = channelStart(mgr, pitch, velocity, updateTimer);
                }
            } else {
                JASChannel* channel = mgr->mChannels[0];
                if (channel) {
                    channel->setKey(pitch - channel->mWaveInfo.mBaseKey);
                    channel->setVelocity(velocity);
                    channel->setUpdateTimer(updateTimer);
                }
            }
            JASChannel* channel;
            if (isSweep && (channel = mgr->mChannels[0])) {
                channel->setKeySweepTarget(pitchDelta, dspTime);
            }
        }
    }
    mPitch = transposedPitch;
    mReadyToPlay = flags & 2;
    return true;
}

bool JASTrack::noteOff(u32 a, u16 b) {
    bool ret = true;
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr* mgr = mChannelMgrs[i];
        if (mgr) {
            if (!mgr->noteOff(a, b))
                ret = false;
        }
    }
    return ret;
}

bool JASTrack::checkNoteStop(u32 channel) const {
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr* mgr = mChannelMgrs[i];
        if (mgr) {
            if (mgr->mChannels[channel])
                return false;
        }
    }
    return true;
}

void JASTrack::overwriteOsc(JASChannel* channel) {
    for (u32 i = 0; i < 2; i++) {
        if (mOscParam[i].mTable != nullptr) {
            channel->setOscInit(i, mOscParam + i);
        }
    }
}

void JASTrack::updateTimedParam() {
    for (u32 i = 0; i < 6; i++) {
        if (mMoveParam.array[i].mCount) {
            mMoveParam.array[i].mValue += (mMoveParam.array[i].mTarget - mMoveParam.array[i].mValue) / mMoveParam.array[i].mCount;
            mMoveParam.array[i].mCount--;
        }
    }
}

void JASTrack::updateTrack(f32 num) {
    updateTempo();
    for (u32 i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr* mgr = mChannelMgrs[i];
        if (mgr) {
            f32 fr8 = mMoveParam.array[0].mValue;
            fr8 *= fr8;
            f32 fr9 = 1.0f;
            f32 fra = mMoveParam.array[1].mValue * mBendSense * 0.333333343267f;
            f32 frb = (mMoveParam.array[3].mValue - 0.5f) * mPanPower;
            f32 frc = mMoveParam.array[2].mValue;
            f32 frd = mMoveParam.array[4].mValue;
            JASSoundParams* params = mgr->mSoundParams;
            if (params) {
                fr8 *= params->mVolume;
                fr9 *= params->mPitch;
                frb += params->mPan - 0.5f;
                frc += params->mFxMix;
                frd += params->mDolby;
            }
            frb *= num;
            if (!mParent) {
                mgr->mParams.mVolume = fr8;
                mgr->mParams.mPitch = fr9;
                mgr->mParams.mPan = frb;
                mgr->mParams.mFxMix = frc;
                mgr->mParams.mDolby = frd;
                mgr->mParams._8 = fra;
            } else {
                TChannelMgr* mgrParent = mParent->mChannelMgrs[i];
                if (!mgrParent)
                    mgrParent = mParent->mChannelMgrs[0];
                mgr->mParams.mVolume = mgrParent->mParams.mVolume * fr8;
                mgr->mParams.mPitch = mgrParent->mParams.mPitch * fr9;
                mgr->mParams.mPan = mgrParent->mParams.mPan - 0.5f + frb;
                mgr->mParams.mFxMix = mgrParent->mParams.mFxMix + frc;
                mgr->mParams.mDolby = mgrParent->mParams.mDolby + frd;
                mgr->mParams._8 = mgrParent->mParams._8 + fra;
            }
            mgr->mParams.mPan += 0.5f;
        }
    }
}

void JASTrack::updateTempo() {
    if (!mParent) {
        f32 rate = JASDriver::getDacRate();
        mSampleInterval = mTempoRate * (1.33333337307f * (mTimebase * mTempo) / rate);
    } else {
        mTempo = mParent->mTempo;
        mTimebase = mParent->mTimebase;
        mSampleInterval = mParent->mSampleInterval;
    }
}

void JASTrack::updateSeq(bool update, f32 num) {
    if (!update)
        update = mInvalidateSeq;
    mInvalidateSeq = false;
    if (update)
        updateTrack(num);
    num *= mPanPower;
    for (int i = 0; i < 0x10; i++) {
        JASTrack* child = mChildren[i];
        if (child && child->mStatus == 1)
            child->updateSeq(update, num);
    }
}

u32 JASTrack::seqTimeToDspTime(f32 seqTime) {
    if (mIsDirectlyPlayed)
        seqTime /= mSampleInterval;
    else {
        seqTime *= 120.0f / mTimebase;
        seqTime *= JASDriver::getSubFrames() / 10.0f;
    }
    return seqTime;
}

void JASTrack::setParam(u32 param, f32 value, u32 b) {
    MoveParam_& timed = mMoveParam.array[param];
    timed.mTarget = value;
    if (!b)
        timed.mValue = timed.mTarget;
    timed.mCount = b;
}

void JASTrack::noteOffAll(u16 a) {
    for (u32 i = 0; i < 8; i++) {
        noteOff((u8)i, a);
    }
    for (int i = 0; i < 0x10; i++) {  // Probably a template
        JASTrack* child = mChildren[i];
        if (child && child->mStatus == 1)
            child->noteOffAll(a);
    }
}

void JASTrack::mute(bool isMute) {
    mIsMute = isMute;
    if (!isMute)
        return;
    noteOffAll(10);
}

void JASTrack::setOscScale(u32 a, f32 scale) {
    mOscParam[a].mScale = scale;
}

void JASTrack::setOscTable(u32 a, const JASOscillator::Point* point) {
    mOscParam[a].mTable = point;
    if (a == 0)
        return;
    mOscParam[a].rel_table = 0;
}

void JASTrack::setOscAdsr(s16 attack, s16 delay, s16 sustain, s16 release, u16 e) {
    memcpy(mOscParam, &sEnvOsc, 0x18);
    mOscPoint[0]._2 = attack;
    mOscParam[0].mTable = mOscPoint;
    mOscPoint[1]._2 = delay;
    mOscPoint[2]._2 = sustain;
    mOscPoint[2]._4 = release;
    mDirectRelease = e;
}

void JASTrack::setFIR(const s16* FIRFilter) {
    for (u32 i = 0; i < 8; i++) {
        mFIR[i] = FIRFilter[i];
    }
    mFilterMode &= 0x20;
    mFilterMode |= 8;
}

void JASTrack::setIIR(const s16* IIRFilter) {
    for (u32 i = 0; i < 8; i++) {
        mIIR[i] = IIRFilter[i];
    }
    mFilterMode |= 0x20;
}

u16 JASTrack::readPortSelf(u32 port) {
    return mTrackPort.readImport(port);
}

void JASTrack::writePortSelf(u32 port, u16 value) {
    mTrackPort.writeExport(port, value);
}

void JASTrack::writePort(u32 port, u16 value) {
    mTrackPort.writeImport(port, value);
    if (port <= 1) {
        mSeqCtrl.interrupt(port == 0 ? JASSeqCtrl::INTRTYPE_VALUE_2 : JASSeqCtrl::INTRTYPE_VALUE_3);
    }
}

u16 JASTrack::readPort(u32 port) {
    return mTrackPort.readExport(port);
}

void JASTrack::setChannelPauseFlag(bool pauseFlag) {
    for (int i = 0; i < mChannelMgrCount; i++) {
        TChannelMgr* mgr = mChannelMgrs[i];
        if (mgr)
            mgr->setPauseFlag(pauseFlag);
    }
    for (int i = 0; i < 0x10; i++) {
        JASTrack* child = mChildren[i];
        if (child)
            child->setChannelPauseFlag(pauseFlag);
    }
}

void JASTrack::pause(bool pauseFlag) {
    if (mPauseFlag != pauseFlag) {
        mPauseFlag = pauseFlag;
        setChannelPauseFlag(pauseFlag);
        mSeqCtrl.interrupt((JASSeqCtrl::IntrType)!pauseFlag);
    }
}

s32 JASTrack::getTransposeTotal() const {
    if (mParent)
        return mParent->getTransposeTotal() + mTranspose;
    else
        return mTranspose;
}

bool JASTrack::isMute() const {
    bool ret;
    if (mParent) {
        ret = false;
        if (mIsMute || mParent->isMute())
            ret = true;
    } else
        ret = mIsMute;
    return ret;
}

void JASTrack::setTempo(u16 tempo) {
    mTempo = tempo;
    mInvalidateSeq = true;
    updateTempo();
}

void JASTrack::setTempoRate(f32 rate) {
    mTempoRate = rate;
    mInvalidateSeq = true;
    updateTempo();
}

void JASTrack::setTimebase(u16 timebase) {
    mTimebase = timebase;
    mInvalidateSeq = true;
    updateTempo();
}

void JASTrack::updateChannel(JASChannel* channel, JASDsp::TChannel* dspChannel) {
    channel->setVibrate(mVibDepth, mVibPitch);
    channel->setTremolo(mTremDepth, mTremPitch);
    if (mFilterMode & 0x20)
        dspChannel->setIIRFilterParam(mIIR);
    if (mFilterMode & 0x1F)
        dspChannel->setFIR8FilterParam(mFIR);
    dspChannel->setFilterMode(mFilterMode);
    dspChannel->setDistFilter(32767.0f * mMoveParam.params.distFilter.mValue);
}

void JASTrack::channelUpdateCallback(u32 a, JASChannel* channel, JASDsp::TChannel* dspChannel, void* data) {
    TChannelMgr* mgr = (TChannelMgr*)data;
    JASTrack* track = mgr->mTrack;
    switch (a) {
    case 0:
    case 1:
        channel->setParams(mgr->mParams);
        track->updateChannel(channel, dspChannel);
        break;
    case 3:
        channel->release(0);
        channel->free();
        mgr->mChannels[0] = nullptr;
        break;
    case 2:
        for (u32 i = 0; i < 8; i++) {
            if (channel == mgr->mChannels[i]) {
                mgr->mChannels[i] = nullptr;
                return;
            }
        }
        break;
    }
}

s32 JASTrack::getChannelCount() const {
    JASCriticalSection lock;
    s32 count = 0;
    for (s32 i = 0; i < mChannelMgrCount; i++) {
        if (mChannelMgrs[i]) {
            for (s32 j = 0; j < 8; j++) {
                if (mChannelMgrs[i]->mChannels[j]) {
                    count++;
                }
            }
        }
    }
    return count;
}

JASTrack* JASTrack::getRootTrack() {
    JASTrack *next, *parent = this;
    for (; next = parent->mParent; parent = next)
        ;
    return parent;
}

int JASTrack::tickProc() {
    if (mPauseFlag)
        return 0;
    int succ = mSeqCtrl.tickProc(this);
    updateTimedParam();
    mInvalidateSeq = true;
    if (succ < 0)
        return -1;
    for (int i = 0; i < 0x10; i++) {
        JASTrack* child = mChildren[i];
        if (child && child->mStatus == 1) {
            if (child->tickProc() < 0) {
                getRootTrack()->updateSeq(false, 1.0f);
                child->close();
                if (child->mIsOwnedByParent) {
                    delete child;
                    mChildren[i] = nullptr;
                }
            }
        }
    }
    return 0;
}

s32 JASTrack::seqMain() {
    if (mIsStopped) {
        updateSeq(true, 1.0f);
        close();
        return -1;
    }
    f32 step = 1.0f;
    while (mPlaytime >= step) {
        mPlaytime -= step;
        if (tickProc() < 0) {
            updateSeq(false, 1.0f);
            close();
            return -1;
        }
    }
    mPlaytime += mSampleInterval;
    updateSeq(false, step);
    return 0;
}

s32 JASTrack::TList::cbSeqMain(void* self) {
    ((JASTrack::TList*)self)->seqMain();
    return 0;
}

void JASTrack::TList::append(JASTrack* track) {
    if (!mCallbackRegistered) {
        if (!JASDriver::registerSubFrameCallback(cbSeqMain, this))
            return;
        mCallbackRegistered = true;
    }
    Push_back(track);
}

void JASTrack::TList::seqMain() {
    iterator iter, next;
    for (iter = begin(); iter != end(); iter = next) {
        next = iter;
        ++next;
        if (iter->seqMain() < 0) {
            Remove(*iter);
            if (iter->mIsOwnedByParent)
                delete *iter;
        }
    }
}

JASTrack::TChannelMgr::TChannelMgr(JASTrack* track) : mParams(), mSoundParams(nullptr), mTrack(track) {
    for (u32 i = 0; i < 8; i++) {
        mChannels[i] = nullptr;
    }
    for (u32 i = 0; i < 8; i++) {
        _38[i] = 0;
    }
}

void JASTrack::TChannelMgr::init() {
    mSoundParams = nullptr;
    mParams.init();
    for (u32 i = 0; i < 8; i++) {
        mChannels[i] = nullptr;
    }
    for (u32 i = 0; i < 8; i++) {
        _38[i] = 0;
    }
}

void JASTrack::TChannelMgr::releaseAll() {
    for (int i = 0; i < 8; i++) {
        JASChannel* channel;
        if ((channel = mChannels[i])) {
            channel->release(0);
            channel->free();
            mChannels[i] = nullptr;
        }
    }
}

bool JASTrack::TChannelMgr::noteOff(u32 a, u16 b) {
    JASChannel* channel = mChannels[a];
    if (!channel)
        return false;

    if (b == 0)
        channel->release(0);
    else
        channel->release(b);

    channel->free();
    mChannels[a] = nullptr;
    return true;
}

void JASTrack::TChannelMgr::setPauseFlag(bool flag) {
    for (int i = 0; i < 8; i++) {
        JASChannel* channel = mChannels[i];
        if (channel)
            channel->setPauseFlag(flag);
    }
}
