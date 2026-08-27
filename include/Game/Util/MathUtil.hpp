#pragma once

#include "JSystem/JGeometry/TUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TQuat.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

namespace MR {
    /// @brief Initializes the precomputed arccosine table.
    void initAcosTable();

    /// @brief Returns the arccosine of a number from the precomputed arccosine table.
    /// @param x The trigonometric ratio to evaluate.
    /// @return The arccosine of the number, in radians.
    f32 acosEx(f32 x);

    /// @brief Computes the next pseudorandom floating-point number within the right-open unit interval.
    /// @return The pseudorandom floating-point number.
    f32 getRandom();

    /// @brief Computes the next pseudorandom floating-point number within the half-open interval `[min, max)`.
    /// @param min The inclusive minimum number.
    /// @param max The exclusive maximum number.
    /// @return The pseudorandom floating-point number.
    f32 getRandom(f32 min, f32 max);

    /// @brief Computes the next pseudorandom integer within the half-open interval `[min, max)`.
    /// @param min The inclusive minimum integer.
    /// @param max The exclusive maximum integer.
    /// @return The pseudorandom integer.
    s32 getRandom(s32 min, s32 max);

    /// @brief Computes the next pseudorandom floating-point number within the half-open interval `[0.0f, 360.0f)`.
    /// @return The pseudorandom number of degrees.
    f32 getRandomDegree();

    /// @brief Initializes a three-dimensional vector with pseudorandom elements within the half-open interval `[min, max)`.
    /// @param[out] pDst A pointer to the three-dimensional vector to initialize.
    /// @param min The inclusive minimum number.
    /// @param min The exclusive maximum number.
    void calcRandomVec(TVec3f* pDst, f32 min, f32 max);

    /// @brief Computes the next pseudorandom Boolean.
    /// @return `true` if the pseudorandom number is less than one half, `false` otherwise.
    bool isHalfProbability() NO_INLINE;

    /// @brief Computes the sign of the next pseudorandom Boolean.
    /// @retval `-1.0f` if the pseudorandom number is less than one half.
    /// @retval `1.0f` if the pseudorandom number is greater than or equal to one half.
    f32 getSignHalfProbability();

    /// @brief Initializes a three-dimensional vector with pseudorandom elements within the half-open interval `[-range, range)`.
    /// @param[out] pDst A pointer to the three-dimensional vector to initialize.
    /// @param range The inclusive negative number and the exclusive positive number.
    void getRandomVector(TVec3f* pDst, f32 range);

    /// @brief Computes vector addition with a pseudorandom three-dimensional vector.
    /// @param[out] pDst A pointer to the three-dimensional vector to initialize.
    /// @param[in] rSrc A reference to the three-dimensional base vector.
    /// @param range The inclusive negative number and the exclusive positive number.
    void addRandomVector(TVec3f* pDst, const TVec3f& rSrc, f32 range);

    /// @brief Computes vector addition with a pseudorandom three-dimensional vector while maintaining the original length.
    /// @param[out] pDst A pointer to the three-dimensional vector to initialize.
    /// @param[in] rSrc A reference to the three-dimensional base vector.
    /// @param range The inclusive negative number and the exclusive positive number.
    void turnRandomVector(TVec3f* pDst, const TVec3f& rSrc, f32 range);

    /// @brief Computes unclamped linear interpolation between two numbers.
    /// @param t The linear interpolant.
    /// @param start The starting number.
    /// @param end The ending number.
    /// @return The result of linear interpolation.
    f32 getInterpolateValue(f32 t, f32 start, f32 end);

    /// @brief Maps a number from the interval `[0.0f, max]` to the interval `[start, end]`.
    /// @param x The number to evaluate.
    /// @param start The lower bound of the new interval.
    /// @param end The upper bound of the new interval.
    /// @param max The upper bound of the old interval.
    /// @return The mapped number.
    f32 getLinerValue(f32 x, f32 start, f32 end, f32 max);

