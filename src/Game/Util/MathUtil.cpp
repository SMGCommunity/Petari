#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <JSystem/JGeometry/TUtil.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <JSystem/JMath/JMath.hpp>

namespace {
    static f32 sMinDegree = 0.0f;
    static f32 sMaxDegree = 360.0f;
};

namespace MR {
    f32 getRandom() {
        u32 rand = SingletonHolder<GameSystem>::get()->mObjHolder->mRandom.rand();
        u32 value = (rand >> 9) | 0x3F800000;

        return reinterpret_cast<f32&>(value) - 1.0f;
    }

    f32 getRandom(f32 min, f32 max) {
        return min + (max - min) * getRandom();
    }

    s32 getRandom(s32 min, s32 max) {
        return static_cast<f32>(min) + (static_cast<f32>(max) - static_cast<f32>(min)) * getRandom();
    }

    f32 getRandomDegree() {
        return sMinDegree + sMaxDegree * getRandom();
    }

    void calcRandomVec(TVec3f *pDst, f32 min, f32 max) {
        pDst->set<f32>(getRandom(min, max), getRandom(min, max), getRandom(min, max));
    }

    bool isHalfProbability() {
        return getRandom() < 0.5f;
    }

    f32 getSignHalfProbability() {
        if (isHalfProbability()) {
            return -1.0f;
        }
        else {
            return 1.0f;
        }
    }

    void getRandomVector(TVec3f* pDst, f32 range) {
        pDst->set<f32>(getRandom(-range, range), getRandom(-range, range), getRandom(-range, range));
    }

    #ifdef NON_MATCHING
    // stack places randVec and otherVec wrongly
    void addRandomVector(TVec3f *pOut, const TVec3f &rOtherVec, f32 a3) {
        f32 x = getRandom(-range, range);
        f32 y = getRandom(-range, range);
        f32 z = getRandom(-range, range);

        TVec3f randVec;
        randVec.x = x;
        randVec.y = y;
        randVec.z = z;

        TVec3f otherVec(rOtherVec);
        otherVec.add(randVec);
        pOut->set(otherVec);
    }
    #endif

    void turnRandomVector(TVec3f* pDst, const TVec3f& rSrc, f32 range) {
        f32 srcLength = PSVECMag(&rSrc);

        addRandomVector(pDst, rSrc, range);

        if (isNearZero(rSrc, 0.001f)) {
            pDst->set(rSrc);
        }
        else {
            pDst->setLength(srcLength);
        }
    }

    f32 getInterpolateValue(f32 t, f32 start, f32 end) {
        return start + (end - start) * t;
    }

    f32 getLinerValue(f32 x, f32 start, f32 end, f32 w) {
        return getInterpolateValue(x / w, start, end);
    }

    f32 getLinerValueFromMinMax(f32 x, f32 min, f32 max, f32 start, f32 end) {
        return getInterpolateValue((JGeometry::TUtil<f32>::clamp(x, min, max) - min) / (max - min), start, end);
    }

    // FIXME: Source registers swapped in multiplication by pi.
    f32 getEaseInValue(f32 x, f32 start, f32 end, f32 w) {
        return getInterpolateValue(1.0f - JMACosRadian(((x / w) * PI) / 2.0f), start, end);
    }

    // FIXME: Source registers swapped in multiplication by pi.
    f32 getEaseOutValue(f32 x, f32 start, f32 end, f32 w) {
        return getInterpolateValue(JMASinRadian(((x / w) * PI) / 2.0f), start, end);
    }

    // FIXME: Source registers swapped in multiplication by pi, and the instruction is swapped.
    f32 getEaseInOutValue(f32 x, f32 start, f32 end, f32 w) {
        return getInterpolateValue((1.0f - JMACosRadian((x / w) * PI)) / 2.0f, start, end);
    }

    // getScaleWithReactionValueZeroToOne
    // getConvergeVibrationValue
    // getReduceVibrationValue

