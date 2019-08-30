#ifndef BEZIERRAIL_H
#define BEZIERRAIL_H

#include "JMap/JMapInfo.h"
#include "JMap/JMapInfoIter.h"
#include "Map/Rail/RailPart.h"

class BezierRail
{
public:
    BezierRail(const JMapInfoIter &, const JMapInfo *);

    f32 normalizePos(f32, s32) const;
    f32 getRailPosCoord(s32) const;
    f32 getNearestRailPosCoord(JGeometry::TVec3<f32> const &);
    void calcPos(JGeometry::TVec3<f32> *, f32) const;
    void calcDirection(JGeometry::TVec3<f32> *, f32) const;
    void calcNearestPos(JGeometry::TVec3<f32> *, f32) const;
    f32 getTotalLength() const;
    s32 getPartLength(s32) const;

    bool mIsLoop; // _0
    u8 _1;
    u8 _2;
    u8 _3;
    s32 mPointNum; // _4
};

#endif // BEZIERRAIL_H