    /// @brief Maps a number from the interval `[min, max]` to the interval `[start, end]`.
    /// @param x The number to evaluate.
    /// @param min The lower bound of the old interval.
    /// @param max The upper bound of the old interval.
    /// @param start The lower bound of the new interval.
    /// @param end The upper bound of the new interval.
    /// @return The mapped number.
    f32 getLinerValueFromMinMax(f32 x, f32 min, f32 max, f32 start, f32 end);

    /// @brief Maps a number from the interval `[0.0f, max]` to the interval `[start, end]`, fit along an ease-in sine curve.
    /// @param x The number to evaluate.
    /// @param start The lower bound of the new interval.
    /// @param end The upper bound of the new interval.
    /// @param max The upper bound of the old interval.
    /// @return The fitted mapped number.
    f32 getEaseInValue(f32 x, f32 start, f32 end, f32 max);

    /// @brief Maps a number from the interval `[0.0f, max]` to the interval `[start, end]`, fit along an ease-out sine curve.
    /// @param x The number to evaluate.
    /// @param start The lower bound of the new interval.
    /// @param end The upper bound of the new interval.
    /// @param max The upper bound of the old interval.
    /// @return The fitted mapped number.
    f32 getEaseOutValue(f32 x, f32 start, f32 end, f32 max);

    /// @brief Maps a number from the interval `[0.0f, max]` to the interval `[start, end]`, fit along an ease-in-out sine curve.
    /// @param x The number to evaluate.
    /// @param start The lower bound of the new interval.
    /// @param end The upper bound of the new interval.
    /// @param max The upper bound of the old interval.
    /// @return The fitted mapped number.
    f32 getEaseInOutValue(f32 x, f32 start, f32 end, f32 max);

    f32 getScaleWithReactionValueZeroToOne(f32 x, f32 rate, f32 amplitude);
    f32 getConvergeVibrationValue(f32 x, f32 start, f32 end, f32 dampScale, f32 rate);
    f32 getReduceVibrationValue(f32 x, f32 time, f32 base, f32 amplitude, f32 freq);
    void makeAxisFrontUp(TVec3f* pSide, TVec3f* pUp, const TVec3f& rFront, const TVec3f& rUp);
    void makeAxisFrontSide(TVec3f* pUp, TVec3f* pSide, const TVec3f& rFront, const TVec3f& rSide);
    void makeAxisUpFront(TVec3f* pSide, TVec3f* pFront, const TVec3f& rUp, const TVec3f& rFront);
    void makeAxisUpSide(TVec3f* pFront, TVec3f* pSide, const TVec3f& rUp, const TVec3f& rSide);

    /*
     * Generate an orthogonal vector to the second argument, starting by projecting the z-vector
     * into the plane orthogonal to the second argument. If the z-vector is parallel to the second
     * argument, the x-vector is instead projected into the orthognal plane. Regardless, the
     * normalized result is placed into the first argument.
     */
    void makeAxisVerticalZX(TVec3f* pVec, const TVec3f& rAxis);

    void makeAxisCrossPlane(TVec3f* pVec, TVec3f* pCross, const TVec3f& rNorm);
    bool makeAxisAndCosignVecToVec(TVec3f* pAxis, f32* pCos, const TVec3f& rFrom, const TVec3f& rTo);

    /*
     * Projects rPoint onto the directed line defined by rPointA and rPointB and places the result into pDst
     */
    f32 calcPerpendicFootToLine(TVec3f* pDst, const TVec3f& rPos, const TVec3f& rPointA, const TVec3f& rPointB);

    /*
     * Same as above, except the result of the projection is clamped between rPointA and rPointB
     */
    f32 calcPerpendicFootToLineInside(TVec3f* pDst, const TVec3f& rPos, const TVec3f& rPointA, const TVec3f& rPointB);

    bool checkHitSegmentSphere(const TVec3f& rSpherePos, const TVec3f& rPointA, const TVec3f& rPointB, f32 radius, TVec3f* pDir);
    bool checkHitSemilinePlane(TVec3f* pHitPos, const TVec3f& rPos, const TVec3f& rDir, const TVec3f& rPlanePos, const TVec3f& rPlaneNorm);
    bool calcReboundVelocity(TVec3f* pVel, const TVec3f& rNormal, f32 reboundRate);
    bool calcReboundVelocity(TVec3f* pVel, const TVec3f& rNormal, f32 reboundRateV, f32 reboundRateH);
    void calcParabolicFunctionParam(f32* pAccel, f32* pVel, f32 max, f32 end);

