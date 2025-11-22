#pragma once

#include "JSystem/JAudio2/JAUSoundObject.hpp"
#include "JSystem/JKernel/JKRDisposer.hpp"

class JAISoundHandle;
class JKRHeap;

class AudSoundObject : public JAUSoundObject, JKRDisposer {
public:
    AudSoundObject(TVec3f*, u8, JKRHeap*);
    virtual ~AudSoundObject();

    virtual JAISoundHandle* startSound(JAISoundID);
    virtual JAISoundHandle* startLevelSound(JAISoundID);

    void addToSoundObjHolder();
    void setTrans(TVec3f*);
    void writePort(JAISoundHandle*, u32, u16);
    void isLimitedSound(JAISoundID);
    void startLevelSound(const char*);
    void startSoundParam(JAISoundID, s32, s32);
    void startLevelSoundParam(JAISoundID, s32, s32);
    void startLevelSoundParam(const char*, s32, s32);
    void modifyLimitedSound(JAISoundID);
    void isPlayingID(JAISoundID);
    void releaseHandle(JAISoundID);
    void setMapCode(s32);
    void setMapCodeExtra(s32);
    void getMapCode() const;
    void clearMapCode();
    void convertNameToLevelSEID(const char*);
    void limitVoiceOne(JAISoundID);
    void stopCategorySound(u32, u32);
    void setMapCodeToPort(JAISoundHandle*, JAISoundID);
    void setCutoffToPort(JAISoundHandle*, JAISoundID);
    void convertSoundIdFromSeVersion(JAISoundID) const;
    void modifySe_Kawamura(JAISoundHandle*, s32);
    void modifyLimitedSound_Kawamura(JAISoundID);
    void modifySe_Takezawa(JAISoundHandle*, s32, s32);
    void modifyLimitedSound_Takezawa(JAISoundID);
    void modifySe_Gohara(JAISoundHandle*, s32, s32);

    u32 _30;
    u32 _34;
    u32 _38;
    void* mHashDatas;  // 0x3C
    JKRHeap* mHeap;    // 0x40
    u32 _44;
};
