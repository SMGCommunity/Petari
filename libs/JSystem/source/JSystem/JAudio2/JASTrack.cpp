#include "JSystem/JAudio2/JASTrack.h"
#include "JSystem/JAudio2/JASChannel.h"
#include "JSystem/JAudio2/JASSoundParams.h"
#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASLfo.h"

#include <cstring>
#include <revolution/os.h>

static const JASOscillator::Point data[4] = {
    {0, 0, 0x7fff},
    {0, 0, 0x7fff},
    {0, 0, 0},
    {0xe, 0, 0}
};

const JASOscillator::Data JASTrack::sEnvOsc = {0, 1f, 0, 0, 1f, 0f};

const JASOscillator::Data JASTrack::sPitchEnvOsc = {1, 1f, 0, 0, 1f, 0f};

JASTrack::JASTrack() : JASSeqCtrl(), _80(), _180(this), _1D0(1), _240(0), _248() {
    _170[0] = &_180;
    for(u32 i = 1; i < 4; i++) {
        _170[i] = nullptr;
    }
    init();
}


JASTrack::~JASTrack() {
    for(int i = 1; i < 4; i++) {
        delete _170[i];
    }
}

void JASTrack::setChannelMgrCount(u32 count) {
    _1D0 = 1;
    for(u32 i = 1; i < count; i++, _1D0++) {
        if(_170[i]) continue;
        if(!(_170[i] = new TChannelMgr(this))) return;
    }
    for(u32 i = _1D0; i < 4; i++) {
        if(_170[i]) {
            delete _170[i];
            _170[i] = nullptr;
        }
    }
}

void JASTrack::init() {
    JASSeqCtrl::init();
    mPorts.init();
    initTimed();
    _80.init();
    memcpy(_E4, &sEnvOsc, 0x18);
    memcpy(_E4 + 1, &sPitchEnvOsc, 0x18);
    for(int i = 0; i < 4; i++) {
        _114[i] = data[i];
    }
    mParent = nullptr;
    for(u32 i = 0; i < 0x10; i++) {
        mChildren[i] = nullptr;
    }
    _170[0]->init();
    _1D0 = 1;
    for(int i = 1; i < 4; i++) {
        if(_170[i]) {
            delete _170[i];
            _170[i] = nullptr;
        }
    }
    _1D4 = &sDefaultBankTable;
    _1D8 = 1f;
    _1DC = 1f;
    _1E0 = 0f;
    _1E4 = 1f;
    _1E8 = 0f;
    _1EC = 1f;
    _1F0 = 0;
    _1F2 = 0;
    _218 = 1f;
    _224 = 0;
    _220 = 0;
    _226 = 0x78;
    mTimerBase = 0x30;
    _21C = 1.0f;
    updateTempo();
    _22A = 0;
    _22B = 0x3C;
    _22C = 0;
    _22E = 0xF0;
    _230 = 0xC;
    _231 = 0x40;
    _232 = 0;
    _233 = 0x64;
    _234[0] = 0x150;
    _234[1] = 0x210;
    _234[2] = 0x352;
    _234[3] = 0x412;
    _234[4] = 0;
    _234[5] = 0;
    for(u32 i = 0; i < 8; i++) {
        _1F4[i] = 0;
    }
    _1F4[0] = 0x7fff;
    for(u32 i = 0; i < 8; i++) {
        _204[i] = 0;
    }
    _204[0] = 0x7fff;
    _214 = 0;
    LLFlags tmp;
    tmp.byteRepr = _244.byteRepr;
    tmp._0 = false;
    tmp._1 = false;
    tmp._2 = true;
    tmp._5 = true;
    tmp._3 = false;
    tmp._4 = false;
    tmp._6 = false;
    _244.byteRepr = tmp.byteRepr;
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
    _244._2 = rParent._244._2;
    _22C = rParent._22C;
    _22E = rParent._22E;
    _231 = rParent._231;
    _232 = rParent._232;
    _220 = rParent._220;
    for(u32 i = 0; i < 8; i++) {
        _1F4[i] = rParent._1F4[i];
    }
    for(u32 i = 0; i < 8; i++) {
        _204[i] = rParent._204[i];
    }
    _214 = rParent._214;
    for(u32 i = 0; i < 6; i++) {
        _234[i] = rParent._234[i];
    }
}

