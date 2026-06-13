#include "JSystem/JAudio2/JAISeqMgr.hpp"
#include "JSystem/JAudio2/JAISeq.hpp"
#include "JSystem/JAudio2/JAISoundHandles.hpp"
#include "JSystem/JAudio2/JAISoundInfo.hpp"

bool JAISeqMgr::isUsingSeqData(const JAISeqDataRegion& seqDataRegion) {
    JSULink< JAISeq >* link;
    for (link = mSeqList.getFirst(); link != mSeqList.getEnd(); link = link->getNext()) {
        if (seqDataRegion.intersects(link->getObject()->getSeqData())) {
            return true;
        }
    }
    return false;
}

int JAISeqMgr::releaseSeqData(const JAISeqDataRegion& seqDataRegion) {
    bool bVar1 = false;
    JSULink< JAISeq >* link;
    for (link = mSeqList.getFirst(); link != mSeqList.getEnd(); link = link->getNext()) {
        if (seqDataRegion.intersects(link->getObject()->getSeqData())) {
            link->getObject()->stop();
            bVar1 = true;
        }
    }

    return bVar1 ? RELEASE_SEQ_1 : RELEASE_SEQ_2;
}

JAISeqMgr::JAISeqMgr(bool setInstance) : JASGlobalInstance< JAISeqMgr >(setInstance), mAudience(nullptr) {
    seqDataMgr = nullptr;
    soundStrategyMgr = nullptr;
    mNumTracks = 16;
    mMove.init();
    mSoundActivity.init();
}

void JAISeqMgr::freeDeadSeq_() {
    JSULink< JAISeq >* i = mSeqList.getFirst();
    while (i != nullptr) {
        JAISeq* seq = i->getObject();
        JSULink< JAISeq >* next = i->getNext();
        if (seq->isDead()) {
            mSeqList.remove(i);
            delete seq;
        }
        i = next;
    }
}

bool JAISeqMgr::startSound(JAISoundID id, JAISoundHandle* handle, const JGeometry::TVec3< f32 >* posPtr) {
    if (handle != nullptr && handle->isSoundAttached()) {
        (*handle)->stop();
    }

    JAISoundInfo* soundInfo = JASGlobalInstance< JAISoundInfo >::getInstance();
    int category = (soundInfo != nullptr) ? soundInfo->getCategory(id) : -1;
    JAISeqData seqData(nullptr, 0);

    if (seqDataMgr->getSeqData(id, &seqData) == JAISeqDataMgr::SeqDataReturnValue_0) {
        return false;
    }

    JAISeq* seq = beginStartSeq_();
    if (seq != nullptr) {
        seq->JAISeqMgr_startID_(id, posPtr, mAudience, category, mNumTracks);
        if (endStartSeq_(seq, handle)) {
            if (soundInfo != nullptr) {
                soundInfo->getSeqInfo(id, seq);
            }
            return true;
        }
    }

    return false;
}

void JAISeqMgr::calc() {
    mMove.calc();

    JSULink< JAISeq >* i;
    for (i = mSeqList.getFirst(); i != mSeqList.getEnd(); i = i->getNext()) {
        i->getObject()->JAISeqMgr_calc_();
    }
    freeDeadSeq_();
}

void JAISeqMgr::stop() {
    JSULink< JAISeq >* i;
    for (i = mSeqList.getFirst(); i != mSeqList.getEnd(); i = i->getNext()) {
        i->getObject()->stop();
    }
}

void JAISeqMgr::stop(u32 fadeTime) {
    JSULink< JAISeq >* i;
    for (i = mSeqList.getFirst(); i != mSeqList.getEnd(); i = i->getNext()) {
        i->getObject()->stop(fadeTime);
    }
}

void JAISeqMgr::stopSoundID(JAISoundID id) {
    if (!id.isAnonymous()) {
        JSULink< JAISeq >* i;
        for (i = mSeqList.getFirst(); i != mSeqList.getEnd(); i = i->getNext()) {
            if ((u32)i->getObject()->getID() == (u32)id) {
                i->getObject()->stop();
            }
        }
    }
}

void JAISeqMgr::mixOut() {
    JSULink< JAISeq >* i;
    for (i = mSeqList.getFirst(); i != mSeqList.getEnd(); i = i->getNext()) {
        i->getObject()->JAISeqMgr_mixOut_(mMove.mParams, mSoundActivity);
    }
}

JAISeq* JAISeqMgr::beginStartSeq_() {
    JAISeq* seq = new JAISeq(this, soundStrategyMgr);
    return seq;
}

bool JAISeqMgr::endStartSeq_(JAISeq* seq, JAISoundHandle* handle) {
    JAISeq* sound = seq->getObject();
    if (sound != nullptr) {
        if (sound->isAlive()) {
            mSeqList.append(seq);
            if (handle != nullptr) {
                sound->attachHandle(handle);
            }
            return true;
        }
        delete sound;
    }
    return false;
}
