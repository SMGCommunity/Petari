#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <JSystem/JGeometry/TUtil.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <JSystem/JMath/JMath.hpp>

extern "C" int __fpclassifyf(f32);

namespace {
    static f32* gAcosTable;
    const f32 cMinDegree = 0.0f;
    const f32 cMaxDegree = 360.0f;
};

namespace MR {
    void initAcosTable() {
        gAcosTable = new f32[256];

        for (u32 i = 0; i < 256; i++) {
            // FIXME: Double-precision floating-point numbers load from incorrect offsets.
            f64 x = (i / 255.0) * 0.02 + 0.98;

            if (x > 1.0f) {
                x = 1.0f;
            }

            gAcosTable[i] = acos(x);
        }
    }

    f32 acosEx(f32 x) {
        if (__fabsf(x) < 0.98f) {
            return JMAAcosRadian(x);
        }
        else if (x < 0.0f) {
            u32 index = static_cast<u32>((-x - 0.98f) * 255.0f * 50.0f);
            f32 acos = gAcosTable[index];

            return PI - acos;
        }
        else {
            u32 index = static_cast<u32>((x - 0.98f) * 255.0f * 50.0f);

            return gAcosTable[index];
        }
    }

    f32 getRandom() {
        u32 rand = SingletonHolder<GameSystem>::get()->mObjHolder->mRandom.rand();
        u32 value = (rand >> 9) | 0x3F800000;

        return reinterpret_cast<f32&>(value) - 1.0f;
    }

    f32 getRandom(f32 min, f32 max) {
        return min + (max - min) * getRandom();
    }

    s32 getRandom(s32 min, s32 max) {
        return getRandom(static_cast<f32>(min), static_cast<f32>(max));
    }

    f32 getRandomDegree() {
        return getRandom(cMinDegree, cMaxDegree);
    }

