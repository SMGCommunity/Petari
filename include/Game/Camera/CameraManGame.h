#pragma once

#include "Game/Camera/CameraMan.h"

class CameraHolder;
class CameraParamChunkHolder;

class CameraManGame : public CameraMan {
public:
    CameraManGame(CameraHolder *, CameraParamChunkHolder *, char *);

    CameraHolder *mHolder;                  // _48
    CameraParamChunkHolder *mChunkHolder;   // _4C
    s32 mDefaultCameraIndex;                // _50
    u32 _54;
    u8 _58[4];
    u32 _5C;
    void *_60;
    void *_64;
    u32 _68;
    u8 _6C;
    u8 _6D[3];
    u32 _70;
    u8 _74;
    u8 _75[3];
};