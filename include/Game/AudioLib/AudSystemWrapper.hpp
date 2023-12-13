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