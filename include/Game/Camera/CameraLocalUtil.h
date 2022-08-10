#pragma once

#include "JSystem/JGeometry/TVec.h"

class Camera;
class CameraDirector;
class CameraMan;
class CameraTargetObj;

namespace CameraLocalUtil {
    const char *getDummyVecRegName();

    CameraDirector *getCameraDirector();
    CameraTargetObj *getTarget(const CameraMan *);
    
    void calcSafePose(CameraMan *, Camera *);

    TVec3f *getPos(const Camera *);
    TVec3f *getWatchPos(const Camera *);
    TVec3f *getUpVec(const Camera *);
    TVec3f *getWatchUpVec(const Camera *);

    void setPos(Camera *, const TVec3f &);

    void setWatchPos(Camera *, const TVec3f &);

    void setUpVecAndWatchUpVec(Camera *, const TVec3f &);

    f32 getFovy(const Camera *);
    f32 getRoll(const Camera *);

    void setFovy(Camera *, f32);
    void setRoll(Camera *, f32);
    
    TVec3f *getGlobalOffset(const Camera *);
    TVec3f *getLocalOffset(const Camera *);

    void setGlobalOffset(Camera *, const TVec3f &);
    void setLocalOffset(Camera *, const TVec3f &);
    void setFrontOffset(Camera *, f32);
    void setUpperOffset(Camera *, f32);

    TVec3f *getPos(const CameraMan *);
    TVec3f *getWatchPos(const CameraMan *);

    TVec3f* getUpVec(const CameraMan *);

    f32 getFovy(const CameraMan *);

    void setPos(CameraMan *, const TVec3f &);
    void setWatchPos(CameraMan *, const TVec3f &);
    void setUpVec(CameraMan *, const TVec3f &);
    void setWatchUpVec(CameraMan *, const TVec3f &);
    void setUsedTarget(const CameraMan *, CameraTargetObj *);

    void setFovy(CameraMan *, f32);
    void setRoll(CameraMan *, f32);

    TVec3f *getLocalOffset(CameraMan *);

    void setGlobalOffset(CameraMan *, const TVec3f &);
    void setLocalOffset(CameraMan *, const TVec3f &);
    void setFrontOffset(CameraMan *, f32);
    void setUpperOffset(CameraMan *, f32);
}