    void calcRandomVec(TVec3f* pDst, f32 min, f32 max) {
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
        f32 srcLength = rSrc.length();

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

    f32 getLinerValue(f32 x, f32 start, f32 end, f32 max) {
        return getInterpolateValue(x / max, start, end);
    }

    f32 getLinerValueFromMinMax(f32 x, f32 min, f32 max, f32 start, f32 end) {
        return getInterpolateValue((JGeometry::TUtil<f32>::clamp(x, min, max) - min) / (max - min), start, end);
    }

    // FIXME: Source registers swapped in multiplication by pi.
    f32 getEaseInValue(f32 x, f32 start, f32 end, f32 max) {
        return getInterpolateValue(1.0f - JMACosRadian(((x / max) * PI) / 2.0f), start, end);
    }

    // FIXME: Source registers swapped in multiplication by pi.
    f32 getEaseOutValue(f32 x, f32 start, f32 end, f32 max) {
        return getInterpolateValue(JMASinRadian(((x / max) * PI) / 2.0f), start, end);
    }

    // FIXME: Source registers swapped in multiplication by pi, and the instruction is swapped.
    f32 getEaseInOutValue(f32 x, f32 start, f32 end, f32 max) {
        return getInterpolateValue((1.0f - JMACosRadian((x / max) * PI)) / 2.0f, start, end);
    }

    // getScaleWithReactionValueZeroToOne
    // getConvergeVibrationValue
    // getReduceVibrationValue

    void makeAxisFrontUp(TVec3f* pParam1, TVec3f* pParam2, const TVec3f& rParam3, const TVec3f& rParam4) {
        PSVECCrossProduct(&rParam4, &rParam3, pParam1);
        PSVECNormalize(pParam1, pParam1);
        PSVECCrossProduct(&rParam3, pParam1, pParam2);
    }

    void makeAxisFrontSide(TVec3f* pParam1, TVec3f* pParam2, const TVec3f& rParam3, const TVec3f& rParam4) {
        PSVECCrossProduct(&rParam3, &rParam4, pParam1);
        PSVECNormalize(pParam1, pParam1);
        PSVECCrossProduct(pParam1, &rParam3, pParam2);
    }

    void makeAxisUpFront(TVec3f* pParam1, TVec3f* pParam2, const TVec3f& rParam3, const TVec3f& rParam4) {
        PSVECCrossProduct(&rParam3, &rParam4, pParam1);
        PSVECNormalize(pParam1, pParam1);
        PSVECCrossProduct(pParam1, &rParam3, pParam2);
    }

    void makeAxisUpSide(TVec3f* pParam1, TVec3f* pParam2, const TVec3f& rParam3, const TVec3f& rParam4) {
        PSVECCrossProduct(&rParam4, &rParam3, pParam1);
        PSVECNormalize(pParam1, pParam1);
        PSVECCrossProduct(&rParam3, pParam1, pParam2);
    }

    void makeAxisVerticalZX(TVec3f* pParam1, const TVec3f& rParam2) {
        TVec3f z(0.0f, 0.0f, 1.0f);

        pParam1->rejection(z, rParam2);

        if (isNearZero(*pParam1, 0.001f)) {
            TVec3f x(1.0f, 0.0f, 0.0f);

            pParam1->rejection(x, rParam2);
        }

        normalize(pParam1);
    }

    void makeAxisCrossPlane(TVec3f* pParam1, TVec3f* pParam2, const TVec3f& rParam3) {
        makeAxisVerticalZX(pParam1, rParam3);
        PSVECCrossProduct(pParam1, &rParam3, pParam2);
        normalizeOrZero(pParam2);
    }

    bool makeAxisAndCosignVecToVec(TVec3f* pParam1, f32* pParam2, const TVec3f& rParam3, const TVec3f& rParam4) {
        TVec3f v1;

        if (isOppositeDirection(rParam3, rParam4, 0.01f)) {
            turnRandomVector(&v1, rParam3, 0.01f);
            normalize(&v1);
        }
        else {
            v1.set(rParam3);
        }

        TVec3f v2;

        PSVECCrossProduct(&v1, &rParam4, &v2);

        if (isNearZero(v2, 0.001f)) {
            pParam1->zero();
            *pParam2 = 1.0f;

            return false;
        }
        else {
            normalize(v2, pParam1);
            *pParam2 = JGeometry::TUtil<f32>::clamp(v1.dot(rParam4), -1.0f, 1.0f);

            return true;
        }
    }

    // calcPerpendicFootToLine
    // calcPerpendicFootToLineInside
    // checkHitSegmentSphere
    // checkHitSemilinePlane
    // calcReboundVelocity
    // calcReboundVelocity
    // calcParabolicFunctionParam

    void makeQuatRotateRadian(TQuat4f* pParam1, const TVec3f& rParam2) {
        pParam1->setEuler(rParam2.x, rParam2.y, rParam2.z);
    }

    // FIXME: Floating-point registers are allocated incorrectly.
    void makeQuatRotateDegree(TQuat4f* pParam1, const TVec3f& rParam2) {
        pParam1->setEuler(rParam2.x * PI_180, rParam2.y * PI_180, rParam2.z * PI_180);
    }

    void makeQuatFromVec(TQuat4f* pParam1, const TVec3f& rParam2, const TVec3f& rParam3) {
        TPos3f pos;

        makeMtxFrontUp(&pos, rParam2, rParam3);
        pos.getQuat(*pParam1);
    }

    void makeQuatSideUp(TQuat4f* pParam1, const TVec3f& rParam2, const TVec3f& rParam3) {
        TPos3f pos;

        makeMtxSideUp(&pos, rParam2, rParam3);
        pos.getQuat(*pParam1);
    }

    void makeQuatUpFront(TQuat4f* pParam1, const TVec3f& rParam2, const TVec3f& rParam3) {
        TPos3f pos;

        makeMtxUpFront(&pos, rParam2, rParam3);
        pos.getQuat(*pParam1);
    }

    void makeQuatUpNoSupport(TQuat4f* pParam1, const TVec3f& rParam2) {
        TPos3f pos;

        makeMtxUpNoSupport(&pos, rParam2);
        pos.getQuat(*pParam1);
    }

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
        return cMinDegree + mod((cMaxDegree + ((90.0f + (val * 57.295776f)) - cMinDegree)), cMaxDegree);
    }
    */

