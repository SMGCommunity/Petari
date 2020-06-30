#pragma once

#include "JMap/JMapInfo.h"
#include "JMap/JMapInfoIter.h"
#include "Map/Rail/RailPart.h"

class BezierRail
{
public:
    BezierRail(const JMapInfoIter &, const JMapInfo *);

    f32 normalizePos(f32, s32) const;
    f32 getRailPosCoord(s32) const;
    f32 getNearestRailPosCoord(const JGeometry::TVec3<f32> &) const;
    void calcPos(JGeometry::TVec3<f32> *, f32) const;
    void calcDirection(JGeometry::TVec3<f32> *, f32) const;
    void calcNearestPos(JGeometry::TVec3<f32> *, f32) const;
    f32 getTotalLength() const;
    f32 getPartLength(s32) const;
    void calcRailCtrlPointIter(JMapInfoIter *, s32) const;

    void calcPosDir(JGeometry::TVec3<f32> *, JGeometry::TVec3<f32> *, f32) const;
    void calcCurrentRailCtrlPointIter(JMapInfoIter *, f32, bool) const;
    static void calcRailDirection(JGeometry::TVec3<f32> *, const RailPart *, f32);
    void getIncludedSection(const RailPart **, f32 *, f32, s32) const;
    s32 getCurrentCtrlPointIndex(f32, bool) const;

    bool mIsLoop; // _0
    u8 _1;
    u8 _2;
    u8 _3;
    s32 mPointNum; // _4
    s32 mPointNumLoop; // _8
    RailPart* mRailParts; // _C
    f32* mSegLengths; // _10
    JMapInfoIter* mIter; // _14
    const JMapInfo* mMapInfo; // _18
};