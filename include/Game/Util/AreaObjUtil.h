#pragma once

#include "JSystem/JGeometry/TBox.h"
#include "JSystem/JGeometry/TVec.h"

class AreaObj;
class AreaObjContainer;
class AreaObjMgr;
class AreaCubeBoundingBox;
class WaterInfo;

namespace MR {
    AreaObjMgr* getAreaObjManager(const char *);
    AreaObj* getAreaObj(const char *, const TVec3f &);
    bool isInAreaObj(const char *, const TVec3f &);
    s32 getAreaObjArg(const AreaObj *, s32);

    void calcCylinderPos(TVec3f *, const AreaObj *);

    void calcCylinderUpVec(TVec3f *, const AreaObj *);
    f32 getCylinderRadius(const AreaObj *);

    void calcCubePos(const AreaObj *, TVec3f *);
    void calcCubeLocalPos(TVec3f *, const AreaObj *, const TVec3f &);
    
    void calcCubeRotate(const AreaObj *, TVec3f *);

    TBox3f *getCubeLocalBox(const AreaObj *);

    AreaObj* getCurrentAstroOverlookAreaObj();

    AreaObj* getAreaIn(const char *, const TVec3f &);

    AreaObj* getWaterAreaObj(WaterInfo *, const TVec3f &);
};
