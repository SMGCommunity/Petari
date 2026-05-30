#pragma once

#include "JSystem/JGeometry/TVec.hpp"

class JAISoundHandle;
class JAISoundID;

class JAISoundHandles {
public:
    JAISoundHandle* getHandleSoundID(JAISoundID);
    JAISoundHandle* getHandleUserData(u32);
    void setPos(const TVec3f&);

    /* 0x00 */ JAISoundHandle* mHandles;
    /* 0x04 */ u32 _4;
};
