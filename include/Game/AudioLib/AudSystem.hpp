#pragma once

#include "Game/AudioLib/AudBgmMgr.hpp"
#include <JSystem/JAudio2/JASAudioReseter.hpp>
#include <JSystem/JAudio2/JAUSoundMgr.hpp>
#include <JSystem/JGeometry/TVec.hpp>

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
class JKRArchive;
class JKRSolidHeap;
class SpkSystem;

class AudSystem : public JAUSoundMgr {
public:
    AudSystem(JAUSectionHeap*, JKRArchive*, JKRArchive*, JKRArchive*);

    AudChordInfo* getChordInfo();
    void setSpeakerResource(JKRArchive*);
    void frameWork();
    void calc();
    bool startSound(JAISoundID, JAISoundHandle*, const TVec3f*);
    bool startLevelSound(JAISoundID, JAISoundHandle*, const TVec3f*);
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
    s32 getPlayCountMin(JAISoundID);
    void setMicMtx(MtxPtr, s32);
    const TVec3f& getMicPos(s32);
    void setFarCamera(bool);
    void setSeVolumeSet(s32, u32);
    void recoverSeVolumeSet(u32);
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

    inline AudEffector* getAudEffector() const { return mAudEffector; }
    inline void setVar(u32 var) { _830 = var; }

    static AudSystem* msBasic;

    /* 0x0814 */ u8 _814;
    /* 0x0815 */ u8 _815;
    /* 0x0816 */ u8 _816;
    /* 0x0818 */ JASAudioReseter mReseter;
    /* 0x0828 */ u8 _828;
    /* 0x0829 */ u8 _829;
    /* 0x082A */ u8 _82A;
    /* 0x082B */ u8 _82B;
    /* 0x082C */ u8 _82C;
    /* 0x0830 */ s32 _830;
    /* 0x0834 */ u32 _834;
    /* 0x0838 */ u32 _838;
    /* 0x083C */ u32 _83C;
    /* 0x0840 */ u8 _840[0x1AC];
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
    /* 0x1314 */ u32 _1314;
    /* 0x1318 */ s32 _1318;
    /* 0x131C */ u32 _131C;
    /* 0x1320 */ s32 _1320;
    /* 0x1324 */ u8 _1324;
    /* 0x1328 */ u32 _1328;
    /* 0x132C */ SpkSystem* mSpkSystem;
};

AudSystem* AudNewAudSystem(JKRSolidHeap*, void*, JKRArchive*, JKRArchive*, JKRArchive*, JKRArchive*);
AudSystem* AudNewAudSystem_(JAUSectionHeap*, JKRArchive*, JKRArchive*, JKRArchive*, int);
