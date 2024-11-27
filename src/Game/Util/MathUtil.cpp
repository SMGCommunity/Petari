#include "Game/Util/MathUtil.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

#include "math_types.hpp"

#include <cmath>

static f32 minDegree = 0.0f;
static f32 maxDegree = 360.0f;

namespace MR {
    f32 getRandom(f32 min, f32 max) {
        return (min + ((max - min) * getRandom()));
    }

    // getRandom(s32, s32)

    f32 getRandomDegree() {
        return minDegree + (maxDegree * getRandom());
    }

    void calcRandomVec(TVec3f *pOut, f32 a2, f32 a3) {
        f32 v10 = (a2 + ((a3 - a2) * getRandom()));
        f32 v11 = (a2 + ((a3 - a2) * getRandom()));
        f32 v12 = getRandom();

        f32 dist = a3 - a2;
        pOut->set((a2 + (dist * v12)), v11, v10);
    }

    u8 isHalfProbability() {
        return MR::getRandom() < 0.5f;
    }

    f32 getSignHalfProbability() {
        if (isHalfProbability()) {
            return -1.0f;
        }

        return 1.0f;
    }

    void getRandomVector(TVec3f *pOut, f32 a2) {
        f32 z = (-a2 + ((a2 - -a2) * getRandom()));
        f32 y = (-a2 + ((a2 - -a2) * getRandom()));
        pOut->set((-a2 + ((a2 - -a2) * getRandom())), y, z);
    }

    #ifdef NON_MATCHING
    // stack places randVec and otherVec wrongly
    void addRandomVector(TVec3f *pOut, const TVec3f &rOtherVec, f32 a3) {
        
        f32 x = -a3 + ((a3 - -a3) * getRandom());
        f32 y = -a3 + ((a3 - -a3) * getRandom());
        f32 z = -a3 + ((a3 - -a3) * getRandom());

        TVec3f randVec;
        randVec.x = x;
        randVec.y = y;
        randVec.z = z;

        TVec3f otherVec(rOtherVec);
        otherVec.add(randVec);
        pOut->set(otherVec);
    }
    #endif

    void turnRandomVector(TVec3f *pOut, const TVec3f &rOtherVec, f32 a3) {
        f32 mag = PSVECMag((Vec*)(&rOtherVec));
        addRandomVector(pOut, rOtherVec, a3);

        if (isNearZero(*pOut, 0.001f)) {
            pOut->set(rOtherVec);
        }
        else {
            pOut->setLength(mag);
        }
    }


    f32 getInterpolateValue(f32 a1, f32 a2, f32 a3) {
        return (a2 + (a1 * (a3 - a2)));
    }

    f32 getLinerValue(f32 a1, f32 a2, f32 a3, f32 a4) {
        return (a2 + (a1 / a4) * (a3 - a2));
    }

    f32 getLinerValueFromMinMax(f32 a1, f32 a2, f32 a3, f32 a4, f32 a5) {
        f32 val = (JGeometry::TUtil<f32>::clamp(a1, a2, a3) - a2) / (a3 - a2);
        return a4 + ((a5 - a4) * val);
    }

    #ifdef NON_MATCHING
    // wrong register orders
    f32 getEaseInValue(f32 a1, f32 a2, f32 a3, f32 a4) {
        f32 val = (1.0f - JMACosRadian((((a1 / a4) * PI) * 0.5f)));
        return a2 + ((a3 - a2) * val);
    }
    #endif

    #ifdef NON_MATCHING
    // wrong register orders
    f32 getEaseOutValue(f32 a1, f32 a2, f32 a3, f32 a4) {
        f32 val = (JMASinRadian((((a1 / a4) * PI) * 0.5f)) * (a3 - a2));
        return a2 + val;
    }
    #endif

    // MR::getEaseInOutValue
    // MR::getScaleWithReactionValueZeroToOne
    // MR::getConvergeVibrationValue
    // MR::getReduceVibrationValue

    void makeAxisFrontUp(TVec3f *a1, TVec3f *a2, const TVec3f &a3, const TVec3f &a4) {
        PSVECCrossProduct((Vec*)&a4, (Vec*)&a3, (Vec*)a1);
        PSVECNormalize((Vec*)a1, (Vec*)a1);
        PSVECCrossProduct((Vec*)&a3, (Vec*)a1, (Vec*)a2);
    }

    // MR::makeAxisFrontSide
    // MR::makeAxisUpFront
    // MR::makeAxisUpSide
    // MR::makeAxisVerticalZX
    // MR::makeAxisCrossPlane
    // MR::makeAxisAndCosignVecToVec
    // MR::calcPerpendicFootToLine
    // MR::calcPerpendicFootToLineInside

