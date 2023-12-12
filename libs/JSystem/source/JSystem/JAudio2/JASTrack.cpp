#include "JSystem/JAudio2/JASTrack.hpp"
#include "JSystem/JAudio2/JASChannel.hpp"
#include "JSystem/JAudio2/JASSoundParams.hpp"
#include "JSystem/JAudio2/JASAiCtrl.hpp"
#include "JSystem/JAudio2/JASDriverIF.hpp"
#include "JSystem/JAudio2/JASLfo.hpp"

#include <cstring>
#include <revolution/os.h>

static const JASOscillator::Point sDefaultAdsr[4] = {
    {0, 0, 0x7fff},
    {0, 0, 0x7fff},
    {0, 0, 0},
    {0xe, 0, 0}
};

const JASOscillator::Data JASTrack::sEnvOsc = {0, 1.0f, 0, 0, 1.0f, 0.0f};

const JASOscillator::Data JASTrack::sPitchEnvOsc = {1, 1.0f, 0, 0, 1.0f, 0.0f};

JASTrack::JASTrack() : JASSeqCtrl(), mRegs(), mInitialMgr(this), mNumChannels(1), _240(0), mNode() {
    mMgrs[0] = &mInitialMgr;
    for(u32 i = 1; i < 4; i++) {
        mMgrs[i] = nullptr;
    }
    init();
}


JASTrack::~JASTrack() {
    for(int i = 1; i < 4; i++) {
        delete mMgrs[i];
    }
}

void JASTrack::setChannelMgrCount(u32 count) {
    mNumChannels = 1;
    for(u32 i = 1; i < count; i++, mNumChannels++) {
        if(mMgrs[i]) continue;
        if(!(mMgrs[i] = new TChannelMgr(this))) return;
    }
    for(u32 i = mNumChannels; i < 4; i++) {
        if(mMgrs[i]) {
            delete mMgrs[i];
            mMgrs[i] = nullptr;
        }
    }
}

void JASTrack::init() {
    JASSeqCtrl::init();
    mPorts.init();
    initTimed();
    mRegs.init();
    
    memcpy(_E4, &sEnvOsc, 0x18);
    memcpy(_E4 + 1, &sPitchEnvOsc, 0x18);
    for(int i = 0; i < 4; i++) {
        mAdsr[i] = sDefaultAdsr[i];
    }
    
    mParent = nullptr;

    for(u32 i = 0; i < 0x10; i++) {
        mChildren[i] = nullptr;
    }

    mMgrs[0]->init();
    mNumChannels = 1;
    for(int i = 1; i < 4; i++) {
        if(mMgrs[i]) {
            delete mMgrs[i];
            mMgrs[i] = nullptr;
        }
    }

    mBankTable = &sDefaultBankTable;

    mPlaytime = 1f;
    mSampleInterval = 1f;

    _1E0 = 0f;
    _1E4 = 1f;
    _1E8 = 0f;
    _1EC = 1f;

    _1F0 = 0;
    _1F2 = 0;

    _218 = 1f;
    _224 = 0;
    _220 = 0;
    mTempo = 120;
    mTimebase = 48;
    mTempoRate = 1.0f;
    updateTempo();

    mTransposeAmt = 0;
    mPitch = 0x3C;

    mBank = 0;
    mPrg = 0xF0;
    _230 = 0xC;
    _231 = 0x40;
    _232 = 0;
    mGateLatency = 100;
    
    mBuses[0] = 0x150;
    mBuses[1] = 0x210;
    mBuses[2] = 0x352;
    mBuses[3] = 0x412;
    mBuses[4] = 0;
    mBuses[5] = 0;

    for(u32 i = 0; i < 8; i++) {
        mFIRFilter[i] = 0;
    }
    mFIRFilter[0] = 0x7fff;
    for(u32 i = 0; i < 8; i++) {
        mIIRFilter[i] = 0;
    }
    mIIRFilter[0] = 0x7fff;
    mFilterMode = 0;

    mPauseFlag = false;
    mIsMute = false;
    mIsDirectlyPlayed = true;
    mInvalidateSeq = true;
    mIsOwnedByParent = false;
    mReadyToPlay = false;
    mIsStopped = false;
    byteRepr = byteRepr;
    _240 = 0;
}

