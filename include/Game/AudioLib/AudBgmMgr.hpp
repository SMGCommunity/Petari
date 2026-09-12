#pragma once

#include "Game/AudioLib/AudBgmKeeper.hpp"
#include "Game/AudioLib/AudBgmVolumeController.hpp"

class JAISoundHandle;

class AudBgmMgr {
public:
    enum BgmType {
        BgmType_Stage = 0,
        BgmType_Sub = 1,
    };

    /// @brief Creates a new `AudBgmMgr`.
    AudBgmMgr();

    ~AudBgmMgr() {};
    void movement();
    JAISoundHandle* start(s32, u32, bool);
    void setNextBGM(s32, u32);
    void clearNextBGM(s32);
    JAISoundHandle* startLastBGM(s32);
    void clearLastBGM(s32);
    void pause();
    void unpause();
    void volDownLevel(bool);
    void volDownStageBgmWhenSubBgmPlaying();
    void startNextBgmWhenStopping(s32);
    void releaseStoppingBgm(s32);
    bool sendToRhythmSystem(s32);
    void setBgmToRhythmDominant(s32);
    void stopRhythmProcess(s32);

    /* 0x000 */ AudBgm* mBgm[2];
    /* 0x008 */ u32 mNextBGM[2];
    /* 0x010 */ u32 mCurrentBGM[2];
    /* 0x018 */ u32 mLastBGM[2];
    /* 0x020 */ AudBgmKeeper mKeeper;
    /* 0x7DC */ AudBgmVolumeController mVolumeController[2];
    /* 0x8FC */ bool _8FC;
};
