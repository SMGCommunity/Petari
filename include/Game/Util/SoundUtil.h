#pragma once

#include <revolution.h>

class AudSoundObject;

namespace MR {

    void startCurrentStageBGM();
    void startBossBGM(s32);

    void startStageBGMFromStageName(const char *, const char * , s32);

    void stopStageBGM(u32);
    void unlockStageBGM();

    void moveVolumeStageBGM(f32, u32);
    void moveVolumeStageBGMForNoteFairy(f32, u32);
    void moveVolumeSubBGM(f32, u32);
    void moreVolumeSubBGM(f32, u32);

    void stopSubBGM(u32);

    void startSound(const LiveActor *, const char *, s32, s32);

    void startSystemSE(const char *, s32, s32);
    void startSpinHitSound(const LiveActor *);
    void startBlowHitSound(const LiveActor *);
    void startSystemME(const char *);
    void startSound(const LiveActor *, const char *, s32, s32);
    void startSoundObject(AudSoundObject *, const char *);

    void startLevelSound(const LiveActor *, const char *, s32, s32, s32);    

    void setAudioEffectType(s32, s32);

    bool isPlayingStageBgm();
    bool isPlayingStageBgmID(u32);
    bool isPlayingStageBgmName(const char *);

    void setStageBGMState(s32, u32);

    void setCubeBgmChangeInvalid();
    bool isCubeBgmChangeInvalid();
    void clearBgmQueue();

    void startAfterBossBGM();

    void startCSSound(const char *, const char *, s32);

    bool hasME();

    void start2PAttackAssistSound();
    void start2PJumpAssistSound();

    void startDPDHitSound();
    void startDPDFreezeLevelSound(const LiveActor *);

    void moveVolumeStageBGMForNoteFairy(f32, u32);

    s32 getRemixMelodyNoteNum(s32);

    void startSystemLevelSE(const char *, s32, s32);
};
