#pragma once

#include "Game/Map/RailPart.hpp"
#include "Game/Util.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class BezierRailPart {
public:
    inline BezierRailPart() {}

    void set(const TVec3f&, const TVec3f&, const TVec3f&, const TVec3f&);
    void calcPos(TVec3f*, f32) const;
    void calcVelocity(TVec3f*, f32) const;

    f32 getLength(f32, f32, int) const;
    f32 getParam(f32) const;

    f32 getNearestParam(const TVec3f&, f32) const;

    inline f32 getVelocityLength(f32 t) const {
        TVec3f v;
        calcVelocity(&v, t);
        return v.length();
    }

    // 0th to 3rd degree control vectors for bezier control
    /* 0x00 */ TVec3f mStart;
    /* 0x0C */ TVec3f mCtrlDegree1;  // "velocity"
    /* 0x18 */ TVec3f mCtrlDegree2;  // "acceleration"
    /* 0x24 */ TVec3f mCtrlDegree3;  // "jerk"
    /* 0x30 */ f32 mLength;
};

class BezierRail {
public:
    BezierRail(const JMapInfoIter&, const JMapInfo*);

    f32 normalizePos(f32, int) const;
    f32 getPartLength(int) const;
    void calcPos(TVec3f*, f32) const;
    void calcDirection(TVec3f*, f32) const;
    void calcPosDir(TVec3f*, TVec3f*, f32) const;
    f32 getNearestRailPosCoord(const TVec3f&) const;
    f32 getRailPosCoord(int) const;
    void calcCurrentRailCtrlPointIter(JMapInfoIter*, f32, bool) const;
    void calcRailCtrlPointIter(JMapInfoIter*, int) const NO_INLINE;
    void getIncludedSection(const RailPart**, f32*, f32, int) const;
    int getCurrentCtrlPointIndex(f32, bool) const;

    f32 getTotalLength() const;

    inline RailPart* getRailPart(s32 idx) const { return &mRailParts[idx]; }

    /* 0x00 */ bool mIsClosed;
    /* 0x04 */ s32 mPointNum;
    /* 0x08 */ s32 mNumRailParts;
    /* 0x0C */ RailPart* mRailParts;  // this is a direct array of RailParts, not a single RailPart
    /* 0x10 */ f32* mPointCoords;
    /* 0x14 */ JMapInfoIter* mIter;
    /* 0x18 */ const JMapInfo* mInfo;
};