    void makeQuatRotateRadian(TQuat4f* pQuat, const TVec3f& rRot);
    void makeQuatRotateDegree(TQuat4f* pQuat, const TVec3f& rRot);
    void makeQuatFromVec(TQuat4f* pQuat, const TVec3f& rFront, const TVec3f& rUp);
    void makeQuatSideUp(TQuat4f* pParam1, const TVec3f& rParam2, const TVec3f& rParam3);
    void makeQuatUpFront(TQuat4f* pQuat, const TVec3f& rUp, const TVec3f& rFront);
    void makeQuatUpNoSupport(TQuat4f* pQuat, const TVec3f& rUp);
    void blendQuatUpFront(TQuat4f* pDst, const TVec3f& rUp, const TVec3f& rFront, f32 upRate, f32 frontRate);
    void blendQuatUpFront(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rUp, const TVec3f& rFront, f32 upRate, f32 frontRate);
    void blendQuatFrontUp(TQuat4f* pDst, const TVec3f& rFront, const TVec3f& rUp, f32 upRate, f32 frontRate);
    void blendQuatFrontUp(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rFront, const TVec3f& rUp, f32 frontRate, f32 upRate);
    void rotateQuatMoment(TQuat4f* pQuat, const TVec3f& rMoment);
    void rotateQuatRollBall(TQuat4f* pQuat, const TVec3f& rVel, const TVec3f& rNorm, f32 radius);
    bool turnQuat(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rFrom, const TVec3f& rTo, f32 angle);
    bool turnQuatXDirRad(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 angle);
    void turnQuatXDirRate(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 rate);
    bool turnQuatYDirRad(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 angle);
    void turnQuatYDirRate(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 rate);
    bool turnQuatZDirRad(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 angle);
    void turnQuatZDirRate(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 rate);

    /// @brief Returns a number representing the sign of a number.
    /// @param x The number to evaluate.
    /// @retval `1.0f` if the number is positive.
    /// @retval `-1.0f` if the number is negative.
    /// @retval `0.0f` if the number is zero.
    f32 sign(f32 x);

    void clampVecAngleDeg(TVec3f* pDst, const TVec3f& rBase, f32 angle);
    void clampLength(TVec3f* pDst, const TVec3f& rSrc, f32 length);
    f32 convergeRadian(f32 angle, f32 target, f32 speed);
    bool isInRange(f32 x, f32 min, f32 max);
    f32 calcRotateY(f32 x, f32 z);
    f32 calcRotateZ(const TVec3f& rVecA, const TVec3f& rVecB);

    inline f32 toDegree(f32 angle) {
        f32 cnv = 180.0f / PI;
        return angle * cnv;
    }

    inline f32 toRadian(f32 angle) {
        return angle * PI_180;
    }

    /// @brief Computes the distance between two points.
    /// @param[in] rPos1 A reference to the position of the first point.
    /// @param[in] rPos2 A reference to the position of the second point.
    /// @return The distance between the points.
    f32 calcDistanceXY(const TVec3f& rPos1, const TVec3f& rPos2);

    void rotateVecDegree(f32* pX, f32* pY, f32 degree);
    void rotateVecDegree(TVec2f* pDst, f32 degree);
    void rotateVecDegree(TVec3f* pDst, const TVec3f& rAxis, f32 degree);
    void rotateVecDegree(TVec3f* pDst, const TVec3f& rSrc, const TVec3f& rAxis, f32 degree);
    void rotateVecRadian(TVec3f* pDst, const TVec3f& rAxis, f32 angle);
    void rotateVecRadian(TVec3f* pDst, const TVec3f& rSrc, const TVec3f& rAxis, f32 angle);
    void calcLocalVec(TVec3f* pVec, MtxPtr pMtx);

