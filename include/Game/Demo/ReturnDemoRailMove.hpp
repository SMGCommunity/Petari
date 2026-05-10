#pragma once

#include "JSystem/JGeometry/TMatrix.hpp"

class LiveActor;
class JMapInfoIter;
class SpinDriverShootPath;
class SpinDriverPathDrawer;

class ReturnDemoRailMove {
public:
    ReturnDemoRailMove(LiveActor*, LiveActor*, const JMapInfoIter&, bool, TPos3f*);

    void posToStart();
    void posToEnd();
    void offPathDraw();
    s32 getDemoFlyBrakeFrame() const;
    void calcPathPosDir(TVec3f*, TVec3f*, f32) const;
    void setupPathDrawForGraneStarReturnDemo();
    void start();
    void update(s32, s32);

    /* 0x0 */ LiveActor* mDemoStarter;  // Can be either StarReturnDemoStarter or GrandStarReturnDemoStarter
    /* 0x4 */ LiveActor* mPowerStar;
    /* 0x8 */ bool mIsGrandStar;
    /* 0xC */ TPos3f* mTransform;
    /* 0x10 */ SpinDriverShootPath* mShootPath;
    /* 0x14 */ SpinDriverPathDrawer* mPathDrawer;
    /* 0x18 */ TVec3f mForward;
};
