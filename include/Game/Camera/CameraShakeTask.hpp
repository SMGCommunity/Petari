#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class CameraShakePattern;

class CameraShakeTask {
public:
    CameraShakeTask(CameraShakePattern*);

    void start(u32, u32);
    void startInfinity(u32);
    void endForce();
    void movement();
    void getOffset(TVec2f*) const;
    bool isEnd() const;
    void startCommon(u32);
    void updatePattern();
    void updateInterval();

    CameraShakePattern* mPattern;  // 0x0
    bool mHasEnded;                // 0x4
    bool mIsInfinite;              // 0x5
    u8 _6[2];
    u32 _8;
    u32 _C;
    u32 _10;
};
