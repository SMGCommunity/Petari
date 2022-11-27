#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.h"

static f32 minDegree = 0.0f;
static f32 maxDegree = 360.0f;

static f32 flt_8060FC80[1816];

#ifdef __cplusplus
extern "C" {
f64 sin(f32);
f64 cos(f32);
f64 fmod(f64, f64);
f64 asin(f32);
}
#endif


namespace MR {
    void initAcosTable();

    template<typename T>
    T sqrt(T val);

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
    // MR::getScaleWithReactionValueZeroToOne
    // MR::getConvergeVibrationValue
    // MR::getReduceVibrationValue
    void separateScalarAndDirection(f32 *pScalar, TVec3f *pDirection, TVec3f &rSrc);

    void makeAxisFrontUp(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisFrontSide(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisUpFront(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisUpSide(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    void makeAxisVerticalZX(TVec3f *, const TVec3f &);
    void makeAxisCrossPlane(TVec3f *, TVec3f *, const TVec3f &);
    void makeAxisAndCosignVecToVec(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &);
    f32 calcPerpendicFootToLine(TVec3f *, const TVec3f &, const TVec3f &, const TVec3f &);
    f32 calcPerpendicFootToLineInside(TVec3f *, const TVec3f &, const TVec3f &, const TVec3f &);

    void blendQuatUpFront(TQuat4f *, const TVec3f &, const TVec3f &, float, float);

    void clampLength(TVec3f *, const TVec3f &, f32);
    f32 convergeRadian(f32, f32, f32);
    bool isInRange(f32, f32, f32);

    f32 calcRotateY(f32, f32);

    f32 calcDistanceXY(const TVec3f &, const TVec3f &);

    void rotateVecDegree(TVec3f *, const TVec3f &, f32);

    bool isNearZero(f32, f32);
    bool isNearZero(const TVec3f &, f32);

    bool isSameDirection(const TVec3f &, const TVec3f &, float);

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

    s32 vecBlendSphere(const TVec3f &, const TVec3f &, TVec3f *, f32);

    void blendColor(_GXColor *, const _GXColor &, const _GXColor  &, f32);
    void blendVec(Vec *, const Vec &, const Vec &, f32);

    int getMinAbsElementIndex(const TVec3f &);
    f32 getMaxElement(const TVec3f &);
    f32 getMaxAbsElement(const TVec3f &);
    int getMaxElementIndex(const TVec3f &);
    int getMaxAbsElementIndex(const TVec3f &);
    f32 diffAngleAbsFast(const TVec3f &, const TVec3f &);

    bool isNearAngleRadian(const TVec3f &, const TVec3f &, float);
    bool isNearAngleDegree(const TVec3f &, const TVec3f &, float);
    bool isNearAngleRadianHV(const TVec3f &, const TVec3f &, const TVec3f &, float, float);
    bool isNearAngleDegreeHV(const TVec3f &, const TVec3f &, const TVec3f &, float, float);

    f32 mod(f32, f32);

    void normalize(TVec2f *);
    void normalize(TVec3f *);
    void normalize(const TVec3f &, TVec3f *);

    f32 normalize(f32, f32, f32);

    bool normalizeOrZero(TVec2f *);
    bool normalizeOrZero(TVec3f *);
    bool normalizeOrZero(const TVec3f &, TVec3f *);

    bool isNan(const TVec3f &);

    template<typename T>
    T converge(T, T, T);

    f32 clamp(f32, f32, f32);

    f32 getScaleWithReactionValueZeroToOne(f32, f32, f32);

    void calcReboundVelocity(TVec3f *, const TVec3f &, f32, f32);

    void getRotatedAxisZ(TVec3f *, const TVec3f &);
    void getRotatedAxisY(TVec3f *, const TVec3f &);

    void turnVecToVecCosOnPlane(TVec3f *, const TVec3f &, const TVec3f &, f32);

    void turnVecToPlane(TVec3f *, const TVec3f &, const TVec3f &);

    f32 cosDegree(f32);
    f32 sinDegree(f32);

    /* there's a couple of issues with stack ordering when it comes to vectors being created and scaled
     * this function automates this and resolves most issues
    */
    inline TVec3f createVecAndScale(const TVec3f &rSrc, f32 scalar) {
        TVec3f vec(rSrc);
        vec.scale(scalar);
        return vec;
    }

    inline TVec3f createNegatedVecAndScale(const TVec3f &rSrc, f32 scalar) {
        return rSrc.negateInline() % scalar;
        //vec.scale(scalar);
        //return vec;
    }

    inline TVec3f createVecAndScaleByAndAdd(const TVec3f &rSrc, const TVec3f &rOtherVec) {
        TVec3f vec(rSrc);
        vec.scaleInline(120.0f);
        
        TVec3f other_vec(rOtherVec);
        other_vec.addInline3(vec);
        return other_vec;
    }

    inline TVec3f createVecAndScaleByAndAdd_2(const TVec3f &rSrc, const TVec3f &rOtherVec) {
        TVec3f vec(rSrc);
        vec.scaleInline(120.0f);
        
        TVec3f other_vec(rOtherVec);
        other_vec.addInline_5(vec);
        return other_vec;
    }

    inline TVec3f negateVector(TVec3f &rVec) {
        return -rVec;
    }

    inline void createVecScaleAndApply(const TVec3f &rSrc, TVec3f &rDest, f32 scalar) {
        rDest += createVecAndScale(rSrc, scalar);
    }

    inline TVec3f addTwoScalarVecs(const TVec3f &rSrcVector_One, const TVec3f &rSrcVector_Two, f32 scalar_one, f32 scalar_two) {
        TVec3f vec_one(rSrcVector_One);
        vec_one.scale(scalar_one);
        f32 scale_2 = scalar_two;
        TVec3f vec_two(rSrcVector_Two);
        vec_two.scale(scale_2);
        TVec3f res(vec_two);
        res += vec_one;
        return res;
    }

    inline TVec3f multVec(const TVec3f &rSrc, f32 mult, f32 mult_2) {
        TVec3f vec_one(rSrc);
        vec_one *= mult;
        TVec3f vec_two(vec_one);
        vec_two *= mult_2;
        return vec_two;
    }

    inline TVec3f multVec(const TVec3f &rSrc, f32 mult) {
        TVec3f vec(rSrc);
        vec *= mult;
        return vec;
    }

    inline TVec3f multAndAddVec(const TVec3f &rSrc, const TVec3f &rSrc_2, f32 mult, f32 mult_2) {
        TVec3f vec_one(rSrc);
        vec_one *= mult;
        TVec3f vec_two(rSrc_2);
        vec_two *= mult_2;
        TVec3f ret(vec_two);
        ret += vec_one;
        return ret;
    }

    inline f32 subtractFromSum(f32 lhs, f32 rhs, f32 sub) {
        return (rhs + lhs) - sub;
    }

    inline f32 divideFromDiff(f32 lhs, f32 rhs, f32 div) {
        return (lhs - rhs) / div;
    }

    inline f32 modAndAdd(f32 a1, f32 a2) {
        return a1 + (f32)fmod(360.0f + a2, 360.0f);
    }

    inline f32 modAndAdd(f32 a1, f32 a2, f32 a3) {
        return a1 + (f32)fmod(a3 + a2, a3);
    }

    inline f32 add(f32 lhs, f32 rhs) {
        return lhs + rhs;
    }

    inline f32 negFloat(f32 val) {
        return (val < 0.0f) ? -val : val;
    }

    inline s32 mod(s32 val, s32 mod) {
        return val % mod;
    }

    f32 repeat(f32 a1, f32 a2, f32 a3) {
        f64 mod = fmod(a3 + (a1 - a2), a3);
        return mod + a2;
    }

    inline void subtractAndSet(TVec3f &a1, const TVec3f &a2, const TVec3f *a3) {
        a1.set(a2 - *a3);
    }

    inline void multAndSet(TVec3f *a1, const TVec3f &a2, f32 a3) {
        a1->set<f32>(a2 * a3);
    }

    inline int getRemainder(int ra, int rb) {
        int quotient = ra / rb;
        quotient *= rb;
        int remainder = ra - quotient;
        return remainder;
    }

    f32 cos(f32);
    f32 sin(f32);
};

f32 PSVECKillElement(const Vec *, const Vec *, const Vec *);