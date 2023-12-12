#pragma once

class CameraShakePattern;

class CameraShakeTask {
public:
    CameraShakeTask(CameraShakePattern *);

    void start(unsigned long, unsigned long);
    void startInfinity(unsigned long);
    void endForce();
    void movement();
    void getOffset(TVec2f *) const;
    bool isEnd() const;
    void startCommon(unsigned long);
    void updatePattern();
    void updateInterval();

    CameraShakePattern *mPattern;   // _0
    bool mHasEnded;                 // _4
    bool mIsInfinite;               // _5
    u8 _6[2];
    u32 _8;
    u32 _C;
    u32 _10;
};