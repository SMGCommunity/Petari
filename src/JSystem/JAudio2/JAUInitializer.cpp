#include "JSystem/JAudio2/JAUInitializer.hpp"

// NOTE: these must be included in this exact order for sinit to match
#include "JSystem/JAudio2/JAISe.hpp"
//
#include "JSystem/JAudio2/JAISeq.hpp"
//
#include "JSystem/JAudio2/JAIStream.hpp"
//
#include "JSystem/JAudio2/JAISoundChild.hpp"
//

#include "JSystem/JAudio2/JASAramStream.hpp"
#include "JSystem/JAudio2/JASAudioThread.hpp"
#include "JSystem/JAudio2/JASDriverIF.hpp"
#include "JSystem/JAudio2/JASDvdThread.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"
#include "JSystem/JAudio2/JASWaveArcLoader.hpp"
#include "JSystem/JKernel/JKRAram.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include "JSystem/JKernel/JKRThread.hpp"
#include <revolution/os.h>

JAU_JASInitializer::JAU_JASInitializer() {
    audioMemory_ = 0;
    audioMemSize_ = 0;
    dvdThreadId_ = -1;
    audioThreadId_ = -1;
    heapSize_ = 0x1000;
    dvdThreadPriority_ = 3;
    audioThreadPriority_ = 2;
    field_0x1c = 0x80;
    dspLevel_ = 1.0f;
    aramBlockSize_ = 0x2760;
    aramChannelNum_ = 2;
    field_0x20 = 0;
    waveArcDir_ = nullptr;
}

inline void JAU_JASInitializer::initJASystem(JKRSolidHeap* pHeap) {
    JASKernel::setupRootHeap(pHeap, heapSize_);

    if (audioMemory_ == 0) {
        audioMemory_ = JKRAram::getManager()->getAudioMemory();
    }

    if (audioMemSize_ == 0) {
        audioMemSize_ = JKRAram::getManager()->getAudioMemSize();
    }

    JASKernel::setupAramHeap(audioMemory_, audioMemSize_);

    JASTrack::newMemPool(field_0x1c);

    if (field_0x20 > 0) {
        JASTrack::TChannelMgr::newMemPool(field_0x20);
    }

    JASDvd::createThread(dvdThreadPriority_, 0x80, 0x1000);
    JASAudioThread::create(audioThreadPriority_);
    JKRThreadSwitch* pThreadSwitch = JKRThreadSwitch::getManager();
    if (pThreadSwitch != nullptr) {
        if (dvdThreadId_ >= 0) {
            pThreadSwitch->enter(JASDvd::getThreadPointer(), dvdThreadId_);
        }

        if (audioThreadId_ >= 0) {
            pThreadSwitch->enter(JASAudioThread::getThreadPointer(), audioThreadId_);
        }
    }

    JASDriver::setDSPLevel(dspLevel_);
    JASAramStream::initSystem(aramBlockSize_, aramChannelNum_);

    if (waveArcDir_ != nullptr) {
        JASWaveArcLoader::setCurrentDir(waveArcDir_);
    }

    switch (OSGetSoundMode()) {
    case 0:
        JASDriver::setOutputMode(0);
        break;
    case 1:
        JASDriver::setOutputMode(1);
        break;
    }
}

JAU_JAIInitializer::JAU_JAIInitializer() {
    mMaxNumSe = 100;
    mMaxNumSeq = 4;
    mMaxNumStream = 2;
    mMaxNumChildren = 16;
}

void JAU_JAIInitializer::initJAInterface() {
    s32 r30 = JASDram->getFreeSize();
    JAIStream::newMemPool(mMaxNumStream);
    JAISeq::newMemPool(mMaxNumSeq);
    JAISe::newMemPool(mMaxNumSe);
    JAISoundChild::newMemPool(mMaxNumChildren);
    s32 r29 = JASDram->getFreeSize();
}