    void makeAxisFrontUp(TVec3f *a1, TVec3f *a2, const TVec3f &a3, const TVec3f &a4) {
        PSVECCrossProduct((Vec*)&a4, (Vec*)&a3, (Vec*)a1);
        PSVECNormalize((Vec*)a1, (Vec*)a1);
        PSVECCrossProduct((Vec*)&a3, (Vec*)a1, (Vec*)a2);
    }

    // makeAxisFrontSide
    // makeAxisUpFront
    // makeAxisUpSide
    // makeAxisVerticalZX
    // makeAxisCrossPlane
    // makeAxisAndCosignVecToVec
    // calcPerpendicFootToLine
    // calcPerpendicFootToLineInside
    // checkHitSegmentSphere
    // checkHitSemilinePlane
    // calcReboundVelocity
    // calcReboundVelocity
    // calcParabolicFunctionParam
    // makeQuatRotateRadian
    // makeQuatRotateDegree
    // makeQuatFromVec
    // makeQuatSideUp
    // makeQuatUpFront
    // makeQuatUpNoSupport
    // blendQuatUpFront
    // blendQuatUpFront
    // blendQuatFrontUp
    // blendQuatFrontUp
    // rotateQuatMoment
    // rotateQuatRollBall
    // turnQuat
    // turnQuatXDirRad
    // turnQuatXDirRate
    // turnQuatYDirRad
    // turnQuatYDirRate
    // turnQuatZDirRad

    f32 sign(f32 x) {
        if (0.0f < x) {
            return 1.0f;
        }

        if (x < 0.0f) {
            return -1.0f;
        }

        return 0.0f;
    }

    // clampVecAngleDeg

    void clampLength(TVec3f* pDst, const TVec3f& rSrc, f32 length) {
        if (rSrc.squared() > length * length) {
            f32 sqr = rSrc.squared();

            if (length <= 0.0000038146973f) {
                pDst->zero();
            }
            else {
                f32 invSqrt = JGeometry::TUtil<f32>::inv_sqrt(sqr);

                pDst->scale(invSqrt * length, rSrc);
            }
        }
        else {
            pDst->set<f32>(rSrc);
        }
    }

    // convergeRadian

    bool isInRange(f32 a1, f32 a2, f32 a3) {
        if (a2 > a3) {
            if (a1 < a3) {
                return false;
            }
            else {
                return !(a1 > a2);
            }
        }
        else {
            if (a1 < a2) {
                return false;
            }
            else {
                return !(a1 > a3);
            }
        }
    }

    /*
    f32 calcRotateY(f32 a1, f32 a2) {
        f32 val = JMath::sAtanTable.atan2_(-a2, a1);
        return (sMinDegree + fmod((sMaxDegree + ((90.0f + (val * 57.295776f)) - sMinDegree)), sMaxDegree));
    }
    */

    // calcRotateZ

    f32 calcDistanceXY(const TVec3f &rPos1, const TVec3f &rPos2) {
        f32 xDelta = rPos1.x - rPos2.x;
        f32 yDelta = rPos1.y - rPos2.y;

        return JGeometry::TUtil<f32>::sqrt(xDelta * xDelta + yDelta * yDelta);
    }

    // rotateVecDegree
    // rotateVecDegree
    // rotateVecDegree
    // rotateVecDegree
    // rotateVecRadian
    // rotateVecRadian
    // calcLocalVec
    // normalize

    void normalize(TVec3f* pVec) {
        PSVECNormalize(pVec, pVec);
    }

    // normalize
    // normalize

    void normalize(const TVec3f& rSrc, TVec3f* pDst) {
        pDst->set(rSrc);
        normalize(pDst);
    }

    // normalize
    // normalizeOrZero

    bool normalizeOrZero(TVec3f* pVec) {
        if (isNearZero(*pVec, 0.001f)) {
            pVec->zero();

            return true;
        }

        normalize(pVec);

        return false;
    }