    // etc

    void clampLength(TVec3f *a1, const TVec3f &a2, f32 a3) {
        if (a2.squared() > (a3 * a3)) {
            f32 sqr = a2.squared();

            if (sqr <= 0.0000038146973f) {
                a1->zero();
            }
            else {
                f32 inv_sqr = JGeometry::TUtil<f32>::inv_sqrt(sqr);
                a1->scale((inv_sqr * a3), a2);
            }
           
        }
        else {
            a1->set(a2);
        }
    }

    // MR::convergeRadian(f32, f32, f32)

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

    /*f32 calcRotateY(f32 a1, f32 a2) {
        f32 val = JMath::sAtanTable.atan2_(-a2, a1);
        return (minDegree + fmod((maxDegree + ((90.0f + (val * 57.295776f)) - minDegree)), maxDegree));
    }*/

    
    f32 calcDistanceXY(const TVec3f &a1, const TVec3f &a2) {
        f32 x_diff = a1.x - a2.x;
        f32 y_diff = a1.y - a2.y;
        return JGeometry::TUtil<f32>::sqrt((x_diff * x_diff) + (y_diff * y_diff));
    }

    bool isNearZero(f32 a1, f32 a2) {
        if (a1 < 0.0f) {
            a1 = -a1;
        }

        if (a1 < a2) {
            return true;
        }

        return false;
    }

    bool isNearZero(const TVec3f &rVec, f32 a2) {
        f32 v2 = rVec.x;
        if ( v2 > a2 )
            return 0;

        f32 v4 = -a2;
        if ( v2 < -a2 )
            return 0;

        f32 v5 = rVec.y;
        if ( v5 > a2 )
            return 0;

        if ( v5 < v4 )
            return 0;

        f32 v6 = rVec.z;

        if ( v6 > a2 )
            return 0;

        return !(rVec.z < -a2);
    }

    
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

        bool res = false;

        if (a1_n >= a3_n && a1_n <= a2_n) {
            res = true;
        }

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

        return normalizeAngleAbs((((1.0f - a3) * a1_n) + (a3 * a2_n)));
    }
    #endif

    u8 lerp(u8 a1, u8 a2, f32 a3) {
        return JGeometry::TUtil<f32>::clamp(a1 + (a3 * (a2 - a1)), 0.0f, 255.0f);
    }

    /*#ifdef NON_MATCHING
    _GXColor lerp(_GXColor a1, _GXColor a2, f32 a3) {
        u8 v6 = lerp(a1.a, a2.a, a3);
        u8 v7 = lerp(a1.b, a2.b, a3);
        u8 v8 = lerp(a1.g, a2.b, a3);
        u8 thing = lerp(a1.r, a2.r, a3);

        return (v6 | ((v7 << 8) & 0xFF00 | ((v8 << 16) & 0xFF000 | thing << 24)) & 0xFFFF00FF) & 0xFFFFFF00;
    }
    #endif*/

    f32 vecKillElement(const TVec3f &a1, const TVec3f &a2, TVec3f *a3) {
        if (isNearZero(a2, 0.001f)) {
            *a3 = a1;
            return 0.0f;
        }

        return PSVECKillElement((const Vec*)&a1, (const Vec*)&a2, (const Vec*)a3);
    }

    void vecScaleAdd(const register TVec3f *a1, const register TVec3f *a2, register f32 a3) {
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
    }

    void PSvecBlend(const register TVec3f *a1, const register TVec3f *a2, register TVec3f *a3, register f32 a4, register f32 a5) {
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
    }

    void vecBlend(const TVec3f &a1, const TVec3f &a2, TVec3f *a3, f32 a4) {
        PSvecBlend(&a1, &a2, a3, 1.0f - a4, a4);
    }

    // MR::vecBlendNormal
    // MR::vecBlendSphere
    // MR::vecRotAxis

    void blendColor(_GXColor *a1, const _GXColor &a2, const _GXColor &a3, f32 a4) {
        a1->r = getInterpolateValue(a4, a2.r, a3.r);
        a1->g = getInterpolateValue(a4, a2.g, a3.g);
        a1->b = getInterpolateValue(a4, a2.b, a3.b);
        a1->a = getInterpolateValue(a4, a2.a, a3.a);
    }

    void blendVec(Vec *a1, const Vec &a2, const Vec &a3, f32 a4) {
        a1->x = getInterpolateValue(a4, a2.x, a3.x);
        a1->y = getInterpolateValue(a4, a2.y, a3.y);
        a1->z = getInterpolateValue(a4, a2.z, a3.z);
    }

    // MR::turnVecToPlane

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
};
