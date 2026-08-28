#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include <JSystem/JGeometry.hpp>

class MapPartsRailPointPassChecker;

class MapPartsRailMover : public MapPartsFunction {
public:
    MapPartsRailMover(LiveActor*);

    /* 0x08 */ virtual ~MapPartsRailMover();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();
    /* 0x24 */ virtual bool isWorking() const;
    /* 0x28 */ virtual void start();
    /* 0x2C */ virtual void end();
    /* 0x30 */ virtual bool receiveMsg(u32);
    /* 0x34 */ virtual void control();

    void moveToInitPos();
    void startWithSignalMotion();
    void cancelSignalMotion();
    bool tryResetPositionRepeat();
    void resetToInitPos();
    void passPoint();
    void reachedEnd();
    void reachedEndPlayerOn();
    bool isReachedEnd() const;
    bool isDone() const;
    void setStateStopAtEndBeforeRotate();
    void calcTimeToNextRailPoint(f32*) const;
    void endRotateAtPoint();
    void calcMoveSpeed(f32*) const;
    void calcMoveSpeedDirect(f32*) const;
    void calcMoveSpeedTime(f32*) const;
    void updateAccel();
    bool tryPassPoint();
    bool tryRestartAtEnd();
    void restartAtEnd();
    void exeMove();
    void exeMoveStart();
    void exeStopAtPoint();
    void exeStopAtEnd();
    void exeWait();
    void exeVanish();
    void exeRotateAtPoint();
    void exeWaitForRestartByPlayerOn();
    void exeStopAtEndWithPlayerOn();
    void exeRotateAtEndPoint();

    MapPartsRailPointPassChecker* mRailPointPassChecker;  // 0x18
    s32 mMoveConditionType;                               // 0x1C
    s32 mMoveStopType;                                    // 0x20
    s32 mSignMotionType;                                  // 0x24
    TVec3f _28;
    f32 _34;
    s32 mStopTime;      // 0x38
    f32 mSpeed;         // 0x3C
    s32 mAccelTime;     // 0x40
    f32 mAcceleration;  // 0x44
    f32 _48;
};
