#pragma once

#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include <JSystem/JAudio2/JAUSoundObject.hpp>
#include <JSystem/JKernel/JKRDisposer.hpp>

class JAISoundID;
class JAISoundHandle;

enum SE_GROUP {
    SE_SYSTEM = 0x00,
    SE_PLAYER_VOICE = 0x01,
    SE_PLAYER_MOTION = 0x02,
    SE_BOSS_VOICE = 0x03,
    SE_BOSS_MOTION = 0x04,
    SE_OBJECT = 0x05,
    SE_ATMOSPHERE = 0x06,
    SE_DEMO = 0x07,
    SE_ENEMY_VOICE = 0x08,
    SE_ENEMY_MOTION = 0x09,
    SE_SUPPORTER_VOICE = 0x0A,
    SE_SUPPORTER_MOTION = 0x0B,
    SE_REMIX_SEQ = 0x0C,
    SE_HOME_BUTTON_MENU = 0x0D,
};

enum BGM_GROUP {
    MAIN_BGM = 0x00,
    MULTI_BGM = 0x01,
};

class AudSoundObjHashData {
public:
    AudSoundObjHashData();

    void init();

    /* 0x00 */ const char* mName;
    /* 0x04 */ u32 mHash;
    /* 0x08 */ JAISoundID mID;
};

class AudSoundObject : public JAUSoundObject, JKRDisposer {
public:
    AudSoundObject(TVec3f*, u8, JKRHeap*);

    virtual JAISoundHandle* startSound(JAISoundID);
    virtual JAISoundHandle* startLevelSound(JAISoundID);
    virtual ~AudSoundObject();

    void addToSoundObjHolder();
    void setTrans(TVec3f*);
    bool writePort(JAISoundHandle*, u32, u16);
    bool isLimitedSound(JAISoundID);
    JAISoundHandle* startLevelSound(const char*);
    JAISoundHandle* startSoundParam(JAISoundID, s32, s32);
    JAISoundHandle* startLevelSoundParam(JAISoundID, s32, s32);
    JAISoundHandle* startLevelSoundParam(const char*, s32, s32);
    bool modifyLimitedSound(JAISoundID);
    bool isPlayingID(JAISoundID) NO_INLINE;
    void releaseHandle(JAISoundID);
    void setMapCode(s32);
    void setMapCodeExtra(s32);
    s32 getMapCode() const;
    void clearMapCode();
    JAISoundID convertNameToLevelSEID(const char*);
    void limitVoiceOne(JAISoundID);
    void stopCategorySound(u32, u32);
    void setMapCodeToPort(JAISoundHandle*, JAISoundID);
    void setCutoffToPort(JAISoundHandle*, JAISoundID);
    JAISoundID convertSoundIdFromSeVersion(JAISoundID) const;
    void modifySe_Kawamura(JAISoundHandle*, s32);
    bool modifyLimitedSound_Kawamura(JAISoundID);
    void modifySe_Takezawa(JAISoundHandle*, s32, s32);
    bool modifyLimitedSound_Takezawa(JAISoundID);
    void modifySe_Gohara(JAISoundHandle*, s32, s32);

    bool isEnableStartSound(JAISoundID soundID) {
        return AudWrap::getSystem()->isEnableStartSound(soundID);
    }

    /* 0x30 */ u32 _30;  // so far: mIsIn2D, is this an enum or an int-like bool?
    /* 0x34 */ s32 mMapCode;
    /* 0x38 */ s32 mMapCodeExtra;
    /* 0x3C */ AudSoundObjHashData* mHashDatas;
    /* 0x40 */ s32 mNumHandles;
    /* 0x44 */ s32 mNumSounds;
};
