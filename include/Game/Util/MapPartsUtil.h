#pragma once

#include "Game/Util.h"

class HitSensor;

namespace MR {
    bool getMapPartsArgMoveConditionType(s32 *, const JMapInfoIter &);
    bool getMapPartsArgRotateSpeed(f32 *, const JMapInfoIter &);
    bool getMapPartsArgRotateTime(s32 *, const JMapInfoIter &);
    bool getMapPartsArgRotateAngle(f32 *, const JMapInfoIter &);
    bool getMapPartsArgRotateAxis(s32 *, const JMapInfoIter &);
    bool getMapPartsArgRotateAccelType(s32 *, const JMapInfoIter &);
    bool getMapPartsArgRotateStopTime(s32 *, const JMapInfoIter &);
    bool getMapPartsArgRotateType(s32 *, const JMapInfoIter &);
    s32 getMapPartsArgShadowType(s32 *, const JMapInfoIter &);
    bool getMapPartsArgSignMotionType(s32 *, const JMapInfoIter &);
    bool getMapPartsArgMovePosture(s32 *, const JMapInfoIter &);
    bool getMapPartsArgMoveStopType(s32 *, const JMapInfoIter &);
    bool getMapPartsArgRailGuideType(s32 *, const JMapInfoIter &);
    bool getMapPartsArgRailInitPosType(s32 *, const JMapInfoIter &);
    bool getMapPartsArgMoveSpeed(f32 *, const JMapInfoIter &);
    bool getMapPartsArgStopTime(s32 *, const JMapInfoIter &);
    bool getMapPartsArgAccelTime(s32 *, const JMapInfoIter &);
    bool getMapPartsArgSpeedCalcType(s32 *, const JMapInfoIter &);
    bool getMapPartsArgRailRotateSpeed(f32 *, const JMapInfoIter &);
    bool getMapPartsArgRailRotateTime(s32 *, const JMapInfoIter &);

    bool hasMapPartsShadow(s32);

    bool hasMapPartsMoveStartSignMotion(s32);

    bool isMoveStartUnconditional(s32);

    void setBodySensorType(LiveActor *, u32);

    const HitSensor* getBodySensor(LiveActor *);
};