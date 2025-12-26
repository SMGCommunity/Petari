#pragma once

#include "Game/AudioLib/AudBgmKeeper.hpp"
#include "Game/AudioLib/AudBgmVolumeController.hpp"

class AudBgmMgr {
public:
    /// @brief Creates a new `AudBgmMgr`.
    AudBgmMgr();

    void movement();
    bool start(s32, u32, bool);
    void setNextBGM(s32, u32);
    void clearNextBGM(s32);
    bool startLastBGM(s32);
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

    /* 0x000 */ AudBgm* _0[2];
    /* 0x008 */ u32 mNextBGM[2];
    /* 0x010 */ u32 _10[2];
    /* 0x018 */ u32 mLastBGM[2];
    /* 0x020 */ AudBgmKeeper mKeeper;
    /* 0x7DC */ AudBgmVolumeController mVolumeController[2];
    /* 0x8FC */ u8 _8FC;
};
