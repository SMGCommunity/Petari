#pragma once

#include "Camera/CameraMan.h"

class CameraLocalUtil
{
public:
    static JGeometry::TVec3f& getWatchPos(const CameraMan *);
    static JGeometry::TVec3f& getPos(const CameraMan *);
    static JGeometry::TVec3f& getUpVec(const CameraMan *);
    static JGeometry::TVec3f& getWatchUpVec(const CameraMan *);
    static f32 getFovy(const CameraMan *);
    static JGeometry::TVec3f& getGlobalOffset(const CameraMan *);
    static JGeometry::TVec3f& getLocalOffset(const CameraMan *);
    static f32 getRoll(const CameraMan *);

    void setWatchPos(CameraMan *, const JGeometry::TVec3f &);
    void setPos(CameraMan *, const JGeometry::TVec3f &);
    void setUpVec(CameraMan *, const JGeometry::TVec3f &);
    void setWatchUpVec(CameraMan *, const JGeometry::TVec3f &);
    void setFovy(CameraMan *, f32);
    void setGlobalOffset(CameraMan *, const JGeometry::TVec3f &);
    void setLocalOffset(CameraMan *, const JGeometry::TVec3f &);
    void setFrontOffset(CameraMan *, f32);
    void setUpperOffset(CameraMan *, f32);
    void setRoll(CameraMan *, f32);
};