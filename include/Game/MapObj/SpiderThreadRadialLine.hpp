#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class SpiderThreadMainPoint;

class SpiderThreadRadialLine {
public:
    SpiderThreadRadialLine(int);

    void addPoint(SpiderThreadMainPoint*);
    void updateVelocity();
    void updatePos();

    SpiderThreadMainPoint* getPoint(int idx) const {
        return mPoints[idx];
    }

    /* 0x00 */ int mBufferSize;
    /* 0x04 */ s32 mNumPoints;
    /* 0x08 */ SpiderThreadMainPoint** mPoints;
};
