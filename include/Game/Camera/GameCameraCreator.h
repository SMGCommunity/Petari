#pragma once

#include "revolution.h"

class CameraParamChunkHolder;

class GameCameraCreator {
public:
    GameCameraCreator(CameraParamChunkHolder *);

    void initCameraCodeCollection(const char *, long);
    void registerCameraCode(unsigned long);
    void termCameraCodeCollection();
    //void scanStartPos();
    //void scanArea();

    CameraParamChunkHolder *mChunkHolder;   // _0
    const char *_4;
    s32 _8;
    u8 mCodes[0x100];                       // _C
};