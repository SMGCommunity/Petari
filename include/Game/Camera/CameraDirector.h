#pragma once

#include "Game/Camera/CameraParamChunkID.h"

class CameraParamChunkID_Tmp : public CameraParamChunkID {
public:
    inline CameraParamChunkID_Tmp() : CameraParamChunkID() {
        
    }
     
    virtual char *getBuffer(unsigned long);

    char mBuffer[0x100]; // _C
};