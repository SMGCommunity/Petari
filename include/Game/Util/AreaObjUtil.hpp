#pragma once

#include "Game/Map/LightZoneDataHolder.hpp"
#include "JSystem/JGeometry/TBox.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class AreaObj;
class AreaObjContainer;
class AreaObjMgr;
class AreaCubeBoundingBox;
class WaterInfo;

namespace MR {
    AreaObjMgr* getAreaObjManager(const char*);
    AreaObj* getAreaObj(const char*, const TVec3f&);
    bool isInAreaObj(const char*, const TVec3f&);
    s32 getAreaObjArg(const AreaObj*, s32);

    bool calcAreaMoveVelocity(TVec3f*, const TVec3f&);

    void calcCylinderPos(TVec3f*, const AreaObj*);

    void calcCylinderUpVec(TVec3f*, const AreaObj*);
    f32 getCylinderRadius(const AreaObj*);

    void calcCubePos(const AreaObj*, TVec3f*);
    void calcCubeLocalPos(TVec3f*, const AreaObj*, const TVec3f&);

    void calcCubeRotate(const AreaObj*, TVec3f*);

    bool calcWhirlPoolAccelInfo(const TVec3f&, TVec3f*);

    TBox3f* getCubeLocalBox(const AreaObj*);

    AreaObj* getCurrentAstroOverlookAreaObj();

    AreaObj* getAreaIn(const char*, const TVec3f&);

    bool getWaterAreaObj(WaterInfo*, const TVec3f&);
    AreaInfo* getWaterAreaInfo(WaterInfo*, const TVec3f&, const TVec3f&, bool);

};  // namespace MR