    /// @brief Converts a two-dimensional vector into a vector of unit length.
    /// @param[in,out] pVec A pointer to the two-dimensional vector to evaluate and initialize.
    void normalize(TVec2f* pVec);

    /// @brief Converts a three-dimensional vector into a vector of unit length.
    /// @param[in,out] pVec A pointer to the three-dimensional vector to evaluate and initialize.
    void normalize(TVec3f* pVec);

    /// @brief Converts the column vectors in a 3×4 matrix into vectors of unit length.
    /// @param[in,out] pMtx A pointer to the 3×4 matrix to evaluate and initialize.
    void normalize(TPos3f* pMtx);

    /// @brief Converts a two-dimensional vector into a vector of unit length.
    /// @param[in] rSrc A reference to the two-dimensional vector to evaluate.
    /// @param[out] pDst A pointer to the two-dimensional vector to initialize.
    void normalize(const TVec2f& rSrc, TVec2f* pDst);

    /// @brief Converts a three-dimensional vector into a vector of unit length.
    /// @param[in] rSrc A reference to the three-dimensional vector to evaluate.
    /// @param[out] pDst A pointer to the three-dimensional vector to initialize.
    void normalize(const TVec3f& rSrc, TVec3f* pDst);

    /// @brief Converts a two-dimensional vector into a vector of unit length.
    /// @param[in,out] pVec A pointer to the two-dimensional vector to evaluate and initialize.
    /// @return `true` if the vector could not be converted and was initialized with the zero vector, `false` otherwise.
    bool normalizeOrZero(TVec2f* pVec);

    /// @brief Converts a three-dimensional vector into a vector of unit length.
    /// @param[in,out] pVec A pointer to the three-dimensional vector to evaluate and initialize.
    /// @return `true` if the vector could not be converted and was initialized with the zero vector, `false` otherwise.
    bool normalizeOrZero(TVec3f* pVec);

    /// @brief Converts a three-dimensional vector into a vector of unit length.
    /// @param[in] rSrc A reference to the three-dimensional vector to evaluate.
    /// @param[out] pDst A pointer to the three-dimensional vector to initialize.
    /// @return `true` if the vector could not be converted and was initialized with the zero vector, `false` otherwise.
    bool normalizeOrZero(const TVec3f& rSrc, TVec3f* pDst);

    /// @brief Extracts the magnitude and direction from a two-dimensional vector.
    /// @param[out] pScalar A pointer to the magnitude to initialize.
    /// @param[out] pDir A pointer to the two-dimensional directional vector to initialize.
    /// @param[in] rVec A reference to the two-dimensional vector to evaluate.
    void separateScalarAndDirection(f32* pScalar, TVec2f* pDir, const TVec2f& rVec);

    /// @brief Extracts the magnitude and direction from a three-dimensional vector.
    /// @param[out] pScalar A pointer to the magnitude to initialize.
    /// @param[out] pDir A pointer to the three-dimensional directional vector to initialize.
    /// @param[in] rVec A reference to the three-dimensional vector to evaluate.
    void separateScalarAndDirection(f32* pScalar, TVec3f* pDir, const TVec3f& rVec);

    /// @brief Maps a number to the unit interval.
    /// @param x The number to evaluate.
    /// @param min The lower bound of the old interval.
    /// @param max The upper bound of the old interval.
    /// @return The mapped number.
    f32 normalize(f32 x, f32 min, f32 max);

    f32 normalizeAbs(f32 x, f32 min, f32 max);

    bool turnVecToVecCos(TVec3f* pDst, const TVec3f& rFrom, const TVec3f& rTo, f32 cosAngle, const TVec3f& rAxis, f32 fallbackLength = 0.02f);

    bool turnVecToVecCosOnPlane(TVec3f* pDst, const TVec3f& rFrom, const TVec3f& rTo, const TVec3f& rAxis, f32 cosAngle);
    bool turnVecToVecCosOnPlane(TVec3f* pVec, const TVec3f& rTo, const TVec3f& rAxis, f32 cosAngle);
    f32 turnVecToVecRadian(TVec3f* pDst, const TVec3f& rFrom, const TVec3f& rTo, f32 angle, const TVec3f& rAxis);
    f32 turnVecToVecDegree(TVec3f* pDst, const TVec3f& rFrom, const TVec3f& rTo, f32 angle, const TVec3f& rAxis);
    void calcMomentRollBall(TVec3f* pMoment, const TVec3f& rVel, const TVec3f& rUp, f32 radius);
    bool calcReflectionVector(TVec3f* pDir, const TVec3f& rNorm, f32 rate, f32 cosAngleMin);

