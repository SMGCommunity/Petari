#pragma once

#include "Game/GameAudio/AudTalkSoundData.hpp"
#include "JSystem/JAudio2/JAISoundHandles.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class JAISoundHandle;

class JAUSoundObject : public JAISoundHandles {
public:
    JAUSoundObject();
    ~JAUSoundObject();

    virtual void process();
    virtual void dispose();
    virtual void stopOK(JAISoundHandle&);
    virtual JAISoundHandle* startSound(JAISoundID);
    virtual void startSoundIndex(JAISoundID, unsigned char);
    virtual JAISoundHandle* startLevelSound(JAISoundID);
    virtual void startLevelSoundIndex(JAISoundID, unsigned char);

    void stopSound(JAISoundID, unsigned long);
    void getLowPrioSound(JAISoundID);
    void getFreeHandleNotReserved();

    bool _C;
    u8 _D[3];
    u32 _10;
    TVec3f* _14;
};
