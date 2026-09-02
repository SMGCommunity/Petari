#pragma once

#include "Game/AudioLib/AudAudience.hpp"
#include "Game/AudioLib/AudBgmMgr.hpp"
#include "Game/AudioLib/AudLimitedSound.hpp"
#include "JSystem/JAudio2/JAIAudible.hpp"
#include "JSystem/JAudio2/JAIAudience.hpp"
#include "JSystem/JAudio2/JAUStdSoundInfo.hpp"
#include <JSystem/JAudio2/JASAudioReseter.hpp>
#include <JSystem/JAudio2/JAUSoundMgr.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>

class AudChordInfo;
class AudEffector;
class AudMeObject;
class AudRemixMgr;
class AudRhythmMeSystem;
class AudSceneMgr;
class AudSeStrategyMgr;
class AudSoundObjHolder;
class AudSoundObject;
class AudSystemVolumeController;
class JAISoundHandle;
class JAISoundID;
class JAUSectionHeap;
class JAUStreamStaticAramMgr;
class JKRArchive;
class JKRSolidHeap;
class SpkSystem;

class AudSystem : public JAUSoundMgr {
public:
    enum MenuState {
        MenuState_Off = 0,
        MenuState_Enter = 1,
        MenuState_Exit = 2,
        MenuState_Active = 3,
    };

    enum MenuType {
        MenuType_HomeButtonMenu = 0,
        MenuType_Pause = 1,
        MenuType_Error = 2,
    };

    AudSystem(JAUSectionHeap*, JKRArchive*, JKRArchive*, JKRArchive*);

    virtual void frameWork();
    virtual void calc();
    virtual bool startSound(JAISoundID, JAISoundHandle*, const TVec3f*);
    virtual bool startLevelSound(JAISoundID, JAISoundHandle*, const TVec3f*);

    AudChordInfo* getChordInfo();
    void setSpeakerResource(JKRArchive*);
    void switchToLuigi(JAISoundID, JAISoundHandle*);
    bool isSubmitSeByVolumeSetting(JAISoundID);
    void updateOutputMode();
    void preProcessToReset();
    void resetAudio(u32, bool);
    void resumeReset();
    bool hasReset() const;
    void initSceneVolume();
    void pause();
    void unpause();
    void enterHomeButtonMenu();
    void exitHomeButtonMenu();
    void updateHomeButtonMenu();
    bool isHomeButtonMenuActive() const;
    void enterPauseMenu();
    void exitPauseMenu();
    void updatePauseMenu();
    bool isPauseMenuActive() const;
    void screenSaverProcess();
    bool isEnableStartSound(JAISoundID);
    void doDvdErrorProcess();
    void exitDvdErrorProcess();
    s32 getNumOfPlaying(JAISoundID);
    u32 getPlayCountMin(JAISoundID);
    void setMicMtx(MtxPtr, s32);
    const TVec3f& getMicPos(s32);
    void setFarCamera(bool);
    void setSeVolumeSet(s32, u32);
    void recoverSeVolumeSet(u32);
    void setSeVolumeSetLevel(s32);
    void initVolumeSetting();
    void initCategoryArrangement();
    void setMeResource(JKRArchive*, u16, u16);
    void seFanfareProcess();
    bool isFanfareSePlaying();
    void registerLimitedSound(JAISoundID, s32);
    void clearAllLimitedSound();
    bool isRegisteredLimitedSound(JAISoundID);
    void updateLimitedSound();
    bool isAlreadyPlayingSoundNear(JAISoundID, const TVec3f*, f32);
    void setVolumeZeroForce(s32);

    void set830(u32 var) {
        _830 = var;
    }

    AudEffector* getAudEffector() const {
        return mAudEffector;
    }

    static AudSystem* msBasic;

    /* 0x0814 */ bool mIsResetReady;
    /* 0x0815 */ bool mIsReset;
    /* 0x0816 */ bool mStopThreads;
    /* 0x0818 */ JASAudioReseter mReseter;
    /* 0x0828 */ bool _828;
    /* 0x0829 */ bool mIsPaused;
    /* 0x082A */ bool _82A;
    /* 0x082B */ bool _82B;  // blocks startSound
    /* 0x082C */ bool _82C;  // blocks startLevelSound
    /* 0x0830 */ s32 _830;
    /* 0x0834 */ JAUStdSoundInfo* mSoundInfo;
    /* 0x0838 */ JAUSectionHeap* mSectionHeap;
    /* 0x083C */ JAUStreamStaticAramMgr* mStreamAramMgr;
    /* 0x0840 */ AudAudience< 4 > mAudience;
    /* 0x09E8 */ AudDopplerAudible< 4 >* mAudible;  // TODO: should this be merged into AudAudience<>??
    /* 0x09EC */ AudSoundObject* mSystemSeObject;
    /* 0x09F0 */ AudSoundObject* mAtmosphereSeObject;
    /* 0x09F4 */ AudMeObject* mSystemMeObject;
    /* 0x09F8 */ AudSceneMgr* mSceneMgr;
    /* 0x09FC */ AudBgmMgr mBgmMgr;
    /* 0x12FC */ AudSoundObjHolder* mSoundObjHolder;
    /* 0x1300 */ AudRhythmMeSystem* mRhythmMeSystem;
    /* 0x1304 */ AudRemixMgr* mRemixMgr;
    /* 0x1308 */ AudEffector* mAudEffector;
    /* 0x130C */ AudSeStrategyMgr* mSeStrategyMgr;
    /* 0x1310 */ AudSystemVolumeController* mVolumeController;
    /* 0x1314 */ s32 mHomeButtonMenuState;
    /* 0x1318 */ s32 mHomeButtonToggleTime;
    /* 0x131C */ s32 mPauseMenuState;
    /* 0x1320 */ s32 mPauseMenuToggleTime;
    /* 0x1324 */ bool mIsDvdError;
    /* 0x1328 */ AudLimitedSoundInfo* mLimitedSoundInfo;
    /* 0x132C */ SpkSystem* mSpkSystem;
};

AudSystem* AudNewAudSystem(JKRSolidHeap*, void*, JKRArchive*, JKRArchive*, JKRArchive*, JKRArchive*) NO_INLINE;
AudSystem* AudNewAudSystem_(JAUSectionHeap*, JKRArchive*, JKRArchive*, JKRArchive*, int) NO_INLINE;
