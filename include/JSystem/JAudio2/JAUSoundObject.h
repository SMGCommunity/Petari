#pragma once

#include "JSystem/JGeometry/TVec.h"
#include "Game/GameAudio/AudTalkSoundData.h"

class JAISoundHandle;

class JAUSoundObject {
public:
    JAISoundHandle *mHandles;   // _0
    u32 _4;

    JAUSoundObject();
    ~JAUSoundObject();

    virtual void process();
    virtual void dispose();
    virtual void stopOK(JAISoundHandle &);
    virtual void startSound(JAISoundID);
    virtual void startSoundIndex(JAISoundID, unsigned char);
    virtual void startLevelSound(JAISoundID);
    virtual void startLevelSoundIndex(JAISoundID, unsigned char);

    void stopSound(JAISoundID, unsigned long);
    void getLowPrioSound(JAISoundID);
    void getFreeHandleNotReserved();

    bool _C;
    u8 _D[3];
    u32 _10;
    TVec3f *_14;
};
