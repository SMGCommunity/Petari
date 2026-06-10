#pragma once

#include "JSystem/JAudio2/JAISoundHandles.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class JAISoundHandle;
class JAISoundID;
class JKRHeap;

class JAUSoundObject : public JAISoundHandles {
public:
    JAUSoundObject();

    JAUSoundObject(TVec3f*, u8, JKRHeap*);
    ~JAUSoundObject();

    virtual void process();
    virtual void dispose();
    virtual bool stopOK(JAISoundHandle&);
    virtual JAISoundHandle* startSound(JAISoundID);
    virtual JAISoundHandle* startSoundIndex(JAISoundID, u8);
    virtual JAISoundHandle* startLevelSound(JAISoundID);
    virtual JAISoundHandle* startLevelSoundIndex(JAISoundID, u8);

    void stopSound(JAISoundID, u32);
    JAISoundHandle* getLowPrioSound(JAISoundID);
    JAISoundHandle* getFreeHandleNotReserved();

    /* 0x0C */ bool mIsAllocated;
    /* 0x10 */ u32 _10;
    /* 0x14 */ TVec3f* mPos;
};
