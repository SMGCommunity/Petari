#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class MapPartsRailMover;
class OceanSmallTurtle : public LiveActor {
public:
    OceanSmallTurtle(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void exeWait();
    void exeFloatSignal();
    void exeFloatUp();
    void exeCatchRail();
    void exeRailMove();
    void startFloat();
    bool boundFloater();
    bool catchNearestRailPos();
    void floatFloater();
    bool isNeedsForBound();
    bool isEndSpringWater();

private:
    /* 0x8C */ MapPartsRailMover* mRailMover;
    /* 0x90 */ f32 mFloatUpSpeed;
    /* 0x94 */ f32 mAccelRate;
    /* 0x98 */ TVec3f mStartNearestRailPos;
    /* 0xA4 */ f32 mFloatDelayTimer;  // used as a timer. a float for some reason
    /* 0xA8 */ f32 mArg1;
    /* 0xAC */ s32 mBoundTimer;
    /* 0xB0 */ TVec3f mMercatorPos;
    /* 0xBC */ TVec3f mInfoRotate;
    /* 0xC8 */ f32 mBoundDisplacement;
    /* 0xCC */ TVec3f mRailClippingPos;
    /* 0xD8 */ f32 mRailClippingRadius;
    /* 0xDC */ f32 mStartRotationX;
    /* 0xE0 */ u8 mIsOnPlayer;
    /* 0xE1 */ u8 isBound;
    /* 0xE2 */ u8 isStartFloat;
    /* 0xE4 */ TPos3f mEffectMtx;
};