void JASTrack::assignExtBuffer(u32 channel, JASSoundParams *buffer) {
    _170[channel]->_48 = buffer;
}

void JASTrack::setSeqData(void *data, u32 a) {
    JASSeqCtrl::start(data, a);
}

void JASTrack::startSeq() {
    {
        JASCriticalSection section;
        sTrackList.append(this);
        _240 = 1;
    }
    
}

void JASTrack::stopSeq() {
    {
        JASCriticalSection section;
        _244._6 = true;
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
            if(currChild->_244._3) {
                delete currChild;
                mChildren[i] = nullptr;
            }
        }
    }
    for(u32 i = 0; i < _1D0; i++) {
        if(_170[i]) _170[i]->releaseAll();
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
        if(child->_244._3) {
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
                //const bool tmp = child->_244._3;
                bool tmp = (bool)child->_244._3;
                child->init();
                child->_244._3 = tmp;
                mChildren[idx] = nullptr;
                connectChild(idx, child);
                break;
        }
    }
    else {
        if(!(child = new JASTrack())) return nullptr;
        child->_244._3 = true;
        connectChild(idx, child);
    }
    child->setChannelMgrCount(_1D0);
    child->inherit(*this);
    return child;
}

void JASTrack::connectBus(int a, int b) {
    _234[a] = b;
}

void JASTrack::setLatestKey(u8 key) {
    _22B = key;
    _22B += getTransposeTotal();
}

bool JASTrack::noteOn(u32 a, u32 b, u32 c) {
    if(isMute()) return false;
    bool ret = true;
    b += getTransposeTotal();
    for(u32 i = 0; i < _1D0; i++) {
        if(_170[i]) {
            _170[i]->noteOff(a, 0);
            JASChannel *res;
            if(!(res = channelStart(_170[i], b, c, 0))) ret = false;
            _170[i]->_0[a] = res;
        }
    }
    return ret;
}

bool JASTrack::gateOn(u32 a, u32 b, f32 f1, u32 c) {
    a += getTransposeTotal();
    if(_233 != 0x64) f1 *= _233 / 100f;
    s32 r1a;
    u32 r19;
    u32 dspTime = seqTimeToDspTime(f1);
    r19 = c & 1;
    u32 goofy = ((u32)((c & 6) != 0)) ? 0 : dspTime;
    u32 tmp;
    if(r19) tmp = _22B;
    else tmp = a;
    r1a = a - tmp;
    for(u32 i = 0; i < _1D0; i++) {
        TChannelMgr *channel = _170[i];
        if(channel) {
            if(_244._4) {
                channel->noteOff(0, 0);
                if(!isMute()) {
                    channel->_0[0] = channelStart(channel, tmp, b, goofy);
                }
            }
            else {
                JASChannel *llChannel = channel->_0[0];
                if(llChannel) {
                    llChannel->setKey(tmp - llChannel->_E1);
                    llChannel->setVelocity(b);
                    llChannel->setUpdateTimer(goofy);
                }
            }
            JASChannel *llChannel;
            if(r19 && (llChannel = channel->_0[0])) llChannel->setKeySweepTarget(r1a, dspTime);
        }
    }
    _22B = a;
    ((LLFlags*)&_244)->_4 = c & 2;
    return true;
}

bool JASTrack::noteOff(u32 a, u16 b) {
    bool ret = true;
    for(u32 i = 0; i < _1D0; i++) {
        TChannelMgr *channel = _170[i];
        if(channel) {
            if(!channel->noteOff(a, b)) ret = false;
        }
    }
    return ret;
}