    bool normalizeOrZero(const TVec3f& rSrc, TVec3f* pDst) {
        pDst->set(rSrc);

        return normalizeOrZero(pDst);
    }

    void separateScalarAndDirection(f32* pScalar, TVec3f* pDir, const TVec3f& rVec) {
        *pScalar = PSVECMag(&rVec);

        if (isNearZero(rVec, 0.001f)) {
            pDir->zero();
        }
        else {
            normalize(rVec, pDir);
        }
    }

    // FIXME: Instruction groups from branches are ordered incorrectly.
    f32 normalize(f32 x, f32 min, f32 max) {
        f32 range = max - min;

        if (!isNearZero(range, 0.001f)) {
            return (JGeometry::TUtil<f32>::clamp(x, min, max) - min) / range;
        }
        else if (x < min) {
            return 0.0f;
        }
        else {
            return 1.0f;
        }
    }

    f32 normalizeAbs(f32 x, f32 min, f32 max) {
        if (x >= 0.0f) {
            return normalize(x, min, max);
        }
        else {
            return -normalize(-x, min, max);
        }
    }

    // turnVecToVecCos
    // turnVecToVecCosOnPlane
    // turnVecToVecCosOnPlane
    // turnVecToVecRadian
    // turnVecToVecDegree
    // calcMomentRollBall
    // calcReflectionVector
    // isSameDirection

    bool isOppositeDirection(const TVec3f& rParam1, const TVec3f& rParam2, f32 param3) {
        if (rParam1.dot(rParam2) >= 0.0f) {
            return false;
        }

        return isSameDirection(rParam1, rParam2, param3);
    }

    bool isNearZero(f32 x, f32 epsilon) {
        if (x < 0.0f) {
            x = -x;
        }

        if (x < epsilon) {
            return true;
        }

        return false;
    }

    bool isNearZero(const TVec3f &rVec, f32 epsilon) {
        f32 v2 = rVec.x;
        if ( v2 > epsilon )
            return 0;

        f32 v4 = -epsilon;
        if ( v2 < -epsilon )
            return 0;

        f32 v5 = rVec.y;
        if ( v5 > epsilon )
            return 0;

        if ( v5 < v4 )
            return 0;

        f32 v6 = rVec.z;

        if ( v6 > epsilon )
            return 0;

        return !(rVec.z < -epsilon);
    }

    // isNearZero

    f32 diffAngleAbs(f32 a1, f32 a2) {
        f32 normalize = normalizeAngleAbs(a1 - a2);

        if (normalize > PI) {
            normalize = (TWO_PI - normalize);
        }

        return normalize;
    }

    f32 normalizeAngleAbs(f32 angle) {
        while (1) {
            if (angle < 0.0f) {
                angle += TWO_PI;
            }
            else {
                if (!(angle > TWO_PI)) {
                    return angle;
                }
                else {
                    angle -= TWO_PI;
                }
            }
        }
    }

    bool isAngleBetween(f32 a1, f32 a2, f32 a3) {
        f32 a1_n = normalizeAngleAbs(a1);
        f32 a2_n = normalizeAngleAbs(a2);
        f32 a3_n = normalizeAngleAbs(a3);

        if (a3_n > a2_n) {
            f32 val = a3_n;
            a3_n = a2_n;
            a2_n = val;
        }

        bool res = a1_n >= a3_n && a1_n <= a2_n;

        if ((a2_n - a3_n) > PI) {
            res = !res;
        }

        return res;
    }

    #ifdef NON_MATCHING
    // register use is wrong, and mull is in wrong order for isAngleBetween
    f32 blendAngle(f32 a1, f32 a2, f32 a3) {
        f32 a1_n = normalizeAngleAbs(a1);
        f32 a2_n = normalizeAngleAbs(a2);

        if (!isAngleBetween(0.5f * (a1_n + a2_n), a1_n, a2_n)) {
            if (a1_n < a2_n) {
                a2_n += TWO_PI;
            }
            else {
                a1_n += TWO_PI;
            }
        }

        return normalizeAngleAbs(((1.0f - a3) * a1_n) + (a3 * a2_n));
    }
    #endif

