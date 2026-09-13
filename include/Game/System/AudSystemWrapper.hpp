#pragma once

#include <revolution/types.h>

class AudSystem;
class JKRExpHeap;
class JKRHeap;
class JKRMemArchive;
class JKRSolidHeap;

class AudSystemWrapper {
public:
    AudSystemWrapper(JKRSolidHeap*, JKRHeap*);

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

    /* 0x00 */ AudSystem* mAudSystem;
    /* 0x04 */ JKRSolidHeap* mSolidHeap;
    /* 0x08 */ JKRHeap* mHeap;
    /* 0x0C */ void* mSmrRes;
    /* 0x10 */ JKRMemArchive* mJaiSeqRes;
    /* 0x14 */ JKRMemArchive* mJaiChordRes;
    /* 0x18 */ JKRMemArchive* mJaiMeRes;
    /* 0x1C */ JKRMemArchive* mJaiRemixSeqRes;
    /* 0x20 */ JKRExpHeap* mSpkHeap;
    /* 0x24 */ JKRMemArchive* mSpkRes;
    /* 0x28 */ bool _28;
    /* 0x29 */ bool mIsResetDone;
    /* 0x2A */ bool mDisableReset;
};