    /// @brief Determines if a three-dimensional vector is approximately parallel to another vector.
    /// @param[in] rVec1 A reference to the first three-dimensional vector to evaluate.
    /// @param[in] rVec2 A reference to the second three-dimensional vector to evaluate.
    /// @param tolerance The maximum allowed difference from zero.
    bool isSameDirection(const TVec3f& rVec1, const TVec3f& rVec2, f32 tolerance = 0.01f);

    /// @brief Determines if a three-dimensional vector is not angled acutely or parallel to another vector.
    /// @param[in] rVec1 A reference to the first three-dimensional vector to evaluate.
    /// @param[in] rVec2 A reference to the second three-dimensional vector to evaluate.
    /// @param tolerance The maximum allowed difference from zero.
    bool isOppositeDirection(const TVec3f& rVec1, const TVec3f& rVec2, f32 tolerance = 0.01f);

    /// @brief Determines if a number is approximately equal to zero.
    /// @param x The number to evaluate.
    /// @param tolerance The maximum allowed difference from zero.
    /// @return `true` if the number is approximately equal to zero, `false` otherwise.
    bool isNearZero(f32 x, f32 tolerance = 0.001f);

    /// @brief Determines if the elements of a three-dimensional vector are all approximately equal to zero.
    /// @param[in] rVec A reference to the three-dimensional vector to evaluate.
    /// @param tolerance The maximum allowed difference from zero.
    /// @return `true` if the elements are approximately equal to zero, `false` otherwise.
    bool isNearZero(const TVec3f& rVec, f32 tolerance = 0.001f);

    /// @brief Determines if the elements of a two-dimensional vector are all approximately equal to zero.
    /// @param[in] rVec A reference to the two-dimensional vector to evaluate.
    /// @param tolerance The maximum allowed difference from zero.
    /// @return `true` if the elements are approximately equal to zero, `false` otherwise.
    bool isNearZero(const TVec2f& rVec, f32 tolerance = 0.001f);

    f32 diffAngleAbs(f32 angle1, f32 angle2);
    f32 normalizeAngleAbs(f32 angle);
    bool isAngleBetween(f32 angle, f32 min, f32 max);
    f32 blendAngle(f32 angleA, f32 angleB, f32 rate);

    /// @brief Computes clamped linear interpolation between two integers.
    /// @param start The starting integer.
    /// @param end The ending integer.
    /// @param t The linear interpolant.
    /// @return The result of linear interpolation.
    u8 lerp(u8 start, u8 end, f32 t);

    /// @brief Computes clamped linear interpolation between two colors.
    /// @param start The starting color.
    /// @param end The ending color.
    /// @param t The linear interpolant.
    /// @return The result of linear interpolation.
    GXColor lerp(GXColor start, GXColor end, f32 t);

    void sortSmall(s32 length, f32* sortArray, s32* indexArray);
    void sortSmall(s32 length, u32* sortArray, s32* indexArray);

    f32 vecKillElement(const TVec3f& rSrc, const TVec3f& rKillDir, TVec3f* pDst);
    void vecScaleAdd(const TVec3f* pScale, const TVec3f* pAdd, f32 scale);
    void PSvecBlend(const TVec3f* pFrom, const TVec3f* pTo, TVec3f* pDst, f32 invRate, f32 rate);
    void vecBlend(const TVec3f& rFrom, const TVec3f& rTo, TVec3f* pDst, f32 rate);
    void vecBlendNormal(const TVec3f& rFrom, const TVec3f& rTo, TVec3f* pVec, f32 rate);
    bool vecBlendSphere(const TVec3f& rFrom, const TVec3f& rTo, TVec3f* pVec, f32 rate);
    void vecRotAxis(const TVec3f& rFrom, const TVec3f& rTo, const TVec3f& rAxis, TVec3f* pVec, f32 angle);

