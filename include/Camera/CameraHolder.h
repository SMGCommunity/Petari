#pragma once

#include "Camera/Camera.h"
#include "Camera/CameraParallel.h"
#include "Actor/NameObj/NameObj.h"

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

    s8 getIndexOf(const char *) const;
    Camera* getDefaultCamera();
    s8 getIndexOfDefault() const;

    s8 mDefaultCameraIdx; // _C
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