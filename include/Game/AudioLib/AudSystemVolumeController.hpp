#pragma once

#include <revolution/types.h>

class AudSystem;

class AudSystemVolumeController {
public:
    enum VolumeSet {
        VolumeSet_0 = 0,
        VolumeSet_1 = 1,
        VolumeSet_2 = 2,
        VolumeSet_3 = 3,
        VolumeSet_ScreenSaver = 4,
        VolumeSet_HomeButtonMenu = 5,
        VolumeSet_PauseMenu = 6,
    };

    AudSystemVolumeController(AudSystem*);

    void init();
    void update();
    void setSeVolumeSetTrig(s32, u32);
    void recoverSeVolumeSet(u32);
    void setSeVolumeSetLevel(s32);
    void updateSeVolumeSetLevel();
    void setSeVolumeSetInner(s32, u32);
    void pushVolumeSet();
    void popVolumeSet();

    /* 0x00 */ AudSystem* mSystem;
    /* 0x04 */ s32 mVolumeSetDelay;
    /* 0x08 */ s32 mCurrVolumeSet;
    /* 0x0C */ s32 mVolumeSets[8];
    /* 0x2C */ s32 mNumVolumeSets;
};
