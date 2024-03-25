#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include <JSystem/JGeometry.hpp>

class MapPartsRailPointPassChecker;

class MapPartsRailMover : public MapPartsFunction {
public:
    MapPartsRailMover(LiveActor *);

    virtual ~MapPartsRailMover();
    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual bool isWorking() const;
    virtual void start();
    virtual void end();
    virtual bool receiveMsg(u32);
    virtual void control();

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
    void calcTimeToNextRailPoint(f32 *) const;
    void endRotateAtPoint();
    void calcMoveSpeed(f32 *) const;
    void calcMoveSpeedDirect(f32 *) const;
    void calcMoveSpeedTime(f32 *) const;
    void updateAccel();
    bool tryPassPoint();
    bool tryRestartAtEnd();
    void restartAtEnd();
    void exeMove();
    void exeMoveStart();
    void exeStopAtPoint();
    void exeStopAtEnd();

    MapPartsRailPointPassChecker* mRailPointPassChecker;    // _18
    s32 mMoveConditionType;                                 // _1C
    s32 mMoveStopType;                                      // _20
    s32 mSignMotionType;                                    // _24
    TVec3f _28;
    f32 _34;
    s32 mStopTime;                                          // _38
    f32 _3C;
    u32 _40;
    f32 _44;
    f32 _48;
};