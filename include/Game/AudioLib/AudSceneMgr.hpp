#pragma once

#include <revolution/types.h>

class JAUSectionHeap;

struct AudSceneSoundInfo {
    /* 0x00 */ const char* mSceneName;
    /* 0x04 */ const char* mStageName;
    /* 0x08 */ s8 mStageId;
    /* 0x09 */ s8 mScenarioId;
};

class AudSceneMgr {
public:
    enum PlayerMode {
        PlayerMode_Mario = 1,
        PlayerMode_Luigi = 2,
    };

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
    const AudSceneSoundInfo* findSceneSoundInfo(const char*, const char*);
    void eraseLastBgmWaveSet();
    void eraseLastSeWaveSet();
    void eraseLastSeScenarioWaveSet();
    void loadWaveSet(const s8*, s32);
    void setPlayerModeMario();
    void setPlayerModeLuigi();
    bool loadPlayerResource();
    bool isPlayerResourceLoaded();

    bool isPlayerModeMario() {
        return mPlayerMode == PlayerMode_Mario;
    }

    bool isPlayerModeLuigi() {
        return mPlayerMode == PlayerMode_Luigi;
    }

    /* 0x00 */ JAUSectionHeap* mSectionHeap;
    /* 0x04 */ u32 _4;
    /* 0x08 */ s32 mSeWaveSetId;
    /* 0x0C */ s32 mSeScenarioWaveSetId;
    /* 0x10 */ s32 mBgmWaveSetId;
    /* 0x14 */ s32 mPlayerMode;
    /* 0x18 */ s32 mPrevPlayerMode;
    /* 0x1C */ bool mIsNewPlayerMode;
    /* 0x1D */ bool _1D;
};
