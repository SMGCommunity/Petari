#pragma once

#include <JSystem/JKernel/JKRArchive.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp> 

class AudSystem;

class AudSystemWrapper {
public:
    AudSystemWrapper(JKRSolidHeap *, JKRHeap *);

    void requestResourceForInitialize();
    void createAudioSystem();
    void createSoundNameConverter();
    void updateRhythm();
    void movement();
    void stopAllSound(u32);
    bool isLoadDoneWaveDataAtSystemInit() const;
    void loadStaticWaveData();
    bool isLoadDoneStaticWaveData() const;
    void loadStageWaveData(const char*, const char*, bool);
    bool isLoadDoneStageWaveData() const;
    void loadScenarioWaveData(const char*, const char*, s32);
    bool isLoadDoneScenarioWaveData() const;
    bool isPermitToReset() const;
    void prepareReset();
    void requestReset(bool);
    bool isResetDone();
    void resumeReset();
    void receiveResourceForInitialize();

    /* 0x00 */ AudSystem* _0;
    /* 0x04 */ JKRSolidHeap* _4;
    /* 0x08 */ JKRHeap* _8;
    /* 0x0C */ JKRArchive* mSMRArchive;
    /* 0x10 */ JKRArchive* mJaiSeqArchive;
    /* 0x14 */ JKRArchive* mJaiCoordArchive;
    /* 0x18 */ JKRArchive* mJaiMeArchive;
    /* 0x1C */ JKRArchive* mJaiRemixArchive;
    /* 0x20 */ JKRExpHeap* _20;
    /* 0x24 */ JKRArchive* mSpkResArchive;
    /* 0x28 */ bool _28;
    /* 0x29 */ bool _29;
    /* 0x2A */ bool _2A;
};