    /// @brief Computes wrapping linear interpolation between two colors.
    /// @param[out] pDst A pointer to the color to initialize.
    /// @param[in] rStart A reference to the starting color.
    /// @param[in] rEnd A reference to the ending color.
    /// @param t The linear interpolant.
    void blendColor(GXColor* pDst, const GXColor& rStart, const GXColor& rEnd, f32 t);

    /// @brief Computes unclamped linear interpolation between two three-dimensional vectors.
    /// @param[out] pDst A pointer to the three-dimensional vector to initialize.
    /// @param[in] rStart A reference to the starting vector.
    /// @param[in] rEnd A reference to the ending vector.
    /// @param t The linear interpolant.
    void blendVec(Vec* pDst, const Vec& rStart, const Vec& rEnd, f32 t);

    f32 turnVecToPlane(TVec3f* pDst, const TVec3f& rSrc, const TVec3f& rNorm);
    u32 getMinAbsElementIndex(const TVec3f& rVec);
    f32 getMaxElement(const TVec3f& rVec);
    f32 getMaxAbsElement(const TVec3f& rVec);
    u32 getMaxElementIndex(const TVec3f& rVec);
    u32 getMaxAbsElementIndex(const TVec3f& rVec);
    f32 diffAngleAbsFast(const TVec3f& rA, const TVec3f& rB);
    f32 diffAngleAbs(const TVec3f& rA, const TVec3f& rB);
    f32 diffAngleAbs(const TVec2f& rA, const TVec2f& rB);
    f32 diffAngleSigned(const TVec3f& rA, const TVec3f& rB, const TVec3f& rAxis);
    f32 diffAngleAbsHorizontal(const TVec3f& rA, const TVec3f& rB, const TVec3f& rAxis);
    f32 diffAngleSignedHorizontal(const TVec3f& rA, const TVec3f& rB, const TVec3f& rAxis);
    bool isNearAngleRadian(const TVec3f& rA, const TVec3f& rB, f32 angle);
    bool isNearAngleDegree(const TVec3f& rA, const TVec3f& rB, f32 angle);
    bool isNearAngleRadianHV(const TVec3f& rA, const TVec3f& rB, const TVec3f& rAxis, f32 angleH, f32 angleV);
    bool isNearAngleDegreeHV(const TVec3f& rA, const TVec3f& rB, const TVec3f& rAxis, f32 angleH, f32 angleV);
    void createBoundingBox(const TVec3f* pPoints, u32 numPoints, TVec3f* pMin, TVec3f* pMax);

    /// @brief Determines if a three-dimensional vector contains a magnitude approximately equal to one.
    /// @param[in] rVec A reference to the three-dimensional vector to evaluate.
    /// @param tolerance The maximum allowed difference from zero.
    bool isNormalize(const TVec3f& rVec, f32 tolerance = 0.001f);

    /// @brief Initializes a three-dimensional vector with elements equal to NaN.
    /// @param[out] rDst A reference to the three-dimensional vector to initialize.
    void setNan(TVec3f& rDst);

    /// @brief Determines if the elements of a three-dimensional vector are equal to NaN.
    /// @param[in] rVec A reference to the three-dimensional vector to evaluate.
    /// @return `true` if the elements are equal to NaN, `false` otherwise.
    bool isNan(const TVec3f& rVec);

    f32 getFootPoint(const TVec3f& rPointA, const TVec3f& rPointB, const TVec3f& rPos, TVec3f* pDst);

    /// @brief Computes the remainder of floating-point division.
    /// @param x The dividend.
    /// @param y The divisor.
    /// @return The remainder of the division.
    f32 mod(f32 x, f32 y);

    /// @brief Computes the angle in radians mapped from [0, TWO_PI).
    /// @param angle The angle, in radians.
    /// @return The mapped angle.
    inline f32 modRadian(f32 angle) {
        return fmod(angle, TWO_PI);
    }

