#pragma once

#include "Game/Map/BezierRail.hpp"
#include "Game/Util.hpp"

class RailRider {
public:
    RailRider(const JMapInfoIter&);
    RailRider(s32, s32);

    void move();
    void moveToNearestPos(const TVec3f&);
    void moveToNearestPoint(const TVec3f&);
    void moveToNextPoint();
    void reverse();
    void calcPosAtCoord(TVec3f*, f32) const;
    void calcDirectionAtCoord(TVec3f*, f32) const;
    f32 calcNearestPos(const TVec3f&) const;
    f32 getTotalLength() const;
    f32 getPartLength(int) const;
    bool isLoop() const;
    bool isReachedGoal() const;
    bool isReachedEdge() const;
    void setCoord(f32);
    void setSpeed(f32);
    bool getRailArgWithInit(const char*, s32*) const;
    bool getRailArgNoInit(const char*, s32*) const;
    f32 getNextPointCoord() const;
    f32 getCurrentPointCoord() const;
    s32 getPointNum() const;
    void copyPointPos(TVec3f*, s32) const;
    f32 getPointCoord(s32) const;
    void initBezierRail(const JMapInfoIter&, const JMapInfo*);
    bool getPointArgS32NoInit(const char*, s32*, s32) const;
    bool getPointArgS32WithInit(const char*, s32*, s32) const;
    bool getCurrentPointArgS32NoInit(const char*, s32*) const;
    bool getCurrentPointArgS32WithInit(const char*, s32*) const;
    bool getNextPointArgS32NoInit(const char*, s32*) const;
    bool getNextPointArgS32WithInit(const char*, s32*) const;
    s32 getNextPointNo() const;
    void syncPosDir();

    inline TVec3f getStartPos() { return mStartPos; }

    inline TVec3f getEndPos() { return mEndPos; }

    BezierRail* mBezierRail;  // 0x0
    f32 mCoord;               // 0x4
    f32 mSpeed;               // 0x8
    bool mIsNotReverse;
    u8 _D;
    u8 _E;
    u8 _F;
    TVec3f mCurPos;        // 0x10
    TVec3f mCurDirection;  // 0x1C
    TVec3f mStartPos;      // 0x28
    TVec3f mEndPos;        // 0x34
    s32 mCurPoint;         // 0x40
};