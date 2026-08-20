#pragma once

#include "revolution.h"

class CameraParamChunkHolder;

class GameCameraCreator {
public:
    GameCameraCreator(CameraParamChunkHolder*);

    void initCameraCodeCollection(const char*, s32);
    void registerCameraCode(u32);
    void termCameraCodeCollection();
    void scanStartPos();
    void scanArea();

    /* 0x0 */ CameraParamChunkHolder* mChunkHolder;
    /* 0x4 */ const char* mName;
    /* 0x8 */ s32 _8;
    /* 0xC */ u8 mCodes[0x100];
};
