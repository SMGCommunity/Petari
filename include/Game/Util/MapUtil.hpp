#pragma once

#include "JSystem/JGeometry.hpp"
#include "Game/Util.hpp"

class LiveActor;
class Triangle;
class TriangleFilterBase;

namespace MR {
    bool isBindedGroundDamageFire(const LiveActor *);
    bool isBindedGroundSand(const LiveActor *);

    bool getFirstPolyOnLineToMap(TVec3f *, Triangle *, const TVec3f &, const TVec3f &);
    bool getFirstPolyOnLineToMapExceptActor(TVec3f * , Triangle *, const TVec3f &, const TVec3f &, const LiveActor *);

    bool isWallCodeNoAction(const Triangle *);

    void setBindTriangleFilter(LiveActor *, TriangleFilterBase *);

    bool checkStikeBallToMap(const TVec3f &, f32);

    bool calcMapGround(const TVec3f &, TVec3f *, f32);

    bool trySetMoveLimitCollision(LiveActor *);
};
