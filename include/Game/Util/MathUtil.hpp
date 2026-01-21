#pragma once

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

    /// @brief Returns a number representing the sign of a number.
    /// @param x The number to evaluate.
    /// @retval `1.0f` if the number is positive.
    /// @retval `-1.0f` if the number is negative.
    /// @retval `0.0f` if the number is zero.
    f32 sign(f32 x);

    void clampVecAngleDeg(TVec3f*, const TVec3f&, f32);
    void clampLength(TVec3f*, const TVec3f&, f32);
    f32 convergeRadian(f32, f32, f32);
    bool isInRange(f32, f32, f32);
    f32 calcRotateY(f32, f32);
    f32 calcRotateZ(const TVec3f&, const TVec3f&);

    /// @brief Computes the distance between two points.
    /// @param[in] rPos1 A reference to the position of the first point.
    /// @param[in] rPos2 A reference to the position of the second point.
    /// @return The distance between the points.
    f32 calcDistanceXY(const TVec3f& rPos1, const TVec3f& rPos2);

    void rotateVecDegree(f32*, f32*, f32);
    void rotateVecDegree(TVec2f*, f32);
    void rotateVecDegree(TVec3f*, const TVec3f&, f32);
    void rotateVecDegree(TVec3f*, const TVec3f&, const TVec3f&, f32);
    void rotateVecRadian(TVec3f*, const TVec3f&, f32);
    void rotateVecRadian(TVec3f*, const TVec3f&, const TVec3f&, f32);
    void calcLocalVec(TVec3f*, MtxPtr);

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

    f32 normalizeAbs(f32, f32, f32);
    bool turnVecToVecCos(TVec3f*, const TVec3f&, const TVec3f&, f32, const TVec3f&, f32);
    bool turnVecToVecCosOnPlane(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&, f32);
    bool turnVecToVecCosOnPlane(TVec3f*, const TVec3f&, const TVec3f&, f32);
    f32 turnVecToVecRadian(TVec3f*, const TVec3f&, const TVec3f&, f32, const TVec3f&);
    f32 turnVecToVecDegree(TVec3f*, const TVec3f&, const TVec3f&, f32, const TVec3f&);
    void calcMomentRollBall(TVec3f*, const TVec3f&, const TVec3f&, f32);
    bool calcReflectionVector(TVec3f*, const TVec3f&, f32, f32);

    /// @brief Determines if a three-dimensional vector is approximately parallel to another vector.
    /// @param[in] rVec1 A reference to the first three-dimensional vector to evaluate.
    /// @param[in] rVec2 A reference to the second three-dimensional vector to evaluate.
    /// @param tolerance The maximum allowed difference from zero.
    bool isSameDirection(const TVec3f&, const TVec3f&, f32 tolerance);

    /// @brief Determines if a three-dimensional vector is not angled acutely or parallel to another vector.
    /// @param[in] rVec1 A reference to the first three-dimensional vector to evaluate.
    /// @param[in] rVec2 A reference to the second three-dimensional vector to evaluate.
    /// @param tolerance The maximum allowed difference from zero.
    bool isOppositeDirection(const TVec3f& rVec1, const TVec3f& rVec2, f32 tolerance);

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

    f32 diffAngleAbs(f32, f32);
    f32 normalizeAngleAbs(f32);
    bool isAngleBetween(f32, f32, f32);
    f32 blendAngle(f32, f32, f32);

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

    void sortSmall(s32, f32*, s32*);
    void sortSmall(s32, u32*, s32*);
    f32 vecKillElement(const TVec3f&, const TVec3f&, TVec3f*);
    void vecScaleAdd(const TVec3f*, const TVec3f*, f32);
    void PSvecBlend(const TVec3f*, const TVec3f*, TVec3f*, f32, f32);
    void vecBlend(const TVec3f&, const TVec3f&, TVec3f*, f32);
    void vecBlendNormal(const TVec3f&, const TVec3f&, TVec3f*, f32);
    bool vecBlendSphere(const TVec3f&, const TVec3f&, TVec3f*, f32);
    void vecRotAxis(const TVec3f&, const TVec3f&, const TVec3f&, TVec3f*, f32);

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

    /// @brief Determines if a three-dimensional vector contains a magnitude approximately equal to one.
    /// @param[in] rVec A reference to the three-dimensional vector to evaluate.
    /// @param tolerance The maximum allowed difference from zero.
    bool isNormalize(const TVec3f& rVec, f32 tolerance);

    /// @brief Initializes a three-dimensional vector with elements equal to NaN.
    /// @param[out] rDst A reference to the three-dimensional vector to initialize.
    void setNan(TVec3f& rDst);

    /// @brief Determines if the elements of a three-dimensional vector are equal to NaN.
    /// @param[in] rVec A reference to the three-dimensional vector to evaluate.
    /// @return `true` if the elements are equal to NaN, `false` otherwise.
    bool isNan(const TVec3f& rVec);

    f32 getFootPoint(const TVec3f&, const TVec3f&, const TVec3f&, TVec3f*);

    /// @brief Computes the remainder of floating-point division.
    /// @param x The dividend.
    /// @param y The divisor.
    /// @return The remainder of the division.
    f32 mod(f32 x, f32 y);

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

    void getRotatedAxisY(TVec3f*, const TVec3f&);
    void getRotatedAxisZ(TVec3f*, const TVec3f&);

    template < typename T >
    T converge(T, T, T);

    /// @brief Computes the cosine of a number, in radians.
    /// @param x The number of radians to evaluate.
    /// @return The ratio of the length of the adjacent to that of the hypotenuse.
    f32 cos(f32 x);

    /// @brief Computes the sine of a number, in radians.
    /// @param x The number of radians to evaluate.
    /// @return The ratio of the length of the opposite to that of the hypotenuse.
    f32 sin(f32 x);

    /// @brief Computes the cosine of a number, in degrees.
    /// @param x The number of degrees to evaluate.
    /// @return The ratio of the length of the adjacent to that of the hypotenuse.
    f32 cosDegree(f32 x);

    /// @brief Computes the sine of a number, in degrees.
    /// @param x The number of degrees to evaluate.
    /// @return The ratio of the length of the opposite to that of the hypotenuse.
    f32 sinDegree(f32 x);

    /// @brief Compares two numbers for the smallest value.
    /// @param a The first number to evaluate.
    /// @param b The second number to evaluate.
    /// @retval `a` if less than `b`.
    /// @retval `b` if less than or equal to `a`.
    f32 min(f32 a, f32 b);

    /// @brief Compares two numbers for the largest value.
    /// @param a The first number to evaluate.
    /// @param b The second number to evaluate.
    /// @retval `a` if greater than or equal to `b`.
    /// @retval `b` if greater than `a`.
    f32 max(f32 a, f32 b);

    /// @brief Restricts a number to an interval.
    /// @param x The number to evaluate.
    /// @param min The lower bound of the interval.
    /// @param max The upper bound of the interval.
    /// @retval `min` if the number is less than or equal to `min`.
    /// @retval `max` if the number is greater than or equal to `max`.
    /// @retval `x` if the number is greater than `min` and less than `max`.
    f32 clamp(f32 x, f32 min, f32 max);

    /// @brief Restricts an integer to an interval.
    /// @param x The integer to evaluate.
    /// @param min The lower bound of the interval.
    /// @param max The upper bound of the interval.
    /// @retval `min` if the integer is less than or equal to `min`.
    /// @retval `max` if the integer is greater than or equal to `max`.
    /// @retval `x` if the integer is greater than `min` and less than `max`.
    s32 clamp(s32 x, s32 min, s32 max);

    /// @brief Restricts a number to the unit interval.
    /// @param[in,out] pX A pointer to the number to evaluate and initialize.
    void clamp01(f32* pX);  // { *pX = *pX < 0.0f ? 0.0f : *pX > 1.0f ? 1.0f : *pX; }

    void clampBoth(f32* value, f32 min, f32 max);

    inline f32 repeat(f32 value, f32 min, f32 max) { return min + (f32)fmod(max + (value - min), max); }

    inline void repeatDegree(f32* value) { *value = repeat(*value, 0.0f, 360.0f); }

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
