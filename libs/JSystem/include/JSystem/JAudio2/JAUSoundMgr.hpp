#pragma once

#include "JSystem/JAudio2/JAISeMgr.hpp"
#include "JSystem/JAudio2/JAISeqMgr.hpp"
#include "JSystem/JAudio2/JAISoundStarter.hpp"
#include "JSystem/JAudio2/JAIStreamMgr.hpp"

class JAIAudience;
class JAISoundID;
class JAISoundHandle;

class JAUSoundMgr : public JAISoundStarter {
public:
    JAUSoundMgr();

    virtual bool startSound(JAISoundID id, JAISoundHandle* handlePtr, const TVec3f* posPtr);

    void setAudience(JAIAudience*);
    void calc();
    void mixOut();
    void stopSoundID(JAISoundID);
    void stopSync();
    void stop(u32 fadeTime);
    void initParams();

    int getNumActiveSounds() const;

    /* 0x004 */ JAISeMgr mSeMgr;
    /* 0x72C */ JAISeqMgr mSeqMgr;
    /* 0x7A4 */ JAIStreamMgr mStreamMgr;
};
