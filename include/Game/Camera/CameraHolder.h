#pragma once

#include "Game/Camera/Camera.h"
#include "Game/NameObj/NameObj.h"

class CameraHolder : public NameObj {
public:
    CameraHolder(const char *);
    virtual ~CameraHolder();

    s32 getIndexOf(const char *) const;
    Camera *getDefaultCamera();
    s32 getIndexOfDefault() const;
    s32 getIndexOf(Camera *) const;
    void createCameras();
    Camera *getCameraInner(long) const;

    s32 mDefaultCameraIndex;    // _C
    Camera **mCameras;          // _10
    void **_14;
    void *mDefaultFrom14;       // _18
};