void JASTrack::initTimed() {
    _9C[0]._0 = 1f;
    _9C[1]._0 = 0f;
    _9C[3]._0 = 0.5f;
    _9C[2]._0 = 0f;
    _9C[4]._0 = 0f;
    _9C[5]._0 = 0f;
    
    for(u32 i = 0; i < 6; i++) {
        _9C[i]._8 = 0;
        _9C[i]._4 = _9C[i]._0;
    }
}

void JASTrack::inherit(const JASTrack &rParent) {
    mIsDirectlyPlayed = rParent.mIsDirectlyPlayed;
    
    mBank = rParent.mBank;
    mPrg = rParent.mPrg;

    _231 = rParent._231;
    _232 = rParent._232;

    _220 = rParent._220;

    for(u32 i = 0; i < 8; i++) {
        mFIRFilter[i] = rParent.mFIRFilter[i];
    }
    for(u32 i = 0; i < 8; i++) {
        mIIRFilter[i] = rParent.mIIRFilter[i];
    }
    mFilterMode = rParent.mFilterMode;

    for(u32 i = 0; i < 6; i++) {
        mBuses[i] = rParent.mBuses[i];
    }
}

void JASTrack::assignExtBuffer(u32 mgr, JASSoundParams *buffer) {
    mMgrs[mgr]->_48 = buffer;
}

void JASTrack::setSeqData(void *data, u32 offset) {
    JASSeqCtrl::start(data, offset);
}

void JASTrack::startSeq() {
    {
        JASCriticalSection lock;
        sTrackList.append(this);
        _240 = 1;
    }
    
}

void JASTrack::stopSeq() {
    {
        JASCriticalSection lock;
        mIsStopped = true;
    }
}

void JASTrack::start() {
    _240 = 1;
}

void JASTrack::close() {
    for(int i = 0; i < 0x10; i++) {
        JASTrack *currChild;
        currChild = mChildren[i];
        if(currChild) {
            currChild->close();
            if(currChild->mIsOwnedByParent) {
                delete currChild;
                mChildren[i] = nullptr;
            }
        }
    }
    for(u32 i = 0; i < mNumChannels; i++) {
        if(mMgrs[i]) mMgrs[i]->releaseAll();
    }
    mParent = nullptr;
    _240 = 2;
}

bool JASTrack::connectChild(u32 idx, JASTrack *child) {
    if(mChildren[idx]) return false;
    child->mParent = this;
    mChildren[idx] = child;
    return true;
}

void JASTrack::closeChild(u32 idx) {
    JASTrack *child;
    if(child = mChildren[idx]) {
        getRootTrack()->updateSeq(false, 1f);
        child->close();
        if(child->mIsOwnedByParent) {
            delete child;
            mChildren[idx] = nullptr;
        }
    }
}

JASTrack* JASTrack::openChild(u32 idx) {
    JASTrack *child;
    child = mChildren[idx];
    if(child) {
        switch(child->_240) {
            case 0: break;
            case 1:
                getRootTrack()->updateSeq(false, 1f);
                child->close();
            case 2:
                bool tmp = child->mIsOwnedByParent;
                child->init();
                child->mIsOwnedByParent = tmp;
                mChildren[idx] = nullptr;
                connectChild(idx, child);
                break;
        }
    }
    else {
        if(!(child = new JASTrack())) return nullptr;
        child->mIsOwnedByParent = true;
        connectChild(idx, child);
    }
    child->setChannelMgrCount(mNumChannels);
    child->inherit(*this);
    return child;
}

void JASTrack::connectBus(int idx, int bus) {
    mBuses[idx] = bus;
}

void JASTrack::setLatestKey(u8 key) {
    mPitch = key;
    mPitch += getTransposeTotal();
}

bool JASTrack::noteOn(u32 channelNum, u32 pitch, u32 c) {
    if(isMute()) return false;
    bool ret = true;
    pitch += getTransposeTotal();
    for(u32 i = 0; i < mNumChannels; i++) {
        if(mMgrs[i]) {
            mMgrs[i]->noteOff(channelNum, 0);
            JASChannel *channel;
            if(!(channel = channelStart(mMgrs[i], pitch, c, 0))) ret = false;
            mMgrs[i]->mChannels[channelNum] = channel;
        }
    }
    return ret;
}

