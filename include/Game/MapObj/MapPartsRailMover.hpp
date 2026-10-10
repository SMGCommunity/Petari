#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include <JSystem/JGeometry.hpp>

class MapPartsRailPointPassChecker;

class MapPartsRailMover : public MapPartsFunction {
public:
    MapPartsRailMover(LiveActor*);

    virtual ~MapPartsRailMover();
    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual bool isWorking() const;
    virtual void start();
    virtual void end();
    virtual bool receiveMsg(u32);

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

    /* 0x18 */ MapPartsRailPointPassChecker* mRailPointPassChecker;
    /* 0x1C */ s32 mMoveConditionType;
    /* 0x20 */ s32 mMoveStopType;
    /* 0x24 */ s32 mSignMotionType;
    /* 0x28 */ TVec3f _28;
    /* 0x34 */ f32 _34;
    /* 0x38 */ s32 mStopTime;
    /* 0x3C */ f32 mSpeed;
    /* 0x40 */ s32 mAccelTime;
    /* 0x44 */ f32 mAcceleration;
    /* 0x48 */ f32 _48;
};
