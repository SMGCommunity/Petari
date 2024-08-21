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
    JKRArchive* mSMRArchive;            // 0xC
    JKRArchive* mJaiSeqArchive;         // 0x10
    JKRArchive* mJaiCoordArchive;       // 0x14
    JKRArchive* mJaiMeArchive;          // 0x18
    JKRArchive* mJaiRemixArchive;       // 0x1C
    JKRExpHeap* mHeap;                  // 0x20
    JKRArchive* mSpkResArchive;         // 0x24
    u8 _28;
    u8 _29;
    u8 _2A;
};