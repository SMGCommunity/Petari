#pragma once

#include <revolution.h>

namespace MR {

    void startCurrentStageBGM();
    void startBossBGM(s32);

    void stopStageBGM(u32);
    void unlockStageBGM();

    void stopSubBGM(u32);

    void startSound(const LiveActor *, const char *, s32, s32);

    void startSystemSE(const char *, s32, s32);

    void setAudioEffectType(s32, s32);

    bool isPlayingStageBgm();
    bool isPlayingStageBgmID(u32);
    bool isPlayingStageBgmName(const char *);

    void setStageBGMState(s32, u32);

    void setCubeBgmChangeInvalid();
    bool isCubeBgmChangeInvalid();
    void clearBgmQueue();
};
