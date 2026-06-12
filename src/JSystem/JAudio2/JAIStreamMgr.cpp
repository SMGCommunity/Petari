#include "JSystem/JAudio2/JAIStreamMgr.hpp"
#include "JSystem/JAudio2/JAISoundHandles.hpp"
#include "JSystem/JAudio2/JAISoundInfo.hpp"
#include "JSystem/JAudio2/JAIStreamDataMgr.hpp"

JAIStreamMgr::JAIStreamMgr(bool setInstance) : JASGlobalInstance< JAIStreamMgr >(setInstance) {
    streamDataMgr_ = nullptr;
    mStreamAramMgr = nullptr;
    soundStrategyMgr = nullptr;
    mAudience = nullptr;
    mParams.init();
    mActivity.init();
}

bool JAIStreamMgr::startSound(JAISoundID id, JAISoundHandle* handle, const JGeometry::TVec3< f32 >* posPtr) {
    if (handle != nullptr && handle->isSoundAttached()) {
        (*handle)->stop();
    }

    s32 streamFileEntry = streamDataMgr_->getStreamFileEntry(id);
    if (streamFileEntry < 0) {
        return false;
    }

    JAIStream* stream = newStream_();
    JAISoundInfo* soundInfo = JASGlobalInstance< JAISoundInfo >::getInstance();

    int category = -1;
    if (soundInfo != nullptr) {
        category = soundInfo->getCategory(id);
    }

    if (stream == nullptr) {
        return false;
    }

    stream->JAIStreamMgr_startID_(id, streamFileEntry, posPtr, mAudience, category);
    if (soundInfo != nullptr) {
        soundInfo->getStreamInfo(id, stream);
    }

    if (handle != nullptr) {
        stream->attachHandle(handle);
    }

    return false;
}

void JAIStreamMgr::freeDeadStream_() {
    JSULink< JAIStream >* i = mStreamList.getFirst();
    while (i != nullptr) {
        JAIStream* stream = i->getObject();
        JSULink< JAIStream >* next = i->getNext();
        if (stream->isDead()) {
            mStreamList.remove(i);
            void* aramAddr = stream->JAIStreamMgr_getAramAddr_();
            if (aramAddr != nullptr) {
                bool result = mStreamAramMgr->deleteStreamAram((u32)aramAddr);
            }

            delete stream;
        }
        i = next;
    }
}

void JAIStreamMgr::calc() {
    JSULink< JAIStream >* i;
    mParams.calc();
    for (i = mStreamList.getFirst(); i != nullptr; i = i->getNext()) {
        i->getObject()->JAIStreamMgr_calc_();
    }
    freeDeadStream_();
}

void JAIStreamMgr::stop() {
    JSULink< JAIStream >* i;
    for (i = mStreamList.getFirst(); i != nullptr; i = i->getNext()) {
        i->getObject()->stop();
    }
}

void JAIStreamMgr::stop(u32 fadeTime) {
    JSULink< JAIStream >* i;
    for (i = mStreamList.getFirst(); i != nullptr; i = i->getNext()) {
        i->getObject()->stop(fadeTime);
    }
}

void JAIStreamMgr::stopSoundID(JAISoundID id) {
    JSULink< JAIStream >* i;
    for (i = mStreamList.getFirst(); i != nullptr; i = i->getNext()) {
        if ((u32)i->getObject()->getID() == (u32)id) {
            i->getObject()->stop();
        }
    }
}

void JAIStreamMgr::mixOut() {
    JSULink< JAIStream >* i;
    for (i = mStreamList.getFirst(); i != nullptr; i = i->getNext()) {
        i->getObject()->JAIStreamMgr_mixOut_(mParams.mParams, mActivity);
    }
}

JAIStream* JAIStreamMgr::newStream_() {
    if (mStreamAramMgr == nullptr) {
        return nullptr;
    }

    JAIStream* stream = new JAIStream(this, soundStrategyMgr);
    if (stream == nullptr) {
        return nullptr;
    }

    mStreamList.append(stream);
    return stream;
}
