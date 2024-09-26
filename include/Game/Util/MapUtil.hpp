#pragma once

#include "JSystem/JGeometry.hpp"
#include "Game/Util.hpp"

class LiveActor;
class Triangle;
class TriangleFilterBase;
class CollisionPartsFilterBase;

namespace MR {
    bool isBindedGroundDamageFire(const LiveActor *);
    bool isBindedGroundSand(const LiveActor *);

    bool getFirstPolyOnLineToMap(TVec3f *, Triangle *, const TVec3f &, const TVec3f &);
    bool getFirstPolyOnLineToMapExceptActor(TVec3f * , Triangle *, const TVec3f &, const TVec3f &, const LiveActor *);

    bool isWallCodeNoAction(const Triangle *);

    void setBindTriangleFilter(LiveActor *, TriangleFilterBase *);

    bool isExistMapCollisionExceptActor(const TVec3f &, const TVec3f &, const LiveActor *);

    bool checkStikeBallToMap(const TVec3f &, f32);

    bool calcMapGround(const TVec3f &, TVec3f *, f32);
    bool calcMapGroundUpper(TVec3f *, const LiveActor *);

    bool trySetMoveLimitCollision(LiveActor *);
};

class Collision {
public:
    static bool checkStrikeLineToMap(const TVec3f &, const TVec3f &, s32, const CollisionPartsFilterBase *, const TriangleFilterBase *);
};