#pragma once

#include "JSystem/JAudio2/JAUSoundObject.hpp"

class JAISoundHandle;
class JKRHeap;

class AudSoundObject : public JAUSoundObject {
public:
    AudSoundObject(TVec3f *, unsigned char, JKRHeap *);    
    virtual ~AudSoundObject();

    virtual void startSound(JAISoundID);
    virtual void startLevelSound(JAISoundID);

    void addToSoundObjHolder();
    void setTrans(TVec3f *);
    void writePort(JAISoundHandle *, unsigned long, unsigned short);
    void isLimitedSound(JAISoundID);
    void startLevelSound(const char *);
    void startSoundParam(JAISoundID, long, long);
    void startLevelSoundParam(JAISoundID, long, long);
    void startLevelSoundParam(const char *, long, long);
    void modifyLimitedSound(JAISoundID);
    void isPlayingID(JAISoundID);
    void releaseHandle(JAISoundID);
    void setMapCode(long);
    void setMapCodeExtra(long);
    void getMapCode() const;
    void clearMapCode();
    void convertNameToLevelSEID(const char *);
    void limitVoiceOne(JAISoundID);
    void stopCategorySound(unsigned long, unsigned long);
    void setMapCodeToPort(JAISoundHandle *, JAISoundID);
    void setCutoffToPort(JAISoundHandle *, JAISoundID);
    void convertSoundIdFromSeVersion(JAISoundID) const;
    void modifySe_Kawamura(JAISoundHandle *, long);
    void modifyLimitedSound_Kawamura(JAISoundID);
    void modifySe_Takezawa(JAISoundHandle *, long, long);
    void modifyLimitedSound_Takezawa(JAISoundID);
    void modifySe_Gohara(JAISoundHandle *, long, long);

    u8 _18[0x18];
    u32 _30;
    u32 _34;
    u32 _38;
    void *mHashDatas;   // 0x3C
    JKRHeap *mHeap;     // 0x40
    u32 _44;
};