#pragma once

#include <revolution/types.h>

class AudSceneSoundInfo;
class JAUSectionHeap;

class AudSceneMgr {
public:
    AudSceneMgr(JAUSectionHeap*);

    bool isLoadDoneSystemInit();
    void loadStaticResource();
    bool isLoadDoneStaticResource();
    void loadStageResource(const char*, const char*);
    void loadStageResource(const AudSceneSoundInfo*);
    bool isLoadDoneStageResource();
    void loadScenarioResource(const char*, const char*, s32);
    void loadScenarioResource(const AudSceneSoundInfo*, s32);
    bool isLoadDoneScenarioResource();
    void startScene();
    void findSceneSoundInfo(const char*, const char*);
    void eraseLastBgmWaveSet();
    void eraseLastSeWaveSet();
    void eraseLastSeScenarioWaveSet();
    void loadWaveSet(const s8*, s32);
    void setPlayerModeMario();
    void setPlayerModeLuigi();
    void loadPlayerResource();
    bool isPlayerResourceLoaded();

    /* 0x00 */ JAUSectionHeap* _0;
    /* 0x04 */ u32 _4;
    /* 0x08 */ u32 _8;
    /* 0x0C */ s32 _C;
    /* 0x10 */ u32 _10;
    /* 0x14 */ u32 mPlayerMode;
    /* 0x18 */ u32 _18;
    /* 0x1C */ u8 _1C;
    /* 0x1D */ u8 _1D;
};