    inline bool isOppositeDirectionRadian(f32 angle, f32 target) {
        f32 modRad = MR::modRadian(angle - target + TWO_PI);
        return target + modRad < -HALF_PI || target + modRad > HALF_PI;
    }

    /// @brief Converts a three-dimensional floating-point vector into a fixed-point vector.
    /// @param[out] pDst A pointer to the three-dimensional fixed-point vector to initialize.
    /// @param[in] rSrc A reference to the three-dimensional floating-point vector to evaluate.
    /// @param q The number of bits to reserve for the decimal portion.
    void floatToFixed16(TVec3s* pDst, const TVec3f& rSrc, u8 q);

    /// @brief Converts a three-dimensional fixed-point vector into a floating-point vector.
    /// @param[out] pDst A pointer to the three-dimensional floating-point vector to initialize.
    /// @param[in] rSrc A reference to the three-dimensional fixed-point vector to evaluate.
    /// @param q The number of bits reserved for the decimal portion.
    void fixed16ToFloat(TVec3f* pDst, const TVec3s& rSrc, u8 q);

    void getRotatedAxisY(TVec3f* pDst, const TVec3f& pSrc);
    void getRotatedAxisZ(TVec3f* pDst, const TVec3f& pSrc);

    template < typename T >
    T converge(T current, T target, T step) {
        if (current < target) {
            current += step;
            return current > target ? target : current;
        } else {
            current -= step;
            return current < target ? target : current;
        }
    }

    /// @brief Returns the value of pi (3.14159274f)
    inline f32 pi() {
        return PI;  // TODO: test if actually JGeometry::TUtil<f32>::PI();
    }

    /// @brief Returns the value of pi * 2 (6.2831855f)
    inline f32 pi2() {
        return PI * 2.0f;  // TODO: test if actually JGeometry::TUtil<f32>::PI() * 2.0f;
    }

    inline f32 epsilon() {
        return JGeometry::TUtil< f32 >::epsilon();
    }

    inline f32 abs(f32 x) {
        return __fabsf(x);
    }

    inline s32 abs(s32 x) {
        return __abs(x);
    }

    /// @brief Computes the cosine of a number, in radians.
    /// @param x The number of radians to evaluate.
    /// @return The ratio of the length of the adjacent to that of the hypotenuse.
    inline f32 cos(f32 x) {
        return JMACosRadian(x);
    }

    /// @brief Computes the sine of a number, in radians.
    /// @param x The number of radians to evaluate.
    /// @return The ratio of the length of the opposite to that of the hypotenuse.
    inline f32 sin(f32 x) {
        return JMASinRadian(x);
    }

    /// @brief Computes the tangent of a number, in radians.
    /// @param x The number of radians to evaluate.
    /// @return The ratio of the length of the opposite to that of the adjacent.
    inline f32 tan(f32 x) {
        return JMASinRadian(x) / JMACosRadian(x);
    }

    /// @brief Computes the arccos of a number in radians.
    /// @param x The ratio to evaluate.
    /// @return The angle in radians such that cos(angle) = x.
    inline f32 acos(f32 x) {
        return JMAAcosRadian(x);
    }

    /// @brief Computes the arcsin of a number in radians.
    /// @param x The ratio to evaluate.
    /// @return The angle in radians such that sin(angle) = x.
    inline f32 asin(f32 x) {
        return JMAAsinRadian(x);
    }

    /// @brief Computes the angle of the vector given by (x,y), in radians.
    /// @param x The x coordinate of input vector
    /// @param y The y coordinate of input vector
    /// @return The heading angle of the vector given by (x,y).
    inline f32 atan2(f32 x, f32 y) {
        return JMAATan2(x, y);
    }

    /// @brief Computes the cosine of a number, in degrees.
    /// @param x The number of degrees to evaluate.
    /// @return The ratio of the length of the adjacent to that of the hypotenuse.
    inline f32 cosDegree(f32 x) {
        return JMACosDegree(x);
    }

    /// @brief Computes the sine of a number, in degrees.
    /// @param x The number of degrees to evaluate.
    /// @return The ratio of the length of the opposite to that of the hypotenuse.
    inline f32 sinDegree(f32 x) {
        return JMASinDegree(x);
    }

