#pragma once

#include <revolution.h>
#include <JSystem/JKernel/JKRArchive.h>
#include <JSystem/JKernel/JKRExpHeap.h>
#include <JSystem/JKernel/JKRHeap.h>
#include <JSystem/JKernel/JKRSolidHeap.h>

class AudSystem;

class AudSystemWrapper {
public:
    AudSystemWrapper(JKRSolidHeap *, JKRHeap *);

    void createAudioSystem();
    bool isResetDone();
    void resumeReset();
    void receiveResouceForInitialize();

    AudSystem* mSystem;                 // _0
    JKRSolidHeap* _4;
    JKRHeap* _8;
    void* mSMRData;                     // _C
    JKRArchive* mJaiSeqArchive;         // _10
    JKRArchive* mJaiCordArchive;        // _14
    JKRArchive* mJaiMeArchive;          // _18
    JKRArchive* mJaiRemixSeqArchive;    // _1C
    JKRExpHeap* _20;
    void* mSpeakerData;                 // _24
    u8 _28;
    u8 _29;
    u8 _2A;
};