#pragma once

#include <JSystem/JGeometry.hpp>

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

    /* 0x00 */ CameraShakePattern* mPattern;
    /* 0x04 */ bool mHasEnded;
    /* 0x05 */ bool mIsInfinite;
    /* 0x08 */ u32 mTime;
    /* 0x0C */ u32 mDelay;
    /* 0x10 */ u32 mDelayTimer;
};
