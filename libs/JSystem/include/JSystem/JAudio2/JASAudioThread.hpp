#pragma once

#include "JSystem/JAudio2/JASGadget.hpp"
#include "JSystem/JAudio2/JASGlobal.hpp"
#include "JSystem/JKernel/JKRThread.hpp"

enum JASAudioMessage {
    AUDIOMSG_DMA = 0,
    AUDIOMSG_DSP = 1,
    AUDIOMSG_STOP = 2,
};

struct JASAudioThread : public JKRThread, public JASGlobalInstance< JASAudioThread > {
    JASAudioThread(int, int, u32) NO_INLINE;
    static void create(s32);
    void stop();
    virtual void* run();
    static void DMACallback();
    static void DSPCallback(void*);

    static JASAudioThread* getThreadPointer() {
        return getInstance();
    }
    static int getDSPSyncCount() {
        return snIntCount;
    }
    static void setDSPSyncCount(u32 intCount) {
        snIntCount = intCount;
    }

    /* 0x7C */ OSThreadQueue sThreadQueue;
    /* 0x84 */ bool sbPauseFlag;  // type unsure

    static volatile int snIntCount;  // type unsure
};
