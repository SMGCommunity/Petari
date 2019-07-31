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

    bool isReachedGoal() const;
    bool isReachedEdge() const;
    void setCoord(f32);
    void setSpeed(f32);

    BezierRail* mBezierRail; // _0
    f32 _4;
    f32 mSpeed; // _8
    u32 _C;
    JGeometry::TVec3<f32> _10;
    JGeometry::TVec3<f32> _1C;
    JGeometry::TVec3<f32> _28;
    JGeometry::TVec3<f32> _34;
    s32 _40;
};

#endif // RAILRIDER_H