#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.h"

namespace MR {
    void initAcosTable();

    f32 getRandom();
    f32 getRandom(f32, f32);
    s32 getRandom(s32, s32);
    f32 getRandomDegree();
    void calcRandomVec(TVec3f *, f32, f32);
    bool isHalfProbability();
    f32 getSignHalfProbability();
    void getRandomVector(TVec3f *, f32);
    void addRandomVector(TVec3f *, const TVec3f &, f32);
    void turnRandomVector(TVec3f *, const TVec3f &, f32);
    f32 getInterpolateValue(f32, f32, f32);
    f32 getLinerValue(f32, f32, f32, f32);
    f32 getLinerValueFromMinMax(f32, f32, f32, f32, f32);
    f32 getEaseInValue(f32, f32, f32, f32);
    f32 getEaseOutValue(f32, f32, f32, f32);

    // MR::getEaseInOutValue
    // MR::getScaleWithReactionValueZeroToOne
    // MR::getConvergeVibrationValue
    // MR::getReduceVibrationValue

    void makeAxisFrontUp(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisFrontSide(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisUpFront(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisUpSide(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisVerticalZX(TVec3f *, const TVec3f &);
    void makeAxisCrossPlane(TVec3f *, TVec3f *, const TVec3f &);
    void makeAxisAndCosignVecToVec(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    f32 calcPerpendicFootToLine(TVec3f *, const TVec3f &, const TVec3f &, const TVec3f &);
    f32 calcPerpendicFootToLineInside(TVec3f *, const TVec3f &, const TVec3f &, const TVec3f &);

    f32 convergeRadian(f32, f32, f32);
    bool isInRange(f32, f32, f32);

    f32 calcDistanceXY(const TVec3f &, const TVec3f &);

    bool isNearZero(const TVec3f &, f32);

    f32 diffAngleAbs(f32, f32);
    f32 normalizeAngleAbs(f32);
    bool isAngleBetween(f32, f32, f32);
    f32 blendAngle(f32, f32, f32);
};