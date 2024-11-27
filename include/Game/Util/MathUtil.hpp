#pragma once

#include <cmath>
#include <revolution.h>
#include "JSystem/JGeometry.hpp"

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

    void blendQuatUpFront(TQuat4f *, const TVec3f &, const TVec3f &, float, float);

    void rotateQuatRollBall(TQuat4f *, const TVec3f &, const TVec3f &, f32);

    void clampLength(TVec3f *, const TVec3f &, f32);
    f32 convergeRadian(f32, f32, f32);
    bool isInRange(f32, f32, f32);

    f32 calcRotateY(f32, f32);

    f32 calcDistanceXY(const TVec3f &, const TVec3f &);

    void rotateVecDegree(TVec3f *, const TVec3f &, f32);

    bool isNearZero(f32, f32);
    bool isNearZero(const TVec3f &, f32);

    bool isSameDirection(const TVec3f &, const TVec3f &, float);
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
    s32 clamp(s32, s32, s32);

    f32 getScaleWithReactionValueZeroToOne(f32, f32, f32);

    f32 getConvergeVibrationValue(f32, f32, f32, f32, f32);

    void calcReboundVelocity(TVec3f *, const TVec3f &, f32, f32);

    void getRotatedAxisZ(TVec3f *, const TVec3f &);
    void getRotatedAxisY(TVec3f *, const TVec3f &);

    void turnVecToVecCosOnPlane(TVec3f *, const TVec3f &, const TVec3f &, f32);

    void turnVecToPlane(TVec3f *, const TVec3f &, const TVec3f &);

    f32 cosDegree(f32);
    f32 sinDegree(f32);

    // this must not be declared as inline. some callers inline it and some do not
    inline f32 max(f32 x, f32 y);

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

    inline TVec3f multVecNoCtor(const TVec3f& rSrc, f32 mult) {
        return rSrc * mult;
    }

    inline TVec3f addVec(const TVec3f& rSrc, const TVec3f& rBase) {
        TVec3f hurr(rBase);
        hurr.addInline6(rSrc);
        return hurr;
    }

    inline TVec3f subVec(const TVec3f& rSrc, const TVec3f& rBase) {
        TVec3f hurr(rBase);
        JMathInlineVEC::PSVECSubtract(hurr.toCVec(), rSrc.toCVec(), hurr.toVec());
        return hurr;
    }

    inline f32 subtractFromSum(f32 lhs, f32 rhs, f32 sub) {
        return (rhs + lhs) - sub;
    }

    inline f32 subtractFromSum_2(f32 lhs, f32 rhs, f32 sub) {
        return (180.0f + (rhs + lhs)) - sub;
    }    

    inline f32 subtractFromSum_3(f32 a1, f32 a2) {
        return a1 - a2;
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

    inline f32 modAndSubtract(f32 a1,  f32 a3, f32 a4) {
        f32 mod = fmod((a3 + (a1 - a4)), a3);
        return a4 + mod;
    }

    inline f32 modAndSubtract_2(f32 a1, f32 a2, f32 a3) {
        f32 mod = fmod((a3 + (a1 - a2)), a3);
        return a3 + mod;
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

    f32 repeat(f32 a1, f32 a2, f32 a3); /*{
        f64 mod = fmod(a3 + (a1 - a2), a3);
        return mod + a2;
    }*/

    inline void subtractAndSet(TVec3f &a1, const TVec3f &a2, const TVec3f *a3) {
        a1.set(a2 - *a3);
    }

    inline void multAndSet(TVec3f *a1, const TVec3f &a2, f32 a3) {
        a1->set(a2 * a3);
    }

    inline int getRemainder(int ra, int rb) {
        int quotient = ra / rb;
        quotient *= rb;
        int remainder = ra - quotient;
        return remainder;
    }

    inline f32 getZero() {
        return 0.0f;
    }

    inline bool isLessThan(f32 lhs, f32 rhs) {
        if (lhs < rhs) {
            return true;
        }
        
        return false;
    }

    inline f64 normalizePhase(f32 value, f32 initial, f32 offset, f32 modulus) {
        return (initial + fmod((offset + (value - initial)), modulus));
    }

    f32 cos(f32);
    f32 sin(f32);
};

f32 PSVECKillElement(const Vec *, const Vec *, const Vec *);