bool JASTrack::gateOn(u32 transposedPitch, u32 velocity, f32 seqTime, u32 flags) {
    
    transposedPitch += getTransposeTotal();
    if(mGateLatency != 100) seqTime *= mGateLatency / 100f;
    s32 pitchDelta;
    u32 isSweep;
    u32 dspTime = seqTimeToDspTime(seqTime);
    isSweep = flags & 1;

    // We only can schedule future notes if we do not want to immediately play the next note?
    u32 updateTimer = (flags & 6) != 0 ? 0 : dspTime;

    u32 pitch;
    if(isSweep) pitch = mPitch;
    else pitch = transposedPitch;
    pitchDelta = transposedPitch - pitch;
    for(u32 i = 0; i < mNumChannels; i++) {
        TChannelMgr *mgr = mMgrs[i];
        if(mgr) {
            if(mReadyToPlay) {
                mgr->noteOff(0, 0);
                if(!isMute()) {
                    mgr->mChannels[0] = channelStart(mgr, pitch, velocity, updateTimer);
                }
            }
            else {
                JASChannel *channel = mgr->mChannels[0];
                if(channel) {
                    channel->setKey(pitch - channel->_E1);
                    channel->setVelocity(velocity);
                    channel->setUpdateTimer(updateTimer);
                }
            }
            JASChannel *channel;
            if(isSweep && (channel = mgr->mChannels[0])) {
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
    for(u32 i = 0; i < mNumChannels; i++) {
        TChannelMgr *mgr = mMgrs[i];
        if(mgr) {
            if(!mgr->noteOff(a, b)) ret = false;
        }
    }
    return ret;
}

bool JASTrack::checkNoteStop(u32 channel) const {
    for(u32 i = 0; i < mNumChannels; i++) {
        TChannelMgr *mgr = mMgrs[i];
        if(mgr) {
            if(mgr->mChannels[channel]) return false;
        } 
    }
    return true;
}

void JASTrack::overwriteOsc(JASChannel *channel) {
    for(u32 i = 0; i < 2; i++) {
        if(_E4[i]._8) {
            channel->setOscInit(i, _E4 + i);
        }
    }
}

void JASTrack::updateTimedParam() {
    for(u32 i = 0; i < 6; i++) {
        if(_9C[i]._8) {
            _9C[i]._0 += (_9C[i]._4 - _9C[i]._0) / _9C[i]._8;
            _9C[i]._8--;
        }
    }
}

void JASTrack::updateTrack(f32 num) {
    updateTempo();
    for(u32 i = 0; i < mNumChannels; i++) {
        TChannelMgr *mgr = mMgrs[i];
        if(mgr) {
            f32 fr8 = _9C[0]._0;
            fr8 *= fr8;
            f32 fr9 = 1f;
            f32 fra = _9C[1]._0 * _230 * 0.333333343267f;
            f32 frb = (_9C[3]._0 - 0.5f) * _218;
            f32 frc = _9C[2]._0;
            f32 frd = _9C[4]._0;
            JASSoundParams *params = mgr->_48;
            if(params) {
                fr8 *= params->_0[0];
                fr9 *= params->_0[2];
                frb += params->_0[3] - 0.5f;
                frc += params->_0[1];
                frd += params->_0[4];
            }
            frb *= num;
            if(!mParent) {
                mgr->mChannelParams._0[0] = fr8;
                mgr->mChannelParams._0[1] = fr9;
                mgr->mChannelParams._0[3] = frb;
                mgr->mChannelParams._0[4] = frc;
                mgr->mChannelParams._0[5] = frd;
                mgr->mChannelParams._0[2] = fra;
            }
            else {
                TChannelMgr *mgrParent = mParent->mMgrs[i];
                if(!mgrParent) mgrParent = mParent->mMgrs[0];
                mgr->mChannelParams._0[0] = mgrParent->mChannelParams._0[0] * fr8;
                mgr->mChannelParams._0[1] = mgrParent->mChannelParams._0[1] * fr9;
                mgr->mChannelParams._0[3] = mgrParent->mChannelParams._0[3] - 0.5f + frb;
                mgr->mChannelParams._0[4] = mgrParent->mChannelParams._0[4] + frc;
                mgr->mChannelParams._0[5] = mgrParent->mChannelParams._0[5] + frd;
                mgr->mChannelParams._0[2] = mgrParent->mChannelParams._0[2] + fra;
            }
            mgr->mChannelParams._0[3] += 0.5f;
        }
    }
}

void JASTrack::updateTempo() {
    if(!mParent) {
        f32 rate = JASDriver::getDacRate();
        mSampleInterval = mTempoRate * (1.33333337307f * (mTimebase * mTempo) / rate);
    }
    else {
        mTempo = mParent->mTempo;
        mTimebase = mParent->mTimebase;
        mSampleInterval = mParent->mSampleInterval;
    }
}

void JASTrack::updateSeq(bool update, f32 num) {
    if(!update) update = mInvalidateSeq;
    mInvalidateSeq = false;
    if(update) updateTrack(num);
    num *= _218;
    for(int i = 0; i < 0x10; i++) {
        JASTrack *child = mChildren[i];
        if(child && child->_240 == 1) child->updateSeq(update, num);
    }
}

u32 JASTrack::seqTimeToDspTime(f32 seqTime) {
    if(mIsDirectlyPlayed) seqTime /= mSampleInterval;
    else {
        seqTime *= 120f / mTimebase;
        seqTime *= JASDriver::getSubFrames() / 10f;
    }
    return seqTime;
}

void JASTrack::setParam(u32 param, f32 value, u32 b) {
    Timed &timed = _9C[param];
    timed._4 = value;
    if(!b) timed._0 = timed._4;
    timed._8 = b;
}

void JASTrack::noteOffAll(u16 a) {
    for(u32 i = 0; i < 8; i++) {
        noteOff((u8)i, a);
    }
    for(int i = 0; i < 0x10; i++) { // Probably a template
        JASTrack *child = mChildren[i];
        if(child && child->_240 == 1) child->noteOffAll(a);
    }
}

void JASTrack::mute(bool isMute) {
    mIsMute = isMute;
    if(!isMute) return;
    noteOffAll(10);
}

void JASTrack::setOscScale(u32 a, f32 scale) {
    _E4[a]._10 = scale;
}

void JASTrack::setOscTable(u32 a, const JASOscillator::Point *point) {
    _E4[a]._8 = point;
    if(a == 0) return;
    _E4[a]._C = 0;
}

void JASTrack::setOscAdsr(s16 attack, s16 delay, s16 sustain, s16 release, u16 e) {
    memcpy(_E4, &sEnvOsc, 0x18);
    mAdsr[0]._2 = attack;
    _E4[0]._8 = mAdsr;
    mAdsr[1]._2 = delay;
    mAdsr[2]._2 = sustain;
    mAdsr[2]._4 = release;
    _224 = e; 
}

void JASTrack::setFIR(const s16 *FIRFilter) {
    for(u32 i = 0; i < 8; i++) {
        mFIRFilter[i] = FIRFilter[i];
    }
    mFilterMode &= 0x20;
    mFilterMode |= 8;
}

void JASTrack::setIIR(const s16 *IIRFilter) {
    for(u32 i = 0; i < 8; i++) {
        mIIRFilter[i] = IIRFilter[i];
    }
    mFilterMode |= 0x20;
}

u16 JASTrack::readPortSelf(u32 port) {
    return mPorts.readImport(port);
}

void JASTrack::writePortSelf(u32 port, u16 value) {
    mPorts.writeExport(port, value);
}

void JASTrack::writePort(u32 port, u16 value) {
    mPorts.writeImport(port, value);
    if(port <= 1) {
        JASSeqCtrl::interrupt(port == 0 ? JASSeqCtrl::INTR_2 : JASSeqCtrl::INTR_3);
    }
}

u16 JASTrack::readPort(u32 port) {
    return mPorts.readExport(port);
}

void JASTrack::setChannelPauseFlag(bool pauseFlag) {
    for(int i = 0; i < mNumChannels; i++) {
        TChannelMgr *mgr = mMgrs[i];
        if(mgr) mgr->setPauseFlag(pauseFlag);
    }
    for(int i = 0; i < 0x10; i++) {
        JASTrack *child = mChildren[i];
        if(child) child->setChannelPauseFlag(pauseFlag);
    }
}

void JASTrack::pause(bool pauseFlag) {
    if(mPauseFlag != pauseFlag) {
        mPauseFlag = pauseFlag;
        setChannelPauseFlag(pauseFlag);
        JASSeqCtrl::interrupt((JASSeqCtrl::IntrType)!pauseFlag);
    }
}

s32 JASTrack::getTransposeTotal() const {
    if(mParent) return mParent->getTransposeTotal() + mTransposeAmt;
    else return mTransposeAmt;
}

bool JASTrack::isMute() const {
    bool ret;
    if(mParent) {
        ret = false;
        if(mIsMute || mParent->isMute()) ret = true;
    }
    else ret = mIsMute;
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

void JASTrack::updateChannel(JASChannel *channel, JASDsp::TChannel *dspChannel) {
    channel->setVibrate(_1E0, _1E4);
    channel->setTremolo(_1E8, _1EC);
    if(mFilterMode & 0x20) dspChannel->setIIRFilterParam(mIIRFilter);
    if(mFilterMode & 0x1F) dspChannel->setFIR8FilterParam(mFIRFilter);
    dspChannel->setFilterMode(mFilterMode);
    dspChannel->setDistFilter(32767f * _9C[5]._0);
}

void JASTrack::channelUpdateCallback (
    u32 a, JASChannel *channel, JASDsp::TChannel *dspChannel, void *data
) {
    TChannelMgr *mgr = (TChannelMgr *)data;
    JASTrack *track = mgr->mParentTrack;
    switch(a) {
        case 0:
        case 1:
            channel->setParams(mgr->mChannelParams);
            track->updateChannel(channel, dspChannel);
            break;
        case 3:
            channel->release(0);
            channel->free();
            mgr->mChannels[0] = nullptr;
            break;
        case 2:
            for(u32 i = 0; i < 8; i++) {
                if(channel == mgr->mChannels[i]) {
                    mgr->mChannels[i] = nullptr;
                    return;
                }
            }
            break;
    }
}

JASTrack* JASTrack::getRootTrack() {
    JASTrack *next, *parent = this;
    for(; next = parent->mParent; parent = next);
    return parent;
}

int JASTrack::tickProc() {
    if(mPauseFlag) return 0;
    int succ = JASSeqCtrl::tickProc(this);
    updateTimedParam();
    mInvalidateSeq = true;
    if(succ < 0) return -1;
    for(int i = 0; i < 0x10; i++) {
        JASTrack *child = mChildren[i];
        if(child && child->_240 == 1) {
            if(child->tickProc() < 0) {
                getRootTrack()->updateSeq(false, 1f);
                child->close();
                if(child->mIsOwnedByParent) {
                    delete child;
                    mChildren[i] = nullptr;
                }
            }
        }
    }
    return 0;
}

s32 JASTrack::seqMain() {
    if(mIsStopped) {
        updateSeq(true, 1f);
        close();
        return -1;
    }
    f32 step = 1f;
    while(mPlaytime >= step) {
        mPlaytime -= step;
        if(tickProc() < 0) {
            updateSeq(false, 1f);
            close();
            return -1;
        }
    }
    mPlaytime += mSampleInterval;
    updateSeq(false, step);
    return 0;
}

s32 JASTrack::TList::cbSeqMain(void *self) {
    ((JASTrack::TList *)self)->seqMain();
    return 0;
}

void JASTrack::TList::append(JASTrack *track) {
    if(!mIsInit) {
        if(!JASDriver::registerSubFrameCallback(cbSeqMain, this)) return;
        mIsInit = true;
    }
    mList.Push_back(track);
}

void JASTrack::TList::seqMain() {
    InternalList::iterator iter, next;
    for(iter = InternalList::iterator(mList.begin()); iter != mList.end(); iter = next) {
        next = iter;
        ++next;
        if(iter->seqMain() < 0) {
            mList.Remove(*iter);
            if(iter->mIsOwnedByParent) delete *iter;
        }
    }
}

JASTrack::TChannelMgr::TChannelMgr(JASTrack *track)
    : mChannelParams(), _48(0), mParentTrack(track) {
    for(u32 i = 0; i < 8; i++) {
        mChannels[i] = nullptr;
    }
    for(u32 i = 0; i < 8; i++) {
        _38[i] = 0;
    }
}

void JASTrack::TChannelMgr::init() {
    _48 = nullptr;
    mChannelParams.init();
    for(u32 i = 0; i < 8; i++) {
        mChannels[i] = nullptr;
    }
    for(u32 i = 0; i < 8; i++) {
        _38[i] = 0;
    }
}

void JASTrack::TChannelMgr::releaseAll() {
    for(int i = 0; i < 8; i++) {
        JASChannel *channel;
        if(channel = mChannels[i]) {
            channel->release(0);
            channel->free();
            mChannels[i] = nullptr;
        }
    }
}

bool JASTrack::TChannelMgr::noteOff(u32 a, u16 b) {
    JASChannel *channel = mChannels[a];
    if(!channel) return false;

    if(b == 0) channel->release(0);
    else channel->release(b);

    channel->free();
    mChannels[a] = nullptr;
    return true;
}

void JASTrack::TChannelMgr::setPauseFlag(bool flag) {
    for(int i = 0; i < 8; i++) {
        JASChannel *channel = mChannels[i];
        if(channel) channel->setPauseFlag(flag);
    }
}

JASDefaultBankTable JASTrack::sDefaultBankTable = JASDefaultBankTable();

JASTrack::TList JASTrack::sTrackList = JASTrack::TList();

namespace JGadget {
    
    TLinkListNode::TLinkListNode() : mPrev(nullptr), mNext(nullptr) {}
    
    TLinkListNode* TNodeLinkList::end() {
        return iterator(&mEnd).curr;
    }

    TNodeLinkList::iterator::iterator(TLinkListNode *node) {
        curr = node;
    }

    TNodeLinkList::iterator::iterator(const TNodeLinkList::iterator &rOther) : curr(rOther.curr) {}

    TNodeLinkList::iterator::iterator() {}

    TLinkListNode* TNodeLinkList::begin() {
        return iterator(mEnd.getNext()).curr;
    }

    TLinkListNode* TLinkListNode::getNext() const {
        return mNext;
    }

    TNodeLinkList::iterator& TNodeLinkList::iterator::operator++() {
        curr = curr->getNext();
        return *this;
    }

    TLinkListNode* TNodeLinkList::iterator::operator->() const {
        return curr;
    }

    bool operator!= (
        JASTrack::TList::InternalList::iterator a,
        JASTrack::TList::InternalList::iterator b
    ) {
        return !(a == b);
    }

    bool operator== (
        JASTrack::TList::InternalList::iterator a,
        JASTrack::TList::InternalList::iterator b
    ) {
        return CALL_INLINE_FUNC(TNodeLinkList::iterator, a.curr)
            == CALL_INLINE_FUNC(TNodeLinkList::iterator, b.curr);
    }

    bool operator==(TNodeLinkList::iterator a, TNodeLinkList::iterator b) {
        return a.curr == b.curr;
    }

    TNodeLinkList::TNodeLinkList() : mEnd() {
        Initialize_();
    }
}

JASTrack::TList::~TList() {}

JASTrack::TList::TList() : mList(), mIsInit(false) {}

JASCriticalSection::JASCriticalSection() {
    success = OSDisableInterrupts();
}

JASDefaultBankTable::JASDefaultBankTable() : JASBankTable(), JASGlobalInstance(true) {}

JASDefaultBankTable::~JASDefaultBankTable() {}

void JASChannel::setUpdateTimer(u32 timer) {
    _14 = timer;
}

void JASChannel::setKey(s32 key) {
    _C8 = key;
}

void JASChannel::setVelocity(u32 velocity) {
    _CA = velocity;
}

void JASChannel::setVibrate(f32 a, f32 b) {
    _5C.setDepth(a);
    _5C.setPitch(b);
}

void JASChannel::setTremolo(f32 a, f32 b) {
    _74.setDepth(a);
    _74.setPitch(b);
}

void JASChannelParams::init() {
    _0[0] = 1f;
    _0[1] = 1f;
    _0[2] = 0f;
    _0[3] = 0.5f;
    _0[4] = 0f;
    _0[5] = 0f;
}

JASChannelParams::JASChannelParams() {
    _0[0] = 1f;
    _0[1] = 1f;
    _0[2] = 0f;
    _0[3] = 0.5f;
    _0[4] = 0f;
    _0[5] = 0f;
}

void JASChannel::setPauseFlag(bool flag) {
    _4 = flag;
}

void JASLfo::setDepth(f32 depth) {
    _C = depth;
}

void JASLfo::setPitch(f32 pitch) {
    _10 = pitch;
}

JASBankList::JASBankList() {}
