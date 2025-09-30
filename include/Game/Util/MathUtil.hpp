#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TQuat.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

namespace MR {
    void initAcosTable();
    f32 acosEx(f32);
    f32 getRandom();
    f32 getRandom(f32, f32);
    s32 getRandom(s32, s32);
    f32 getRandomDegree();
    void calcRandomVec(TVec3f*, f32, f32);
    bool isHalfProbability() NO_INLINE;
    f32 getSignHalfProbability();
    void getRandomVector(TVec3f*, f32);
    void addRandomVector(TVec3f*, const TVec3f&, f32);
    void turnRandomVector(TVec3f*, const TVec3f&, f32);
    f32 getInterpolateValue(f32, f32, f32);
    f32 getLinerValue(f32, f32, f32, f32);
    f32 getLinerValueFromMinMax(f32, f32, f32, f32, f32);
    f32 getEaseInValue(f32, f32, f32, f32);
    f32 getEaseOutValue(f32, f32, f32, f32);
    f32 getEaseInOutValue(f32, f32, f32, f32);
    f32 getScaleWithReactionValueZeroToOne(f32, f32, f32);
    f32 getConvergeVibrationValue(f32, f32, f32, f32, f32);
    f32 getReduceVibrationValue(f32, f32, f32, f32, f32);
    void makeAxisFrontUp(TVec3f*, TVec3f*, const TVec3f&, const TVec3f&);
    void makeAxisFrontSide(TVec3f*, TVec3f*, const TVec3f&, const TVec3f&);
    void makeAxisUpFront(TVec3f*, TVec3f*, const TVec3f&, const TVec3f&);
    void makeAxisUpSide(TVec3f*, TVec3f*, const TVec3f&, const TVec3f&);

    /* 
     * Generate an orthogonal vector to the second argument, starting by projecting the z-vector
     * into the plane orthogonal to the second argument. If the z-vector is parallel to the second
     * argument, the x-vector is instead projected into the orthognal plane. Regardless, the
     * normalized result is placed into the first argument.
     */
    void makeAxisVerticalZX(TVec3f*, const TVec3f&);

    void makeAxisCrossPlane(TVec3f*, TVec3f*, const TVec3f&);
    bool makeAxisAndCosignVecToVec(TVec3f*, f32*, const TVec3f&, const TVec3f&);

    /*
     * Projects rPoint onto the directed line defined by rTip and rTail and places the result into pOut
     */
    f32 calcPerpendicFootToLine(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&);

    /*
     * Same as above, except the result of the projection is clamped between rTip and rTail
     */
    f32 calcPerpendicFootToLineInside(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&);

