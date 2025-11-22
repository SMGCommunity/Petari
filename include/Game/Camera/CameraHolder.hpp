#pragma once

#include "Game/NameObj/NameObj.hpp"

class Camera;
class CamTranslatorBase;

class CameraHolder : public NameObj {
public:
    CameraHolder(const char*);
    virtual ~CameraHolder();

    CamTranslatorBase* getTranslator(s32);
    s32                getIndexOf(const char*) const;
    Camera*            getDefaultCamera();
    s32                getIndexOfDefault() const;
    s32                getIndexOf(Camera*) const;
    void               createCameras();
    Camera*            getCameraInner(s32) const;

    s32                 mDefaultCameraIndex; // 0xC
    Camera**            mCameras;            // 0x10
    CamTranslatorBase** mTranslators;        // 0x14
    CamTranslatorBase*  mDefaultTranslator;  // 0x18
};