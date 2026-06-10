#include "JSystem/JAudio2/JASAudioReseter.hpp"
#include "JSystem/JAudio2/JASAudioThread.hpp"
#include "JSystem/JAudio2/JASCriticalSection.hpp"
#include "JSystem/JAudio2/JASDSPChannel.hpp"
#include "JSystem/JAudio2/JASDriverIF.hpp"

JASAudioReseter::JASAudioReseter() {
    mTime = 0;
    mDoneFlag = true;
    mThreadStopFlag = false;
    mDSPLevel = JASDriver::getDSPLevel();
}

JASAudioReseter::~JASAudioReseter() {
}

bool JASAudioReseter::start(u32 time, bool threadStopFlag) {
    if (mDoneFlag == false) {
        return false;
    }
    mThreadStopFlag = threadStopFlag;
    JASCriticalSection critical;
    if (!JASDriver::registerDspSyncCallback(callback, this)) {
        return false;
    } else {
        mDSPLevel = JASDriver::getDSPLevel();
        mTime = time;
        mDoneFlag = false;
        return true;
    }
}

void JASAudioReseter::resume() {
    JASDriver::setDSPLevel(mDSPLevel);
}

s32 JASAudioReseter::checkDone() const {
    return mDoneFlag;
}

s32 JASAudioReseter::calc() {
    if (mTime == 0) {
        for (size_t i = 0; i < 64; i++) {
            JASDSPChannel* handle = JASDSPChannel::getHandle(i);
            if (handle->getStatus() == 0) {
                handle->drop();
            }
        }
        if (mThreadStopFlag != false) {
            JASAudioThread* pAudioThread = JASGlobalInstance< JASAudioThread >::getInstance();
            pAudioThread->stop();
        }
        mDoneFlag = 1;
        return -1;
    }

    mTime--;
    JASDriver::setDSPLevel(mTime * JASDriver::getDSPLevel() / (mTime + 1));
    return 0;
}

s32 JASAudioReseter::callback(void* reseter) {
    JASAudioReseter* i_this = (JASAudioReseter*)reseter;
    return i_this->calc();
}
