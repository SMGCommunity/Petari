#ifndef RAILRIDER_H
#define RAILRIDER_H

#include "JGeometry/TVec3.h"
#include "JMap/JMapInfoIter.h"
#include "Map/Rail/BezierRail.h"

class RailRider
{
public:
    RailRider(const JMapInfoIter &);
    RailRider(s32, s32);

    void move();
    void moveToNearestPos(const JGeometry::TVec3<f32> &);
    void moveToNearestPoint(const JGeometry::TVec3<f32> &);
    void moveToNextPoint();
    void reverse();
    void calcPosAtCoord(JGeometry::TVec3<f32> *, f32) const;
    void calcDirectionAtCoord(JGeometry::TVec3<f32> *, f32) const;
    void calcNearestPos(const JGeometry::TVec3<f32> &) const;
    s32 getTotalLength() const;
    s32 getPartLength(s32) const;
    bool isLoop() const;
    bool isReachedGoal() const;
    bool isReachedEdge() const;
    void setCoord(f32);
    void setSpeed(f32);
    f32 getNextPointCoord() const;
    f32 getCurrentPointCoord() const;
    s32 getPointNum() const;

    void initBezierRail(const JMapInfoIter &, const JMapInfo *);
    void syncPosDir();
    s32 getNextPointNo() const;

    BezierRail* mBezierRail; // _0
    f32 mTotalLength; // _4
    f32 mSpeed; // _8
    u8 _C;
    u8 _D;
    u8 _E;
    u8 _F;
    JGeometry::TVec3<f32> _10;
    JGeometry::TVec3<f32> _1C;
    JGeometry::TVec3<f32> _28;
    JGeometry::TVec3<f32> _34;
    s32 _40;
};

#endif // RAILRIDER_H