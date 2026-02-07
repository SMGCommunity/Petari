#pragma once

#include <revolution.h>

class AudSoundObject;
class LiveActor;
class JAISoundID;

namespace MR {

    void startCurrentStageBGM();
    void startBossBGM(s32);

    void startStageBGM(const char*, bool);
    void startStageBGMFromStageName(const char*, const char*, s32);

    void startAtmosphereSE(const char*, s32, s32);
    void startAtmosphereLevelSE(const char*, s32, s32);

    void stopStageBGM(u32);
    void unlockStageBGM();

    void startLastStageBGM();

    void moveVolumeStageBGM(f32, u32);
    void moveVolumeStageBGMForNoteFairy(f32, u32);
    void moveVolumeSubBGM(f32, u32);
    void moreVolumeSubBGM(f32, u32);

    void stopSubBGM(u32);

    void startSound(const LiveActor*, const char*, s32, s32);
    bool startSound(const LiveActor*, JAISoundID, s32, s32);

    void actorSoundMovement(LiveActor*);

    void startCSSound(const char*, const char*, s32);
    void startCSSound2P(const char*, const char*);
    void startSystemSE(const char*, s32, s32);
    void startSystemSE(JAISoundID, s32, s32);
    void stopSystemSE(const char*, u32);
    void stopSystemSE(JAISoundID, u32);
    void startSpinHitSound(const LiveActor*);
    void startBlowHitSound(const LiveActor*);
    void startSystemME(const char*);
    void startSound(const LiveActor*, const char*, s32, s32);
    void startSoundObject(AudSoundObject*, const char*);

    void startLevelSound(const LiveActor*, const char*, s32, s32, s32);
    void startLevelSound(const LiveActor*, JAISoundID, s32, s32, s32);
    void releaseSoundHandle(const LiveActor*, const char*);

    void setAudioEffectType(s32, s32);

    bool isPlayingStageBgm();
    bool isPlayingStageBgmID(u32);
    bool isPlayingStageBgmName(const char*);
    bool isStopOrFadeoutStageBgmID(u32);
    bool isStopOrFadeoutBgmName(const char*);
    bool isPreparedStageBgm();

    void setStageBGMState(s32, u32);
    void setStageBGMStateBit(u32);

    void setCubeBgmChangeInvalid();
    bool isCubeBgmChangeInvalid();
    void clearBgmQueue();

    void startEventBGM(s32);
    void startAfterBossBGM();

    void startCSSound(const char*, const char*, s32);
    void startCSSound2P(const char*, const char*);

    bool hasME();

    void start2PAttackAssistSound();
    void start2PJumpAssistSound();
    void start2PJumpAssistJustSound();

    void startDPDHitSound();
    void startDPDFreezeLevelSound(const LiveActor*);

    void moveVolumeStageBGMForNoteFairy(f32, u32);

    s32 getRemixMelodyNoteNum(s32);

    void startSystemLevelSE(const char*, s32, s32);
    void startSystemLevelSE(JAISoundID, s32, s32);

    void submitSE();

    void playLevelMarioPinchBGM(bool);
    void stopMarioPinchBGMSoon();
    void submitTrigSE();
    void permitTrigSE();

    void permitSE();

    void setSoundVolumeSetting(s32, u32);

    void startSubBGM(const char*, bool);

    void stopSound(const LiveActor*, const char*, u32);
    void stopSound(const LiveActor*, JAISoundID, u32);
    void startSoundObjectLevel(AudSoundObject*, const char*, s32);

    void limitedStarPieceHitSound();

    void submitLevelSE();
    void permitLevelSE();

    void setSeVersion(const LiveActor*, u32);

    void startSoundSeVer(const LiveActor*, const char*, s32, s32);
    void startLevelSoundSeVer(const LiveActor*, const char*, s32, s32, s32);

    void setMapSondCodeGravity(const LiveActor*, s32);
};  // namespace MR
