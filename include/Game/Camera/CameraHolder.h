#pragma once

#include "Game/NameObj/NameObj.h"

class Camera;
class CamTranslatorBase;

class CameraHolder : public NameObj {
public:
    CameraHolder(const char *);
    virtual ~CameraHolder();

    CamTranslatorBase *getTranslator(long);
    s32 getIndexOf(const char *) const;
    Camera *getDefaultCamera();
    s32 getIndexOfDefault() const;
    s32 getIndexOf(Camera *) const;
    void createCameras();
    Camera *getCameraInner(long) const;

    s32 mDefaultCameraIndex;                // _C
    Camera **mCameras;                      // _10
    CamTranslatorBase **mTranslators;      // _14
    CamTranslatorBase *mDefaultTranslator; // _18
};