bool JASTrack::checkNoteStop(u32 a) const {
    for(u32 i = 0; i < _1D0; i++) {
        TChannelMgr *channel = _170[i];
        if(channel) {
            if(channel->_0[a]) return false;
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
    for(u32 i = 0; i < _1D0; i++) {
        TChannelMgr *channel = _170[i];
        if(channel) {
            f32 fr8 = _9C[0]._0;
            fr8 *= fr8;
            f32 fr9 = 1f;
            f32 fra = _9C[1]._0 * _230 * 0.333333343267f;
            f32 frb = (_9C[3]._0 - 0.5f) * _218;
            f32 frc = _9C[2]._0;
            f32 frd = _9C[4]._0;
            JASSoundParams *params = channel->_48;
            if(params) {
                fr8 *= params->_0[0];
                fr9 *= params->_0[2];
                frb += params->_0[3] - 0.5f;
                frc += params->_0[1];
                frd += params->_0[4];
            }
            frb *= num;
            if(!mParent) {
                channel->_20._0[0] = fr8;
                channel->_20._0[1] = fr9;
                channel->_20._0[3] = frb;
                channel->_20._0[4] = frc;
                channel->_20._0[5] = frd;
                channel->_20._0[2] = fra;
            }
            else {
                TChannelMgr *channelParent = mParent->_170[i];
                if(!channelParent) channelParent = mParent->_170[0];
                channel->_20._0[0] = channelParent->_20._0[0] * fr8;
                channel->_20._0[1] = channelParent->_20._0[1] * fr9;
                channel->_20._0[3] = channelParent->_20._0[3] - 0.5f + frb;
                channel->_20._0[4] = channelParent->_20._0[4] + frc;
                channel->_20._0[5] = channelParent->_20._0[5] + frd;
                channel->_20._0[2] = channelParent->_20._0[2] + fra;
            }
            channel->_20._0[3] += 0.5f;
        }
    }
}

void JASTrack::updateTempo() {
    if(!mParent) {
        f32 rate = JASDriver::getDacRate();
        _1DC = _21C * (1.33333337307f * (mTimerBase * _226) / rate);
    }
    else {
        _226 = mParent->_226;
        mTimerBase = mParent->mTimerBase;
        _1DC = mParent->_1DC;
    }
}

void JASTrack::updateSeq(bool toggle, f32 num) {
    if(!toggle) toggle = _244._5;
    _244._5 = false;
    if(toggle) updateTrack(num);
    num *= _218;
    for(int i = 0; i < 0x10; i++) { // Probably a template
        JASTrack *child = mChildren[i];
        if(child && child->_240 == 1) child->updateSeq(toggle, num);
    }
}

u32 JASTrack::seqTimeToDspTime(f32 seqTime) {
    if(_244._2) seqTime /= _1DC;
    else {
        seqTime *= 120f / mTimerBase;
        seqTime *= JASDriver::getSubFrames() / 10f;
    }
    return seqTime;
}

void JASTrack::setParam(u32 a, f32 value, u32 b) {
    Timed &timed = _9C[a];
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

void JASTrack::mute(bool toggle) {
    _244._1 = toggle;
    if(!toggle) return;
    noteOffAll(10);
}

void JASTrack::setOscScale(u32 a, f32 num) {
    _E4[a]._10 = num;
}

void JASTrack::setOscTable(u32 a, const JASOscillator::Point *point) {
    _E4[a]._8 = point;
    if(!a) return;
    _E4[a]._C = 0;
}

void JASTrack::setOscAdsr(s16 a, s16 b, s16 c, s16 d, u16 e) {
    memcpy(_E4, &sEnvOsc, 0x18);
    _114[0]._2 = a;
    _E4[0]._8 = _114;
    _114[1]._2 = b;
    _114[2]._2 = c;
    _114[2]._4 = d;
    _224 = e; 
}

void JASTrack::setFIR(const s16 *FIR) {
    for(u32 i = 0; i < 8; i++) {
        _1F4[i] = FIR[i];
    }
    _214 &= 0x20;
    _214 |= 8;
}

void JASTrack::setIIR(const s16 *IIR) {
    for(u32 i = 0; i < 8; i++) {
        _204[i] = IIR[i];
    }
    _214 |= 0x20;
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

void JASTrack::setChannelPauseFlag(bool flag) {
    for(int i = 0; i < _1D0; i++) {
        TChannelMgr *channel = _170[i];
        if(channel) channel->setPauseFlag(flag);
    }
    for(int i = 0; i < 0x10; i++) {
        JASTrack *child = mChildren[i];
        if(child) child->setChannelPauseFlag(flag);
    }
}

void JASTrack::pause(bool flag) {
    if(((LLFlags*)&_244)->_0 != flag) {
        ((LLFlags*)&_244)->_0 = flag;
        setChannelPauseFlag(flag);
        JASSeqCtrl::interrupt((JASSeqCtrl::IntrType)!flag);
    }
}

s32 JASTrack::getTransposeTotal() const {
    if(mParent) return mParent->getTransposeTotal() + _22A;
    else return _22A;
}

bool JASTrack::isMute() const {
    bool ret;
    if(mParent) {
        ret = false;
        if(_244._1 || mParent->isMute()) ret = true;
    }
    else ret = _244._1;
    return ret;
}

void JASTrack::setTempo(u16 tempo) {
    _226 = tempo;
    _244._5 = true;
    updateTempo();
}

void JASTrack::setTempoRate(f32 rate) {
    _21C = rate;
    _244._5 = true;
    updateTempo();
}

void JASTrack::setTimebase(u16 timebase) {
    mTimerBase = timebase;
    _244._5 = true;
    updateTempo();
}

void JASTrack::updateChannel(JASChannel *channel, JASDsp::TChannel *dspChannel) {
    channel->setVibrate(_1E0, _1E4);
    channel->setTremolo(_1E8, _1EC);
    if(_214 & 0x20) dspChannel->setIIRFilterParam(_204);
    if(_214 & 0x1F) dspChannel->setFIR8FilterParam(_1F4);
    dspChannel->setFilterMode(_214);
    dspChannel->setDistFilter(32767f * _9C[5]._0);
}

void JASTrack::channelUpdateCallback(u32 a, JASChannel *channel, JASDsp::TChannel *dspChannel, void *data) {
    TChannelMgr *mgr = (TChannelMgr *)data;
    JASTrack *track = mgr->_4C;
    switch(a) {
        case 0:
        case 1:
            channel->setParams(mgr->_20);
            track->updateChannel(channel, dspChannel);
            break;
        case 3:
            channel->release(0);
            channel->free();
            mgr->_0[0] = nullptr;
            break;
        case 2:
            for(u32 i = 0; i < 8; i++) {
                if(channel == mgr->_0[i]) {
                    mgr->_0[i] = nullptr;
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
    if(_244._0) return 0;
    int succ = JASSeqCtrl::tickProc(this);
    updateTimedParam();
    _244._5 = true;
    if(succ < 0) return -1;
    for(int i = 0; i < 0x10; i++) {
        JASTrack *child = mChildren[i];
        if(child && child->_240 == 1) {
            if(child->tickProc() < 0) {
                getRootTrack()->updateSeq(false, 1f);
                child->close();
                if(child->_244._3) {
                    delete child;
                    mChildren[i] = nullptr;
                }
            }
        }
    }
    return 0;
}

s32 JASTrack::seqMain() {
    if(_244._6) {
        updateSeq(true, 1f);
        close();
        return -1;
    }
    f32 num = 1f;
    while(_1D8 >= num) {
        _1D8 -= num;
        if(tickProc() < 0) {
            updateSeq(false, 1f);
            close();
            return -1;
        }
    }
    _1D8 += _1DC;
    updateSeq(false, num);
    return 0;
}

s32 JASTrack::TList::cbSeqMain(void *self) {
    ((JASTrack::TList *)self)->seqMain();
    return 0;
}

void JASTrack::TList::append(JASTrack *track) {
    if(!_C) {
        if(!JASDriver::registerSubFrameCallback(cbSeqMain, this)) return;
        _C = true;
    }
    _0.Push_back(track);
}

void JASTrack::TList::seqMain() {
    JGADGET_LINK_LIST(JASTrack, _248)::iterator iter, next;
    for(iter = JGADGET_LINK_LIST(JASTrack, _248)::iterator(_0.begin()); iter != _0.end(); iter = next) {
        next = iter;
        ++next;
        if(iter->seqMain() < 0) {
            _0.Remove(*iter);
            if(iter->_244._3) delete *iter;
        }
    }
}

JASTrack::TChannelMgr::TChannelMgr(JASTrack *track) : _20(), _48(0), _4C(track) {
    for(u32 i = 0; i < 8; i++) {
        _0[i] = nullptr;
    }
    for(u32 i = 0; i < 8; i++) {
        _38[i] = 0;
    }
}

void JASTrack::TChannelMgr::init() {
    _48 = nullptr;
    _20.init();
    for(u32 i = 0; i < 8; i++) {
        _0[i] = nullptr;
    }
    for(u32 i = 0; i < 8; i++) {
        _38[i] = 0;
    }
}

void JASTrack::TChannelMgr::releaseAll() {
    for(int i = 0; i < 8; i++) {
        JASChannel *channel;
        if(channel = _0[i]) {
            channel->release(0);
            channel->free();
            _0[i] = nullptr;
        }
    }
}

bool JASTrack::TChannelMgr::noteOff(u32 a, u16 b) {
    JASChannel *channel = _0[a];
    if(!channel) return false;

    if(b == 0) channel->release(0);
    else channel->release(b);

    channel->free();
    _0[a] = nullptr;
    return true;
}

void JASTrack::TChannelMgr::setPauseFlag(bool flag) {
    for(int i = 0; i < 8; i++) {
        JASChannel *channel = _0[i];
        if(channel) channel->setPauseFlag(flag);
    }
}

JASDefaultBankTable JASTrack::sDefaultBankTable = JASDefaultBankTable();

JASTrack::TList JASTrack::sTrackList = JASTrack::TList();

template<typename T>
JASMemPool_MultiThreaded<T> JASPoolAllocObject_MultiThreaded<T>::memPool_ = JASMemPool_MultiThreaded<T>();

namespace JGadget {
    
    TLinkListNode::TLinkListNode() : mPrev(nullptr), mNext(nullptr) {}
    
    TNodeLinkList::iterator::iteratorData TNodeLinkList::end() {
        return iterator::iteratorData(iterator(&mEnd).curr);
    }

    TNodeLinkList::iterator::iterator(TLinkListNode *node) {
        curr = node;
    }

    TNodeLinkList::iterator::iterator(const TNodeLinkList::iterator &rOther) : curr(rOther.curr) {}

    TNodeLinkList::iterator::iterator() {}

    TNodeLinkList::iterator::iteratorData TNodeLinkList::begin() {
        return iterator::iteratorData(iterator(mEnd.getNext()).curr);
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
        JGADGET_LINK_LIST(JASTrack, _248)::iterator a,
        JGADGET_LINK_LIST(JASTrack, _248)::iterator b
    ) {
        return !(a == b);
    }

    bool operator== (
        JGADGET_LINK_LIST(JASTrack, _248)::iterator a,
        JGADGET_LINK_LIST(JASTrack, _248)::iterator b
    ) {
        return (TNodeLinkList::iterator)a == (TNodeLinkList::iterator)b;
    }

    bool operator==(TNodeLinkList::iterator a, TNodeLinkList::iterator b) {
        return a.curr == b.curr;
    }

    TNodeLinkList::TNodeLinkList() : mEnd() {
        Initialize_();
    }
}

JASTrack::TList::~TList() {}

JASTrack::TList::TList() : _0(), _C(false) {}

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
