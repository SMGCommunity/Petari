#pragma once

#include "JSystem/JGeometry/TVec3.h"
#include "JSystem/JMap/JMapInfoIter.h"
#include "Map/Rail/BezierRail.h"

class RailRider
{
public:
    RailRider(const JMapInfoIter &);
    RailRider(s32, s32);

    void move();
    void moveToNearestPos(const JGeometry::TVec3f &);
    void moveToNearestPoint(const JGeometry::TVec3f &);
    void moveToNextPoint();
    void reverse();
    void calcPosAtCoord(JGeometry::TVec3f *, f32) const;
    void calcDirectionAtCoord(JGeometry::TVec3f *, f32) const;
    f32 calcNearestPos(const JGeometry::TVec3f &) const;
    f32 getTotalLength() const;
    s32 getPartLength(s32) const;
    bool isLoop() const;
    bool isReachedGoal() const;
    bool isReachedEdge() const;
    void setCoord(f32);
    void setSpeed(f32);
    bool getRailArgWithInit(const char *, s32 *) const;
    bool getRailArgNoInit(const char *, s32 *) const;
    f32 getNextPointCoord() const;
    f32 getCurrentPointCoord() const;
    s32 getPointNum() const;
    void copyPointPos(JGeometry::TVec3f *, s32) const;
    f32 getPointCoord(s32) const;
    void initBezierRail(const JMapInfoIter &, const JMapInfo *);
    bool getPointArgS32NoInit(const char *, s32 *, s32) const;
    bool getPointArgS32WithInit(const char *, s32 *, s32) const;
    bool getCurrentPointArgS32NoInit(const char *, s32 *) const;
    bool getCurrentPointArgS32WithInit(const char *, s32 *) const;
    bool getNextPointArgS32NoInit(const char *, s32 *) const;
    bool getNextPointArgS32WithInit(const char *, s32 *) const;
    s32 getNextPointNo() const;
    void syncPosDir();

    BezierRail* mBezierRail; // _0
    f32 mTotalLength; // _4
    f32 mSpeed; // _8
    u8 _C;
    u8 _D;
    u8 _E;
    u8 _F;
    JGeometry::TVec3f mCurrentPos; // _10
    JGeometry::TVec3f mCurrentDirection; // _1C
    JGeometry::TVec3f mStartPos; // _28
    JGeometry::TVec3f mEndPos; // _34
    s32 mCurPoint; // _40
};