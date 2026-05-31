#pragma once

#include <revolution.h>

class AudSeKeeper;
class AudSoundObject;
class JAISoundHandle;
class JAISoundID;
class LiveActor;

namespace MR {
    enum BossBgmID {
        BossBgmID_01_A,
        BossBgmID_01_B,
        BossBgmID_02_A,
        BossBgmID_02_B,
        BossBgmID_03_A,
        BossBgmID_03_B,
        BossBgmID_05_A,
        BossBgmID_05_B,
        BossBgmID_Begoman = BossBgmID_01_A,
        BossBgmID_StinkBugA = BossBgmID_01_A,
        BossBgmID_StinkBugB = BossBgmID_01_B,
        BossBgmID_DinoPackunA = BossBgmID_03_A,
        BossBgmID_DinoPackunB = BossBgmID_03_B,
        BossBgmID_DodoryuA = BossBgmID_05_A,
        BossBgmID_DodoryuB = BossBgmID_05_B,
        BossBgmID_OtaKing = BossBgmID_01_A,
        BossBgmID_TombSpiderA = BossBgmID_03_A,
        BossBgmID_TombSpiderB = BossBgmID_03_B,
        BossBgmID_TripodBossA = BossBgmID_02_A,
        BossBgmID_TripodBossB = BossBgmID_02_B,
    };

    enum EventBgmID {
        EventBgmID_Hurry,
        EventBgmID_Chase,
        EventBgmID_Race_01,
        EventBgmID_Race_02,
        EventBgmID_PenguinRace = EventBgmID_Race_01,
        EventBgmID_TeresaRace = EventBgmID_Race_02,
        EventBgmID_Surfing = EventBgmID_Race_01,
    };

    JAISoundHandle* startSystemSE(const char*, s32, s32);
    JAISoundHandle* startSystemSE(JAISoundID, s32, s32);
    JAISoundHandle* startSystemLevelSE(const char*, s32, s32);
    JAISoundHandle* startSystemLevelSE(JAISoundID, s32, s32);
    void stopSystemSE(const char*, u32);
    void stopSystemSE(JAISoundID, u32);
    JAISoundHandle* startAtmosphereSE(const char*, s32, s32);
    JAISoundHandle* startAtmosphereLevelSE(const char*, s32, s32);
    JAISoundHandle* startSoundObject(AudSoundObject*, const char*);
    JAISoundHandle* startSoundObjectLevel(AudSoundObject*, const char*, s32);
    JAISoundHandle* startSoundObjectLevel(AudSoundObject*, JAISoundID, s32);
    JAISoundHandle* startSoundObjectLevelParam(AudSoundObject*, const char*, s32, s32, s32);
    JAISoundHandle* startSoundObjectLevelParam(AudSoundObject*, JAISoundID, s32, s32, s32);
    JAISoundHandle* startSound(const LiveActor*, const char*, s32, s32);
    JAISoundHandle* startSound(const LiveActor*, JAISoundID, s32, s32);
    JAISoundHandle* startSoundSeVer(const LiveActor*, const char*, s32, s32);
    JAISoundHandle* startLevelSound(const LiveActor*, const char*, s32, s32, s32);
    JAISoundHandle* startLevelSound(const LiveActor*, JAISoundID, s32, s32, s32);
    JAISoundHandle* startLevelSoundSeVer(const LiveActor*, const char*, s32, s32, s32);
    void stopSound(const LiveActor*, const char*, u32);
    void stopSound(const LiveActor*, JAISoundID, u32);
    void setSeVersion(const LiveActor*, u32);
    void releaseSoundHandle(const LiveActor*, const char*);
    void startBas(const LiveActor*, const char*, bool, f32, f32);
    void actorSoundMovement(LiveActor*);
    void addJointToSeKeeper(AudSeKeeper*, const char*, const char*);
    void startLevelSoundSeKeeper(AudSeKeeper*, const char*, const char*);
    bool hasME();
    void startSystemME(const char*);
    s32 getMapSoundCodeFoot(const LiveActor*);
    void setMapSondCodeGravity(const LiveActor*, s32);
    void startTalkSound(u8, const LiveActor*);
    void startRemixSound(s32, s32, f32);
    s32 getRemixMelodyNoteNum(s32);
    void limitedSound(const char*, s32);
    JAISoundHandle* startStageBGM(const char*, bool);
    JAISoundHandle* startStageBGMFromStageName(const char*, const char*, s32);
    void stopStageBGM(u32);
    void unlockStageBGM();
    void setNextStageBGM(const char*);
    void moveVolumeStageBGM(f32, u32);
    void moveVolumeStageBGMForNoteFairy(f32, u32);
    JAISoundHandle* startLastStageBGM();
    bool isPlayingStageBgm();
    bool isPlayingStageBgmID(u32);
    bool isPlayingStageBgmName(const char*);
    bool isStopOrFadeoutStageBgmID(u32);
    bool isStopOrFadeoutBgmName(const char*);
    bool isPreparedStageBgm();
    void setStageBGMState(s32, u32);
    void setStageBGMStateBit(u32);
    JAISoundHandle* startSubBGM(const char*, bool);
    void stopSubBGM(u32);
    void moveVolumeSubBGM(f32, u32);
    bool isPlayingSubBgmID(u32);
    void playLevelMarioPinchBGM(bool);
    void stopMarioPinchBGMSoon();
    void submitTrigSE();
    void permitTrigSE();
    void submitLevelSE();
    void permitLevelSE();
    void submitSE();
    void permitSE();
    bool isPermitSE();
    void setCubeBgmChangeInvalid();
    bool isCubeBgmChangeInvalid();
    void clearBgmQueue();
    void setMicMtx(MtxPtr, s32);
    void setSoundVolumeSetting(s32, u32);
    void recoverSoundVolumeSetting(u32);
    void setAudioEffectType(s32, s32);
    void startCSSound(const char*, const char*, s32);
    void startCSSound2P(const char*, const char*);
    void startSpinHitSound(const LiveActor*);
    void startBlowHitSound(const LiveActor*);
    void startDPDHitSound();
    void startDPDFreezeLevelSound(const LiveActor*);
    void start2PJumpAssistSound();
    void start2PJumpAssistJustSound();
    void start2PAttackAssistSound();
    void limitedStarPieceHitSound();
    void startCurrentStageBGM();
    JAISoundHandle* startBossBGM(s32);
    JAISoundHandle* startEventBGM(s32);
    JAISoundHandle* startAfterBossBGM();
};  // namespace MR
