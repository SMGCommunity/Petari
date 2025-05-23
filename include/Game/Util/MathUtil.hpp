#pragma once

#include <cmath>
#include <revolution.h>
#include "JSystem/JGeometry.hpp"
#include "JSystem/JGeometry/TQuat.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace MR {
    void initAcosTable();

    f32 getRandom();
    f32 getRandom(f32, f32);
    s32 getRandom(s32, s32);
    f32 getRandomDegree();
    void calcRandomVec(TVec3f *, f32, f32);
    u8 isHalfProbability();
    f32 getSignHalfProbability();
    void getRandomVector(TVec3f *, f32);
    void addRandomVector(TVec3f *, const TVec3f &, f32);
    void turnRandomVector(TVec3f *, const TVec3f &, f32);
    f32 getInterpolateValue(f32, f32, f32);
    f32 getLinerValue(f32, f32, f32, f32);
    f32 getLinerValueFromMinMax(f32, f32, f32, f32, f32);
    f32 getEaseInValue(f32, f32, f32, f32);
    f32 getEaseOutValue(f32, f32, f32, f32);
    f32 getEaseInOutValue(f32, f32, f32, f32);
    f32 getScaleWithReactionValueZeroToOne(f32, f32, f32);
    // MR::getConvergeVibrationValue
    // MR::getReduceVibrationValue
    void separateScalarAndDirection(f32 *pScalar, TVec3f *pDirection, const TVec3f &rSrc);

    void makeAxisFrontUp(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisFrontSide(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisUpFront(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisUpSide(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);

    /* 
     * Generate an orthogonal vector to the second argument, starting by projecting the z-vector
     * into the plane orthogonal to the second argument. If the z-vector is parallel to the second
     * argument, the x-vector is instead projected into the orthognal plane. Regardless, the
     * normalized result is placed into the first argument.
     */
    void makeAxisVerticalZX(TVec3f *, const TVec3f &);

    void makeAxisCrossPlane(TVec3f *, TVec3f *, const TVec3f &);
    bool makeAxisAndCosignVecToVec(TVec3f *, f32 *, const TVec3f &, const TVec3f &);

    /*
     * Projects rPoint onto the directed line defined by rTip and rTail and places the result into pOut
     */
    f32 calcPerpendicFootToLine(TVec3f *pOut, const TVec3f &rPoint, const TVec3f &rTip, const TVec3f &rTail);
    
    /*
     * Same as above, except the result of the projection is clamped between rTip and rTail
     */
    f32 calcPerpendicFootToLineInside(TVec3f *pOut, const TVec3f &rPoint, const TVec3f &rTip, const TVec3f &rTail);

    void blendQuatUpFront(TQuat4f *, const TVec3f &, const TVec3f &, f32, f32);

    void rotateQuatRollBall(TQuat4f *, const TVec3f &, const TVec3f &, f32);

    void clampLength(TVec3f *, const TVec3f &, f32);
    f32 convergeRadian(f32, f32, f32);
    bool isInRange(f32, f32, f32);

    f32 calcRotateY(f32, f32);

    f32 calcDistanceXY(const TVec3f &, const TVec3f &);

    void rotateVecDegree(TVec3f *, const TVec3f &, f32);
    void rotateVecRadian(TVec3f *, const TVec3f &, f32);

    bool isNearZero(f32, f32);
    bool isNearZero(const TVec3f &, f32);

    bool isSameDirection(const TVec3f &, const TVec3f &, f32);
    bool isOppositeDirection(const TVec3f &, const TVec3f &, f32);

    f32 diffAngleAbs(f32, f32);
    f32 diffAngleAbs(const TVec3f &, const TVec3f &);
    f32 diffAngleAbsHorizontal(const TVec3f &, const TVec3f &, const TVec3f &);
    f32 diffAngleSignedHorizontal(const TVec3f &, const TVec3f &, const TVec3f &);
    f32 normalizeAngleAbs(f32);
    bool isAngleBetween(f32, f32, f32);
    f32 blendAngle(f32, f32, f32);
    u8 lerp(u8, u8, f32);
    _GXColor lerp(_GXColor, _GXColor, f32);

    // MR::sortSmall(s32, f32 *, s32 *)
    // MR::sortSmall(s32, u32 *, s32 *)

    f32 vecKillElement(const TVec3f &, const TVec3f &, TVec3f *);
    void vecScaleAdd(const TVec3f *, const TVec3f *, f32);
    void PSvecBlend(const TVec3f *, const TVec3f *, TVec3f *, f32, f32);
    void vecBlend(const TVec3f &, const TVec3f &, TVec3f *, f32);

    s32 vecBlendSphere(const TVec3f &, const TVec3f &, TVec3f *, f32);

    void blendColor(_GXColor *, const _GXColor &, const _GXColor  &, f32);
    void blendVec(Vec *, const Vec &, const Vec &, f32);

    int getMinAbsElementIndex(const TVec3f &);
    f32 getMaxElement(const TVec3f &);
    f32 getMaxAbsElement(const TVec3f &);
    int getMaxElementIndex(const TVec3f &);
    int getMaxAbsElementIndex(const TVec3f &);
    f32 diffAngleAbsFast(const TVec3f &, const TVec3f &);

    bool isNearAngleRadian(const TVec3f &, const TVec3f &, f32);
    bool isNearAngleDegree(const TVec3f &, const TVec3f &, f32);
    bool isNearAngleRadianHV(const TVec3f &, const TVec3f &, const TVec3f &, f32, f32);
    bool isNearAngleDegreeHV(const TVec3f &, const TVec3f &, const TVec3f &, f32, f32);

    f32 mod(f32, f32);

    void normalize(TVec2f *);
    void normalize(TVec3f *);
    void normalize(const TVec3f &, TVec3f *);

    void normalize(TPos3f *);

    f32 normalize(f32, f32, f32);
    f32 normalizeAbs(f32, f32, f32);

    bool normalizeOrZero(TVec2f *);
    bool normalizeOrZero(TVec3f *);
    bool normalizeOrZero(const TVec3f &, TVec3f *);

    bool isNan(const TVec3f &);

    template<typename T>
    T converge(T, T, T);

    f32 clamp(f32, f32, f32);
    s32 clamp(s32, s32, s32);

    f32 getScaleWithReactionValueZeroToOne(f32, f32, f32);

    f32 getConvergeVibrationValue(f32, f32, f32, f32, f32);

    void calcReboundVelocity(TVec3f *, const TVec3f &, f32, f32);

    void getRotatedAxisZ(TVec3f *, const TVec3f &);
    void getRotatedAxisY(TVec3f *, const TVec3f &);

    void turnVecToVecCosOnPlane(TVec3f *, const TVec3f &, const TVec3f &, f32);

    void turnVecToVecDegree(TVec3f *, const TVec3f &, const TVec3f &, f32, const TVec3f &);

    void turnVecToPlane(TVec3f *, const TVec3f &, const TVec3f &);

    f32 cosDegree(f32);
    f32 sinDegree(f32);

    // this must not be declared as inline. some callers inline it and some do not
    inline f32 max(f32 x, f32 y);

    f32 cos(f32);
    f32 sin(f32);

    void checkHitSegmentSphere(const TVec3f &, const TVec3f &, const TVec3f &, TVec3f *);

    inline f32 adjustAngle(f32 angle, f32 delta, f32 offset) {
        return (angle + delta) - offset;
    }

    inline f32 normalizeAngle(f32 a, f32 b) {
        return a + fmod(360.0 + b, 360.0);
    }

    inline f32 wrapAngleTowards(f32 a, f32 b) {
        return a + (f32)fmod(360.0f + (b - a), 360.0f);
    }

    #ifdef __MWERKS__
    inline f32 frsqrte(register f32 val) {
        register f32 recip;
        __asm {
                frsqrte recip, val
            }
        return recip * val;
    }
    
    inline f32 speedySqrtf(register f32 x) {
        register f32 recip;

        if (x > 0.0f) {
            __asm { frsqrte recip, x }
            f32 v = recip * x;
            recip =  -((v*recip) - 3.0f);
            recip = (recip * v);
            recip *= 0.5f;
            return recip;
        }
        return x;
    }
    #else
    f32 frsqrte(f32 val);
    f32 speedySqrtf(f32);
    #endif

    template<typename T>
    T sqrt(T val) {
        return speedySqrtf(val);
    }

    bool turnQuatZDirRad(TQuat4f *, const TQuat4f &, const TVec3f &, f32);
};

f32 PSVECKillElement(const Vec *, const Vec *, const Vec *);
