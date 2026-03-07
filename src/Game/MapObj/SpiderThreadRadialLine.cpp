#include "Game/MapObj/SpiderThreadRadialLine.hpp"
#include "Game/MapObj/SpiderThreadMainPoint.hpp"

SpiderThreadRadialLine::SpiderThreadRadialLine(int bufferSize) : mBufferSize(bufferSize), mNumPoints(0), mPoints(nullptr) {
    mPoints = new SpiderThreadMainPoint*[mBufferSize];
    for (s32 idx = 0; idx < mBufferSize; idx++) {
        mPoints[idx] = nullptr;
    }
}

void SpiderThreadRadialLine::addPoint(SpiderThreadMainPoint* pPoint) {
    mPoints[mNumPoints] = pPoint;
    mNumPoints++;
}

void SpiderThreadRadialLine::updateVelocity() {
    for (s32 idx = 0; idx < mNumPoints; idx++) {
        mPoints[idx]->updateVelocity();
    }
}

void SpiderThreadRadialLine::updatePos() {
    for (s32 idx = 0; idx < mNumPoints; idx++) {
        mPoints[idx]->updatePos();
    }
}
