#pragma once

#include "JSystem/JGeometry/TVec.h"

class AreaObj;
class AreaObjMgr;
class AreaCubeBoundingBox;

namespace MR {
    AreaObjMgr* getAreaObjManager(const char *);
    AreaObj* getAreaObj(const char *, const TVec3f &);
    bool isInAreaObj(const char *, const TVec3f &);
    s32 getAreaObjArg(const AreaObj *, s32);

    void calcCylinderPos(TVec3f *, const AreaObj *);

    void calcCubePos(const AreaObj *, TVec3f *);
    void calcCubeLocalPos(TVec3f *, const AreaObj *, const TVec3f &);
    
    void calcCubeRotate(const AreaObj *, TVec3f *);

    AreaCubeBoundingBox *getCubeLocalBox(const AreaObj *);
};
