#pragma once

#include <JSystem/JKernel/JKRArchive.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp> 

class AudSystemWrapper {
public:
    AudSystemWrapper(JKRSolidHeap *, JKRHeap *);

    void requestResourceForInitialize();
    void createAudioSystem();
    void createSoundNameConverter();
    void updateRhythm();
    void movement();

    u32 _0;
    JKRHeap* _4;
    u32 _8;
    JKRArchive* mSMRArchive;            // _C
    JKRArchive* mJaiSeqArchive;         // _10
    JKRArchive* mJaiCoordArchive;       // _14
    JKRArchive* mJaiMeArchive;          // _18
    JKRArchive* mJaiRemixArchive;       // _1C
    JKRExpHeap* mHeap;                  // _20
    JKRArchive* mSpkResArchive;         // _24
    u8 _28;
    u8 _29;
    u8 _2A;
};