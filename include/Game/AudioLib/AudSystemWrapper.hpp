#pragma once

#include <revolution.h>
#include <JSystem/JKernel/JKRArchive.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>

class AudSystem;

class AudSystemWrapper {
public:
    AudSystemWrapper(JKRSolidHeap *, JKRHeap *);

    void createAudioSystem();
    bool isResetDone();
    void resumeReset();
    void receiveResouceForInitialize();

    void requestResourceForInitialize();

    void updateAudioSystem();

    AudSystem* mSystem;                 // 0x0
    JKRSolidHeap* _4;
    JKRHeap* _8;
    void* mSMRData;                     // 0xC
    JKRArchive* mJaiSeqArchive;         // 0x10
    JKRArchive* mJaiCordArchive;        // 0x14
    JKRArchive* mJaiMeArchive;          // 0x18
    JKRArchive* mJaiRemixSeqArchive;    // 0x1C
    JKRExpHeap* _20;
    void* mSpeakerData;                 // 0x24
    u8 _28;
    u8 _29;
    u8 _2A;
};