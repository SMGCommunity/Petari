#pragma once

#include "JSystem/JAudio2/JAUSoundObject.hpp"
#include "JSystem/JKernel/JKRDisposer.hpp"

class AudSoundObject : public JAUSoundObject, JKRDisposer {
public:
    AudSoundObject(TVec3f*, u8, JKRHeap*);

    virtual JAISoundHandle* startSound(JAISoundID);
    virtual JAISoundHandle* startLevelSound(JAISoundID);

    void addToSoundObjHolder();
    void setTrans(TVec3f*);
    bool writePort(JAISoundHandle*, u32, u16);
    bool isLimitedSound(JAISoundID);
    JAISoundHandle* startLevelSound(const char*);
    JAISoundHandle* startSoundParam(JAISoundID, s32, s32);
    JAISoundHandle* startLevelSoundParam(JAISoundID, s32, s32);
    JAISoundHandle* startLevelSoundParam(const char*, s32, s32);
    bool modifyLimitedSound(JAISoundID);
    bool isPlayingID(JAISoundID);
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

    u32 _30;
    u32 _34;
    u32 _38;
    void* mHashDatas;  // 0x3C
    JKRHeap* mHeap;    // 0x40
    u32 _44;
};
