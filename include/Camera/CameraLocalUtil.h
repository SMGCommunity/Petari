#pragma once

#include "Camera/CameraMan.h"

class CameraLocalUtil
{
public:
    static JGeometry::TVec3<f32>& getWatchPos(const CameraMan *);
    static JGeometry::TVec3<f32>& getPos(const CameraMan *);
    static JGeometry::TVec3<f32>& getUpVec(const CameraMan *);
    static JGeometry::TVec3<f32>& getWatchUpVec(const CameraMan *);
    static f32 getFovy(const CameraMan *);
    static JGeometry::TVec3<f32>& getGlobalOffset(const CameraMan *);
    static JGeometry::TVec3<f32>& getLocalOffset(const CameraMan *);
    static f32 getRoll(const CameraMan *);

    void setWatchPos(CameraMan *, const JGeometry::TVec3<f32> &);
    void setPos(CameraMan *, const JGeometry::TVec3<f32> &);
    void setUpVec(CameraMan *, const JGeometry::TVec3<f32> &);
    void setWatchUpVec(CameraMan *, const JGeometry::TVec3<f32> &);
    void setFovy(CameraMan *, f32);
    void setGlobalOffset(CameraMan *, const JGeometry::TVec3<f32> &);
    void setLocalOffset(CameraMan *, const JGeometry::TVec3<f32> &);
    void setFrontOffset(CameraMan *, f32);
    void setUpperOffset(CameraMan *, f32);
    void setRoll(CameraMan *, f32);
};