    u8 lerp(u8 start, u8 end, f32 t) {
        return JGeometry::TUtil<f32>::clamp(start + (end - start) * t, 0.0f, 255.0f);
    }

    GXColor lerp(GXColor start, GXColor end, f32 t) {
        u8 a = lerp(start.a, end.a, t);
        u8 b = lerp(start.b, end.b, t);
        u8 g = lerp(start.g, end.b, t);
        u8 r = lerp(start.r, end.r, t);

        GXColor color = { r, g, b, a };

        return color;
    }

    //This function implements the selection sort sorting algorithm
    // on an array of f32 where sortArray is the array to be sorted
    // and indexArray holds the indices the elements had in the original array
    void sortSmall(s32 length, f32 *sortArray, s32 *indexArray){
        for(int i = 0; i < length; i++){
            indexArray[i] = i;
        }
        for(int index = 0; index < length; index++){
            f32 element = sortArray[index];
            int indexOfSmallestElement = index;
            for(int i = index+1; i < length; i++){
                if(element > sortArray[i]){
                    element = sortArray[i];
                    indexOfSmallestElement = i;
                }
            }
            s32 temp = indexArray[index];
            f32 temp2 = sortArray[index];
            indexArray[index] = indexArray[indexOfSmallestElement];
            sortArray[index] = element;
            indexArray[indexOfSmallestElement] = temp;
            sortArray[indexOfSmallestElement] = temp2;
        }
    };

    //This function implements the selection sort sorting algorithm
    // on an array of u32 where sortArray is the array to be sorted
    // and indexArray holds the indices the elements had in the original array
    void sortSmall(s32 length, u32 *sortArray, s32 *indexArray){
        for(int i = 0; i < length; i++){
            indexArray[i] = i;
        }
        for(int index = 0; index < length; index++){
            u32 element = sortArray[index];
            int indexOfSmallestElement = index;
            for(int i = index+1; i < length; i++){
                if(element > sortArray[i]){
                    element = sortArray[i];
                    indexOfSmallestElement = i;
                }
            }
            s32 temp = indexArray[index];
            u32 temp2 = sortArray[index];
            indexArray[index] = indexArray[indexOfSmallestElement];
            sortArray[index] = element;
            indexArray[indexOfSmallestElement] = temp;
            sortArray[indexOfSmallestElement] = temp2;
        }
    };

    f32 vecKillElement(const TVec3f &a1, const TVec3f &a2, TVec3f *a3) {
        if (isNearZero(a2, 0.001f)) {
            *a3 = a1;

            return 0.0f;
        }

        return PSVECKillElement(&a1, &a2, a3);
    }

    void vecScaleAdd(const register TVec3f *a1, const register TVec3f *a2, register f32 a3) {
        #ifdef __MWERKS__
        __asm {
            psq_l f0, 0(a1), 0, 0
            psq_l f3, 0(a2), 0, 0
            psq_l f2, 8(a1), 1, 0
            psq_l f4, 8(a2), 1, 0
            ps_madds0 f0, f3, a3, f0
            ps_madds0 f2, f4, a3, f2
            psq_st f0, 0(a1), 0, 0
            psq_st f2, 8(a1), 1, 0
        }
        #endif
    }

    void PSvecBlend(const register TVec3f *a1, const register TVec3f *a2, register TVec3f *a3, register f32 a4, register f32 a5) {
        #ifdef __MWERKS__
        __asm {
            psq_l     f0, 0(a1), 0, 0
            psq_l     f3, 8(a1), 1, 0
            ps_muls0  f4, f0, a4
            psq_l     f0, 0(a2), 0, 0
            ps_muls0  f3, f3, a4
            psq_l     f1, 8(a2), 1, 0
            ps_madds0 f4, f0, f2, f4
            ps_madds0 f3, f1, f2, f3
            psq_st    f4, 0(a3), 0, 0
            psq_st    f3, 8(a3), 1, 0
        }
        #endif
    }

