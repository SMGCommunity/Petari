#pragma once

#include "JSystem/JGeometry/TVec.h"

class CameraMan;
class CameraTargetObj;

namespace CameraLocalUtil {
    const char *getDummyVecRegName();

    TVec3f *getPos(const CameraMan *);
    TVec3f *getWatchPos(const CameraMan *);

    void setWatchPos(CameraMan *, const TVec3f &);

    void setUsedTarget(const CameraMan *, CameraTargetObj *);
}