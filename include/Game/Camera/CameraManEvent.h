#pragma once

#include "Game/Camera/CameraMan.h"

class CameraHolder;
class CameraParamChunkHolder;

class CameraManEvent : public CameraMan {
public:
    class ChunkFIFOItem {
    public:
        u32 _0;
        u32 _4;
        u32 _8;
        u32 _C;
        u32 _10;
        u8 _14[4];
    };

    CameraManEvent(CameraHolder *, CameraParamChunkHolder *, char *);

    CameraHolder *mHolder;                  // _48
    CameraParamChunkHolder* mChunkHolder;   // _4C
    ChunkFIFOItem mItems[4];                // _54
    u32 _B4;
    u32 _B8;
    u32 _BC;
};