    bool checkHitSegmentSphere(const TVec3f&, const TVec3f&, const TVec3f&, f32, TVec3f*);
    bool checkHitSemilinePlane(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&, const TVec3f&);
    bool calcReboundVelocity(TVec3f*, const TVec3f&, f32);
    bool calcReboundVelocity(TVec3f*, const TVec3f&, f32, f32);
    void calcParabolicFunctionParam(f32*, f32*, f32, f32);
    void makeQuatRotateRadian(TQuat4f*, const TVec3f&);
    void makeQuatRotateDegree(TQuat4f*, const TVec3f&);
    void makeQuatFromVec(TQuat4f*, const TVec3f&, const TVec3f&);
    void makeQuatSideUp(TQuat4f*, const TVec3f&, const TVec3f&);
    void makeQuatUpFront(TQuat4f*, const TVec3f&, const TVec3f&);
    void makeQuatUpNoSupport(TQuat4f*, const TVec3f&);
    void blendQuatUpFront(TQuat4f*, const TVec3f&, const TVec3f&, f32, f32);
    void blendQuatUpFront(TQuat4f*, const TQuat4f&, const TVec3f&, const TVec3f&, f32, f32);
    void blendQuatFrontUp(TQuat4f*, const TVec3f&, const TVec3f&, f32, f32);
    void blendQuatFrontUp(TQuat4f*, const TQuat4f&, const TVec3f&, const TVec3f&, f32, f32);
    void rotateQuatMoment(TQuat4f*, const TVec3f&);
    void rotateQuatRollBall(TQuat4f*, const TVec3f&, const TVec3f&, f32);
    bool turnQuat(TQuat4f*, const TQuat4f&, const TVec3f&, const TVec3f&, f32);
    bool turnQuatXDirRad(TQuat4f*, const TQuat4f&, const TVec3f&, f32);
    void turnQuatXDirRate(TQuat4f*, const TQuat4f&, const TVec3f&, f32);
    bool turnQuatYDirRad(TQuat4f*, const TQuat4f&, const TVec3f&, f32);
    void turnQuatYDirRate(TQuat4f*, const TQuat4f&, const TVec3f&, f32);
    bool turnQuatZDirRad(TQuat4f*, const TQuat4f&, const TVec3f&, f32);
    f32 sign(f32);
    void clampVecAngleDeg(TVec3f*, const TVec3f&, f32);
    void clampLength(TVec3f*, const TVec3f&, f32);
    f32 convergeRadian(f32, f32, f32);
    bool isInRange(f32, f32, f32);
    f32 calcRotateY(f32, f32);
    f32 calcRotateZ(const TVec3f&, const TVec3f&);
    f32 calcDistanceXY(const TVec3f&, const TVec3f&);
    void rotateVecDegree(f32*, f32*, f32);
    void rotateVecDegree(TVec2f*, f32);
    void rotateVecDegree(TVec3f*, const TVec3f&, f32);
    void rotateVecDegree(TVec3f*, const TVec3f&, const TVec3f&, f32);
    void rotateVecRadian(TVec3f*, const TVec3f&, f32);
    void rotateVecRadian(TVec3f*, const TVec3f&, const TVec3f&, f32);
    void calcLocalVec(TVec3f*, MtxPtr);
    void normalize(TVec2f*);
    void normalize(TVec3f*);
    void normalize(TPos3f*);
    void normalize(const TVec2f&, TVec2f*);
    void normalize(const TVec3f&, TVec3f*);
    bool normalizeOrZero(TVec2f*);
    bool normalizeOrZero(TVec3f*);
    bool normalizeOrZero(const TVec3f&, TVec3f*);
    void separateScalarAndDirection(f32*, TVec2f*, const TVec2f&);
    void separateScalarAndDirection(f32*, TVec3f*, const TVec3f&);
    f32 normalize(f32, f32, f32);
    f32 normalizeAbs(f32, f32, f32);
    bool turnVecToVecCos(TVec3f*, const TVec3f&, const TVec3f&, f32, const TVec3f&, f32);
    bool turnVecToVecCosOnPlane(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&, f32);
    bool turnVecToVecCosOnPlane(TVec3f*, const TVec3f&, const TVec3f&, f32);
    f32 turnVecToVecRadian(TVec3f*, const TVec3f&, const TVec3f&, f32, const TVec3f&);
    f32 turnVecToVecDegree(TVec3f*, const TVec3f&, const TVec3f&, f32, const TVec3f&);
    void calcMomentRollBall(TVec3f*, const TVec3f&, const TVec3f&, f32);
    bool calcReflectionVector(TVec3f*, const TVec3f&, f32, f32);
    bool isSameDirection(const TVec3f&, const TVec3f&, f32);
    bool isOppositeDirection(const TVec3f&, const TVec3f&, f32);
    bool isNearZero(f32, f32);
    bool isNearZero(const TVec3f&, f32);
    bool isNearZero(const TVec2f&, f32);
    f32 diffAngleAbs(f32, f32);
    f32 normalizeAngleAbs(f32);
    bool isAngleBetween(f32, f32, f32);
    f32 blendAngle(f32, f32, f32);
    u8 lerp(u8, u8, f32);
    GXColor lerp(GXColor, GXColor, f32);
    void sortSmall(s32, f32*, s32*);
    void sortSmall(s32, u32*, s32*);
    f32 vecKillElement(const TVec3f&, const TVec3f&, TVec3f*);
    void vecScaleAdd(const TVec3f*, const TVec3f*, f32);
    void PSvecBlend(const TVec3f*, const TVec3f*, TVec3f*, f32, f32);
    void vecBlend(const TVec3f&, const TVec3f&, TVec3f*, f32);
    void vecBlendNormal(const TVec3f&, const TVec3f&, TVec3f*, f32);
    bool vecBlendSphere(const TVec3f&, const TVec3f&, TVec3f*, f32);
    void vecRotAxis(const TVec3f&, const TVec3f&, const TVec3f&, TVec3f*, f32);
    void blendColor(GXColor*, const GXColor&, const GXColor&, f32);
    void blendVec(Vec*, const Vec&, const Vec&, f32);
    f32 turnVecToPlane(TVec3f*, const TVec3f&, const TVec3f&);
    u32 getMinAbsElementIndex(const TVec3f&);
    f32 getMaxElement(const TVec3f&);
    f32 getMaxAbsElement(const TVec3f&);
    u32 getMaxElementIndex(const TVec3f&);
    u32 getMaxAbsElementIndex(const TVec3f&);
    f32 diffAngleAbsFast(const TVec3f&, const TVec3f&);
    f32 diffAngleAbs(const TVec3f&, const TVec3f&);
    f32 diffAngleAbs(const TVec2f&, const TVec2f&);
    f32 diffAngleSigned(const TVec3f&, const TVec3f&, const TVec3f&);
    f32 diffAngleAbsHorizontal(const TVec3f&, const TVec3f&, const TVec3f&);
    f32 diffAngleSignedHorizontal(const TVec3f&, const TVec3f&, const TVec3f&);
    bool isNearAngleRadian(const TVec3f&, const TVec3f&, f32);
    bool isNearAngleDegree(const TVec3f&, const TVec3f&, f32);
    bool isNearAngleRadianHV(const TVec3f&, const TVec3f&, const TVec3f&, f32, f32);
    bool isNearAngleDegreeHV(const TVec3f&, const TVec3f&, const TVec3f&, f32, f32);
    void createBoundingBox(const TVec3f*, u32, TVec3f*, TVec3f*);
    bool isNormalize(const TVec3f&, f32);
    void setNan(TVec3f& rVec);
    bool isNan(const TVec3f& rVec);
    f32 getFootPoint(const TVec3f&, const TVec3f&, const TVec3f&, TVec3f*);
    f32 mod(f32, f32);
    void floatToFixed16(TVec3s*, const TVec3f&, u8);
    void fixed16ToFloat(TVec3f*, const TVec3s&, u8);
    void getRotatedAxisY(TVec3f*, const TVec3f&);
    void getRotatedAxisZ(TVec3f*, const TVec3f&);

