#pragma once

#include "Game/MapObj/SpiderThreadPoint.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class SpiderThreadPart;
class SpiderThreadRadialLine;
class SpiderThreadMainPoint;

class SpiderThreadPointNearInfo {
public:
    SpiderThreadPointNearInfo();

    void setInfo(f32, SpiderThreadPart*, const TVec3f&);
    void clear();
    const TVec3f* getPos() const;

    /* 0x00 */ SpiderThreadMainPoint* mMainPoint;
    /* 0x04 */ SpiderThreadPoint* mPartPoint;
    /* 0x08 */ f32 mDistToPoint;
    /* 0x0C */ f32 mSpringFactor;
    /* 0x10 */ SpiderThreadPart* mThreadPart;
};

class SpiderThreadMainPoint : public SpiderThreadPoint {
public:
    SpiderThreadMainPoint(const TVec3f&, int);

    void addNearMainPoint(SpiderThreadMainPoint*, SpiderThreadPart*);
    void addNearPartPoint(SpiderThreadPoint*, f32);
    void removeNearPoint(const SpiderThreadPoint*);
    void addNearPointToRadial(SpiderThreadPoint*);
    void removeNearPointFromRadial(const SpiderThreadPoint*);
    void cutNearPoints(s32*, SpiderThreadMainPoint**);
    void updateVelocity();
    void closeToNearPoint(const SpiderThreadPointNearInfo*);

    inline SpiderThreadPointNearInfo* getLastInfo() const { return &mNearInfos[mNumNearInfos - 1]; }

    /* 0x4C */ SpiderThreadRadialLine* mRadialLine;
    /* 0x50 */ s32 mNearInfoBufferSize;
    /* 0x54 */ s32 mNumNearInfos;
    /* 0x58 */ SpiderThreadPointNearInfo* mNearInfos;  // array
};
