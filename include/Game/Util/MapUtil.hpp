#pragma once

#include "Game/Util.hpp"
#include "JSystem/JGeometry.hpp"

class LiveActor;
class Triangle;
class TriangleFilterBase;
class CollisionPartsFilterBase;

namespace MR {
    bool isBindedGroundAreaMove(const LiveActor*);
    bool isBindedGroundRailMove(const LiveActor*);
    bool isBindedGroundDamageFire(const LiveActor*);
    bool isBindedGroundSand(const LiveActor*);

    bool isWallPolygon(f32);
    bool isFloorPolygon(f32);
    bool isCeilingPolygon(f32);

    bool getFirstPolyOnLineToMap(TVec3f*, Triangle*, const TVec3f&, const TVec3f&);
    bool getFirstPolyOnLineToMapExceptActor(TVec3f*, Triangle*, const TVec3f&, const TVec3f&, const LiveActor*);
    bool getFirstPolyNormalOnLineToMap(TVec3f*, const TVec3f&, const TVec3f&, TVec3f*, const HitSensor*);

    bool isWallCodeNoAction(const Triangle*);

    void setBindTriangleFilter(LiveActor*, TriangleFilterBase*);

    bool isExistMapCollision(const TVec3f&, const TVec3f&);
    bool isExistMapCollisionExceptActor(const TVec3f&, const TVec3f&, const LiveActor*);

    bool checkStikeBallToMap(const TVec3f&, f32);

    bool calcMapGround(const TVec3f&, TVec3f*, f32);
    bool calcMapGroundUpper(TVec3f*, const LiveActor*);

    bool trySetMoveLimitCollision(LiveActor*);

    void makeQuatRotateDegree(TQuat4f*, const TVec3f&);

    bool isFloorPolygonCos(const TVec3f&, const TVec3f&, f32);

    bool isFallNextMove(const LiveActor*, f32, f32, f32, const TriangleFilterBase*);

    s32 getSoundCodeIndex(const JMapInfoIter&);

    char* getWallCodeString(const Triangle*);
};  // namespace MR

class Collision {
public:
    static bool checkStrikeLineToMap(const TVec3f&, const TVec3f&, s32, const CollisionPartsFilterBase*, const TriangleFilterBase*);
    static bool checkStrikeLineToSunshade(const TVec3f&, const TVec3f&, s32, const CollisionPartsFilterBase*, const TriangleFilterBase*);
};