    void vecBlend(const TVec3f &a1, const TVec3f &a2, TVec3f *a3, f32 a4) {
        PSvecBlend(&a1, &a2, a3, 1.0f - a4, a4);
    }

    // vecBlendNormal
    // vecBlendSphere
    // vecRotAxis

    void blendColor(GXColor* pDst, const GXColor& rStart, const GXColor& rEnd, f32 t) {
        pDst->r = getInterpolateValue(t, rStart.r, rEnd.r);
        pDst->g = getInterpolateValue(t, rStart.g, rEnd.g);
        pDst->b = getInterpolateValue(t, rStart.b, rEnd.b);
        pDst->a = getInterpolateValue(t, rStart.a, rEnd.a);
    }

    void blendVec(Vec* pDst, const Vec& rStart, const Vec& rEnd, f32 t) {
        pDst->x = getInterpolateValue(t, rStart.x, rEnd.x);
        pDst->y = getInterpolateValue(t, rStart.y, rEnd.y);
        pDst->z = getInterpolateValue(t, rStart.z, rEnd.z);
    }

    // turnVecToPlane

    int getMinAbsElementIndex(const TVec3f &rVec) {
        f64 abs_x = __fabs(rVec.x);
        f64 abs_y = __fabs(rVec.y);
        f64 abs_z = __fabs(rVec.z);

        if (abs_x < abs_y && abs_x < abs_z) {
            return 0;
        }

        if (abs_y < abs_z) {
            return 1;
        }

        return 2;
    }

    f32 getMaxElement(const TVec3f &rVec) {
        f32* vec_arr = (f32*)(&rVec);
        return vec_arr[getMaxElementIndex(rVec)];
    }

    f32 getMaxAbsElement(const TVec3f &rVec) {
        f32* vec_arr = (f32*)(&rVec);
        return vec_arr[getMaxAbsElementIndex(rVec)];
    }

    int getMaxElementIndex(const TVec3f &rVec) {
        if (rVec.x > rVec.y && rVec.x > rVec.z) {
            return 0;
        }

        if (rVec.y > rVec.z) {
            return 1;
        }

        return 2;
    }

    int getMaxAbsElementIndex(const TVec3f &rVec) {
        f64 abs_x = __fabs(rVec.x);
        f64 abs_y = __fabs(rVec.y);
        f64 abs_z = __fabs(rVec.z);

        if (abs_x > abs_y && abs_x > abs_z) {
            return 0;
        }

        if (abs_y > abs_z) {
            return 1;
        }

        return 2;
    }

    // diffAngleAbsFast
    // diffAngleAbs
    // diffAngleAbs
    // diffAngleSigned
    // diffAngleAbsHorizontal
    // diffAngleSignedHorizontal
    // isNearAngleRadian
    // isNearAngleDegree
    // isNearAngleRadianHV
    // isNearAngleDegreeHV
    // createBoundingBox
    // isNormalize
    // setNan
    // isNan
    // getFootPoint

    f32 mod(f32 x, f32 y) {
        return fmod(x, y);
    }

    void floatToFixed16(TVec3s* pDst, const TVec3f& pSrc, u8 q) {
        f32 scale = 1 << q;

        pDst->x = pSrc.x * scale;
        pDst->y = pSrc.y * scale;
        pDst->z = pSrc.z * scale;
    }

    void fixed16ToFloat(TVec3f* pDst, const TVec3s& pSrc, u8 q) {
        f32 scale = 1.0f / (1 << q);

        pDst->x = pSrc.x * scale;
        pDst->y = pSrc.y * scale;
        pDst->z = pSrc.z * scale;
    }

    // getRotatedAxisY
    // getRotatedAxisZ
};
