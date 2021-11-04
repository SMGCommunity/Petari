#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.h"

static f32 minDegree = 0.0f;
static f32 maxDegree = 360.0f;

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


    void clampLength(TVec3f *, const TVec3f &, f32);
    f32 convergeRadian(f32, f32, f32);
    bool isInRange(f32, f32, f32);

    f32 calcRotateY(f32, f32);

    f32 calcDistanceXY(const TVec3f &, const TVec3f &);

    bool isNearZero(f32, f32);
    bool isNearZero(const TVec3f &, f32);

    f32 diffAngleAbs(f32, f32);
    f32 normalizeAngleAbs(f32);
    bool isAngleBetween(f32, f32, f32);
    f32 blendAngle(f32, f32, f32);
    u8 lerp(u8, u8, f32);
    _GXColor lerp(_GXColor, _GXColor, f32);

    // MR::sortSmall(long, float *, long *)
    // MR::sortSmall(long, unsigned long *, long *)

    f32 vecKillElement(const TVec3f &, const TVec3f &, TVec3f *);
    void vecScaleAdd(const TVec3f *, const TVec3f *, f32);
    void PSvecBlend(const TVec3f *, const TVec3f *, TVec3f *, f32, f32);
    void vecBlend(const TVec3f &, const TVec3f &, TVec3f *, f32);

    void blendColor(_GXColor *, const _GXColor &, const _GXColor  &, f32);
    void blendVec(Vec *, const Vec &, const Vec &, f32);

    int getMinAbsElementIndex(const TVec3f &);
    f32 getMaxElement(const TVec3f &);
    f32 getMaxAbsElement(const TVec3f &);
    int getMaxElementIndex(const TVec3f &);
    int getMaxAbsElementIndex(const TVec3f &);

    f32 mod(f32, f32);

    void normalize(TVec3f *);
};

f32 PSVECKillElement(const Vec *, const Vec *, const Vec *);