    // calcRotateZ

    f32 calcDistanceXY(const TVec3f& rPos1, const TVec3f& rPos2) {
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
        *pScalar = rVec.length();

        if (isNearZero(rVec, 0.001f)) {
            pDir->zero();
        }
        else {
            normalize(rVec, pDir);
        }
    }

    f32 normalize(f32 x, f32 min, f32 max) {
        f32 range = max - min;

        if (!isNearZero(range, 0.001f)) {
            
        }
        else if (x < min) {
            return 0.0f;
        }
        else {
            return 1.0f;
        }

        return (JGeometry::TUtil<f32>::clamp(x, min, max) - min) / range;
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

    bool isSameDirection(const TVec3f& rVec1, const TVec3f& rVec2, f32 tolerance) {
        if (__fabsf(rVec1.y * rVec2.z - rVec1.z * rVec2.y) > tolerance) {
            return false;
        }

        if (__fabsf(rVec1.z * rVec2.x - rVec1.x * rVec2.z) > tolerance) {
            return false;
        }

        if (__fabsf(rVec1.x * rVec2.y - rVec1.y * rVec2.x) > tolerance) {
            return false;
        }

        return true;
    }

    bool isOppositeDirection(const TVec3f& rVec1, const TVec3f& rVec2, f32 tolerance) {
        if (rVec1.dot(rVec2) >= 0.0f) {
            return false;
        }

        return isSameDirection(rVec1, rVec2, tolerance);
    }

    bool isNearZero(f32 x, f32 tolerance) {
        if (x < 0.0f) {
            x = -x;
        }

        if (x < tolerance) {
            return true;
        }

        return false;
    }

    bool isNearZero(const TVec3f& rVec, f32 tolerance) {
        if (rVec.x > tolerance) {
            return false;
        }

        if (rVec.x < -tolerance) {
            return false;
        }

        if (rVec.y > tolerance) {
            return false;
        }

        if (rVec.y < -tolerance) {
            return false;
        }

        if (rVec.z > tolerance) {
            return false;
        }

        if (rVec.z < -tolerance) {
            return false;
        }

        return true;
    }

    bool isNearZero(const TVec2f& rVec, f32 tolerance) {
        if (rVec.x > tolerance) {
            return false;
        }

        if (rVec.x < -tolerance) {
            return false;
        }

        if (rVec.y > tolerance) {
            return false;
        }

        if (rVec.y < -tolerance) {
            return false;
        }

        return true;
    }

    f32 diffAngleAbs(f32 a1, f32 a2) {
        f32 normalize = normalizeAngleAbs(a1 - a2);

        if (normalize > PI) {
            normalize = (TWO_PI - normalize);
        }

        return normalize;
    }

    f32 normalizeAngleAbs(f32 angle) {
        while (true) {
            if (angle < 0.0f) {
                angle += TWO_PI;
            }
            else if (angle > TWO_PI) {
                angle -= TWO_PI;
            }
            else {
                return angle;
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

        if (a2_n - a3_n > PI) {
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

    u32 getMinAbsElementIndex(const TVec3f &rVec) {
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

    u32 getMaxElementIndex(const TVec3f &rVec) {
        if (rVec.x > rVec.y && rVec.x > rVec.z) {
            return 0;
        }

        if (rVec.y > rVec.z) {
            return 1;
        }

        return 2;
    }

    u32 getMaxAbsElementIndex(const TVec3f &rVec) {
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

    f32 diffAngleAbsFast(const TVec3f& rParam1, const TVec3f& rParam2) {
        return JMAAcosRadian(rParam1.dot(rParam2));
    }

    f32 diffAngleAbs(const TVec3f& rParam1, const TVec3f& rParam2) {
        f32 length1 = rParam1.length();
        f32 length2 = rParam2.length();
        f32 x = rParam1.dot(rParam2) / (length1 * length2);

        if (x >= 1.0f) {
            return 0.0f;
        }
        else if (x <= -1.0f) {
            return PI;
        }
        else {
            return acosEx(x);
        }
    }

    f32 diffAngleAbs(const TVec2f& rParam1, const TVec2f& rParam2) {
        f32 length1 = JGeometry::TUtil<f32>::sqrt(rParam1.dot(rParam1)); // TODO: Should probably be `TVec2f::length`
        f32 length2 = JGeometry::TUtil<f32>::sqrt(rParam2.dot(rParam2)); // TODO: Should probably be `TVec2f::length`
        f32 x = rParam1.dot(rParam2) / (length1 * length2);

        if (x >= 1.0f) {
            return 0.0f;
        }
        else if (x <= -1.0f) {
            return PI;
        }
        else {
            return acosEx(x);
        }
    }

    f32 diffAngleSigned(const TVec3f& rParam1, const TVec3f& rParam2, const TVec3f& rParam3) {
        f32 angleDiff = diffAngleAbs(rParam1, rParam2);

        TVec3f v;
        PSVECCrossProduct(&rParam1, &rParam3, &v);

        if (v.dot(rParam2) >= 0.0f) {
            return angleDiff;
        }
        else {
            return -angleDiff;
        }
    }

    f32 diffAngleAbsHorizontal(const TVec3f& rParam1, const TVec3f& rParam2, const TVec3f& rParam3) {
        TVec3f v1;
        TVec3f v2;

        vecKillElement(rParam1, rParam3, &v1);
        vecKillElement(rParam2, rParam3, &v2);

        return diffAngleAbs(v1, v2);
    }

    // diffAngleSignedHorizontal

    bool isNearAngleRadian(const TVec3f& rParam1, const TVec3f& rParam2, f32 param3) {
        if (isNearZero(rParam1, 0.001f) || isNearZero(rParam2, 0.001f)) {
            return false;
        }

        TVec3f v1;
        normalize(rParam1, &v1);

        TVec3f v2;
        normalize(rParam2, &v2);

        f32 cos = JMACosRadian(param3);
        f32 dot = v1.dot(v2);

        return dot >= cos;
    }

    bool isNearAngleDegree(const TVec3f& rParam1, const TVec3f& rParam2, f32 param3) {
        param3 = PI_180 * param3;

        return isNearAngleRadian(rParam1, rParam2, param3);
    }

    bool isNearAngleRadianHV(const TVec3f& rParam1, const TVec3f& rParam2, const TVec3f& rParam3, f32 param4, f32 param5) {
        TVec3f v1;
        TVec3f v2;
        TVec3f v3;
        TVec3f v4;

        if (param5 > 1.5707964) {
            param5 = HALF_PI;
        }

        if (normalizeOrZero(rParam1, &v1)) {
            return false;
        }

        if (normalizeOrZero(rParam2, &v2)) {
            return false;
        }

        if (normalizeOrZero(rParam3, &v3)) {
            return false;
        }

        v4.rejection(rParam1, rParam3);
        normalizeOrZero(&v4);

        f32 cos = JMACosRadian(param4);
        f32 dot1 = v4.dot(v2);

        if (dot1 < cos) {
            return false;
        }

        f32 sin = __fabsf(JMASinRadian(param5));
        f32 dot2 = __fabsf(v1.dot(v3));

        return !(dot2 > sin);
    }

    bool isNearAngleDegreeHV(const TVec3f& rParam1, const TVec3f& rParam2, const TVec3f& rParam3, f32 param4, f32 param5) {
        param5 = PI_180 * param5;
        param4 = PI_180 * param4;

        return isNearAngleRadianHV(rParam1, rParam2, rParam3, param4, param5);
    }

    // createBoundingBox

    bool isNormalize(const TVec3f& rVec, f32 tolerance) {
        return __fabsf(1.0f - rVec.length()) <= tolerance;
    }

    // setNan

    bool isNan(const TVec3f& rVec) {
        if (__fpclassifyf(rVec.x) == 1
            || __fpclassifyf(rVec.y) == 1
            || __fpclassifyf(rVec.z) == 1)
        {
            return true;
        }

        return false;
    }

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
