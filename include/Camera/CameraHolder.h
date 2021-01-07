#pragma once

#include "NameObj/NameObj.h"
#include "Camera/Camera.h"
#include "Camera/CameraParallel.h"

#include <revolution.h>

struct CameraEntry
{
    const char* mName;
    const char* mType;
    Camera* (*mCreationFunc)(void);
    s32 mUnk;
};

class CameraHolder : public NameObj
{
public:
    CameraHolder(const char *);

    CamTranslatorDummy* getTranslator(s32);
    s32 getIndexOf(const char *) const;
    Camera* getDefaultCamera();
    s32 getIndexOfDefault() const;
    s32 getIndexOf(Camera *) const;
    void createCameras();
    Camera* getCameraInner(s32) const;

    s32 mDefaultCameraIdx; // _C
    Camera** mCameras; // _10
    CamTranslatorDummy** mTranslators; // _14
    Camera* mDefaultCamera;
};

namespace
{
    template<typename T>
    Camera* createCamera();

    template<>
    Camera* createCamera<CameraParallel>()
    {
        return new CameraParallel("平行カメラ");
    }

    const CameraEntry sCameraTable[0x1] =
    {
        { "CAM_TYPE_XZ_PARA", "並行", createCamera<CameraParallel>, 0x1000000 }
    };
};