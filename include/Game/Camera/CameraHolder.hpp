#pragma once

#include "Game/NameObj/NameObj.hpp"

class Camera;
class CamTranslatorBase;

class CameraHolder : public NameObj {
public:
    CameraHolder(const char*);

    s32 getNum() const;
    CamTranslatorBase* getTranslator(s32);
    s32 getIndexOf(const char*) const;
    Camera* getDefaultCamera();
    s32 getIndexOfDefault() const;
    s32 getIndexOf(Camera*) const;
    void createCameras();
    Camera* getCameraInner(s32) const;

    /* 0x0C */ s32 mDefaultCameraIndex;
    /* 0x10 */ Camera** mCameras;
    /* 0x14 */ CamTranslatorBase** mTranslators;
    /* 0x18 */ Camera* mDefaultCamera;
};