    template<typename T>
    T converge(T, T, T);

    f32 cos(f32);
    f32 sin(f32);
    f32 cosDegree(f32);
    f32 sinDegree(f32);
    f32 min(f32, f32);
    f32 max(f32, f32);
    f32 clamp(f32, f32, f32);
    s32 clamp(s32, s32, s32);
    s32 clamp01(f32*);
    s32 clampBoth(f32* pX, f32 min, f32 max);

    inline f32 wrapAngleTowards(f32 a, f32 b) {
        return a + (f32)fmod(360.0f + (b - a), 360.0f);
    }

    #ifdef __MWERKS__
    inline f32 frsqrte(register f32 x) {
        register f32 recip;

        __asm {
            frsqrte recip, x

        }
        return recip * x;
    }
    
    inline f32 fastSqrtf(register f32 x) {
        register f32 recip;

        if (x > 0.0f) {
            __asm { frsqrte recip, x }
            f32 v = recip * x;
            recip =  -(v * recip - 3.0f);
            recip = (recip * v);
            recip *= 0.5f;
            return recip;
        }

        return x;
    }
    #else
    f32 frsqrte(f32);
    f32 fastSqrtf(f32);
    #endif

    template<typename T>
    T sqrt(T x) {
        return fastSqrtf(x);
    }
};

f32 PSVECKillElement(const Vec*, const Vec*, const Vec*);
f32 JMASqrt(f32);
