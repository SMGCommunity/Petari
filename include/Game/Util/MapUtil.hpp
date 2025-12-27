#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class CollisionPartsFilterBase;
class HitInfo;
class HitSensor;
class JMapInfoIter;
class LiveActor;
class Triangle;
class TriangleFilterBase;

namespace MR {
    const TVec3f* getNormal(const Triangle*);
    bool isWallPolygon(const TVec3f&, const TVec3f&);
    bool isFloorPolygon(const TVec3f&, const TVec3f&);
    bool isFloorPolygonCos(const TVec3f&, const TVec3f&, f32);
    bool isWallPolygon(f32);
    bool isFloorPolygon(f32);
    bool isCeilingPolygon(f32);
    bool isWaterPolygon(const Triangle*);
    bool isThroughPolygon(const Triangle*);
    bool getFirstPolyOnLineToMap(TVec3f*, Triangle*, const TVec3f&, const TVec3f&);
    bool getFirstPolyOnLineToMapAndMoveLimit(TVec3f*, Triangle*, const TVec3f&, const TVec3f&);
    bool getFirstPolyOnLineToWaterSurface(TVec3f*, Triangle*, const TVec3f&, const TVec3f&);
    bool getFirstPolyOnLineToMapExceptSensor(TVec3f*, Triangle*, const TVec3f&, const TVec3f&, const HitSensor*);
    bool getFirstPolyOnLineToMapExceptActor(TVec3f*, Triangle*, const TVec3f&, const TVec3f&, const LiveActor*);
    bool getFirstPolyOnLineToMap(TVec3f*, Triangle*, const TVec3f&, const TVec3f&, const CollisionPartsFilterBase*, const TriangleFilterBase*);
    bool getFirstPolyOnLineToWaterSurface(TVec3f*, Triangle*, const TVec3f&, const TVec3f&, const CollisionPartsFilterBase*,
                                          const TriangleFilterBase*);
    bool getFirstPolyNormalOnLineToMap(TVec3f*, const TVec3f&, const TVec3f&, TVec3f*, const HitSensor*);
    u32 getNearPolyOnLineSort(const TVec3f&, const TVec3f&, const TVec3f&, const HitSensor*);
    bool getSortedPoly(TVec3f*, Triangle*, u32);
    const Triangle* getSortedPoly(u32);
    bool isExistMapCollision(const TVec3f&, const TVec3f&);
    bool isExistMoveLimitCollision(const TVec3f&, const TVec3f&);
    bool isExistMapCollisionExceptActor(const TVec3f&, const TVec3f&, const LiveActor*);
    bool checkStrikePointToMap(const TVec3f&, HitInfo*);
    bool checkStrikeBallToMap(const TVec3f&, f32);
    bool calcMapGround(const TVec3f&, TVec3f*, f32);
    bool calcMapGroundUpper(TVec3f*, const LiveActor*);
    bool isFallNextMove(const LiveActor*, f32, f32, f32, const TriangleFilterBase*);
    bool isFallNextMove(const TVec3f&, const TVec3f&, const TVec3f&, f32, f32, f32, const TriangleFilterBase*);
    bool isFallOrDangerNextMove(const LiveActor*, f32, f32, f32);
    bool isFallOrDangerNextMove(const TVec3f&, const TVec3f&, const TVec3f&, f32, f32, f32);
    void calcVelocityMovingPoint(const Triangle*, const TVec3f&, TVec3f*);
    u32 createAreaPolygonList(Triangle*, u32, const TVec3f&, const TVec3f&);
    u32 createAreaPolygonListArray(Triangle*, u32, TVec3f*, u32);
    bool trySetMoveLimitCollision(LiveActor*);
    bool isBindedGroundIce(const LiveActor*);
    bool isBindedGroundSand(const LiveActor*);
    bool isBindedGroundDamageFire(const LiveActor*);
    bool isBindedGroundWaterBottomH(const LiveActor*);
    bool isBindedGroundWaterBottomM(const LiveActor*);
    bool isBindedGroundWater(const LiveActor*);
    bool isBindedGroundSinkDeath(const LiveActor*);
    bool isBindedGroundAreaMove(const LiveActor*);
    bool isBindedGroundRailMove(const LiveActor*);
    bool isBindedGroundBrake(const LiveActor*);
    bool isBindedDamageFire(const LiveActor*);
    bool isBindedDamageElectric(const LiveActor*);
    u32 getCameraID(const Triangle*);
    const char* getFloorCodeString(const Triangle*);
    const char* getWallCodeString(const Triangle*);
    const char* getSoundCodeString(const Triangle*);
    s32 getFloorCodeIndex(const JMapInfoIter&);
    s32 getSoundCodeIndex(const JMapInfoIter&);
    s32 getFloorCodeIndex(const Triangle*);
    s32 getWallCodeIndex(const Triangle*);
    bool isGroundCodeWaterIter(const JMapInfoIter&);
    bool isGroundCodeDeath(const Triangle*);
    bool isGroundCodeDamage(const Triangle*);
    bool isGroundCodeIce(const Triangle*);
    bool isGroundCodeDamageFire(const Triangle*);
    bool isGroundCodeFireDance(const Triangle*);
    bool isGroundCodeSand(const Triangle*);
    bool isGroundCodeDamageElectric(const Triangle*);
    bool isGroundCodeWaterBottomH(const Triangle*);
    bool isGroundCodeWaterBottomM(const Triangle*);
    bool isGroundCodeSinkDeath(const Triangle*);
    bool isGroundCodeRailMove(const Triangle*);
    bool isGroundCodeAreaMove(const Triangle*);
    bool isGroundCodeNoStampSand(const Triangle*);
    bool isGroundCodeSinkDeathMud(const Triangle*);
    bool isGroundCodeBrake(const Triangle*);
    bool isWallCodeGhostThrough(const Triangle*);
    bool isWallCodeRebound(const Triangle*);
    bool isWallCodeNoAction(const Triangle*);
    bool isCameraCodeThrough(const Triangle*);
    bool isCodeSand(const Triangle*);
    const Triangle* getCameraPolyFast(const TVec3f&, const TVec3f&, const HitSensor*);
    bool getFirstPolyOnLineBFast(const TVec3f&, const TVec3f&, TVec3f*, Triangle*);
};  // namespace MR

namespace Collision {
    s32 checkStrikePointToMap(const TVec3f&, HitInfo*);
    s32 checkStrikeBallToMap(const TVec3f&, f32, const CollisionPartsFilterBase*, const TriangleFilterBase*);
    s32 checkStrikeBallToMapWithMovingReaction(const TVec3f&, f32, const CollisionPartsFilterBase*, const TriangleFilterBase*);
    s32 checkStrikeBallToMapWithThickness(const TVec3f&, f32, f32, const CollisionPartsFilterBase*, const TriangleFilterBase*);
    s32 checkStrikeLineToMap(const TVec3f&, const TVec3f&, s32, const CollisionPartsFilterBase*, const TriangleFilterBase*);
    s32 checkStrikeLineToSunshade(const TVec3f&, const TVec3f&, s32, const CollisionPartsFilterBase*, const TriangleFilterBase*);
    const HitInfo* getStrikeInfoMap(u32);
    u32 getStrikeInfoNumMap();
};  // namespace Collision