    /// @brief Computes the tangent of a number, in degrees.
    /// @param x The number of degrees to evaluate.
    /// @return The ratio of the length of the opposite to that of the adjacent.
    inline f32 tanDegree(f32 x) {
        return JMASinDegree(x) / JMACosDegree(x);
    }

    /// @brief Compares two numbers for the smallest value.
    /// @param a The first number to evaluate.
    /// @param b The second number to evaluate.
    /// @retval `a` if less than `b`.
    /// @retval `b` if less than or equal to `a`.
    inline f32 min(f32 a, f32 b) {
        a = a >= b ? b : a;
        return a;
    }

    inline s32 min(s32 a, s32 b) {
        a = a >= b ? b : a;
        return a;
    }

    /// @brief Compares two numbers for the largest value.
    /// @param a The first number to evaluate.
    /// @param b The second number to evaluate.
    /// @retval `a` if greater than or equal to `b`.
    /// @retval `b` if greater than `a`.
    inline f32 max(f32 a, f32 b) {
        a = a >= b ? a : b;
        return a;
    }

    inline s32 max(s32 a, s32 b) {
        a = a >= b ? a : b;
        return a;
    }

    /// @brief Restricts a number to an interval.
    /// @param x The number to evaluate.
    /// @param min The lower bound of the interval.
    /// @param max The upper bound of the interval.
    /// @retval `min` if the number is less than or equal to `min`.
    /// @retval `max` if the number is greater than or equal to `max`.
    /// @retval `x` if the number is greater than `min` and less than `max`.
    inline f32 clamp(f32 x, f32 min, f32 max) {
        f32 ret;
        if (x < min) {
            ret = min;
        } else if (x > max) {
            ret = max;
        } else {
            ret = x;
        }

        return ret;
    }

    /// @brief Restricts an integer to an interval.
    /// @param x The integer to evaluate.
    /// @param min The lower bound of the interval.
    /// @param max The upper bound of the interval.
    /// @retval `min` if the integer is less than or equal to `min`.
    /// @retval `max` if the integer is greater than or equal to `max`.
    /// @retval `x` if the integer is greater than `min` and less than `max`.
    inline s32 clamp(s32 x, s32 min, s32 max) {
        s32 ret;
        if (x < min) {
            ret = min;
        } else if (x > max) {
            ret = max;
        } else {
            ret = x;
        }

        return ret;
    }

    /// @brief Restricts a number to the unit interval.
    /// @param[in,out] pX A pointer to the number to evaluate and initialize.
    inline void clamp01(f32* pX) {
        *pX = *pX < 0.0f ? 0.0f : *pX > 1.0f ? 1.0f : *pX;
    }

    inline f32 clamp01(f32 x) {
        if (x < 0.0f) {
            x = 0.0f;
        }
        if (x > 1.0f) {
            x = 1.0f;
        }
        return x;
    }

    inline void clampBoth(f32* value, f32 min, f32 max) {
        *value = clamp(*value, min, max);
    }

    inline void clampMax(f32* val, f32 max) {
        f32 ret;
        if (*val >= max) {
            ret = max;
        } else {
            ret = *val;
        }
        *val = ret;
    }

    inline f32 repeat(f32 value, f32 min, f32 max) {
        return min + (f32)fmod(max + (value - min), max);
    }

    inline void repeatDegree(f32* value) {
        *value = repeat(*value, 0.0f, 360.0f);
    }

    inline f32 repeatDegree(f32 value) {
        return repeat(value, 0.0f, 360.0f);
    }

    inline f32 negateIfLessZero(f32 val) {
        if (val < 0.0f) {
            val = -val;
        }

        return val;
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
            __asm { frsqrte recip, x}
            f32 v = recip * x;
            recip = -(v * recip - 3.0f);
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

    template < typename T >
    T sqrt(T x) {
        return fastSqrtf(x);
    }

};  // namespace MR

f32 PSVECKillElement(const Vec*, const Vec*, const Vec*);
f32 JMASqrt(f32);
