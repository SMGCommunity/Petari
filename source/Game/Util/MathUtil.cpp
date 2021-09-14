#include "Game/Util/MathUtil.h"
#include "JSystem/JGeometry/TUtil.h"
#include "JSystem/JMath/JMath.h"

#include "math_types.h"

#include <cmath>

namespace MR {
    bool isHalfProbability() {
        return (getRandom() < 0.5f);
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

    
    f32 calcDistanceXY(const TVec3f &a1, const TVec3f &a2) {
        f32 x_diff = a1.x - a2.x;
        f32 y_diff = a1.y - a2.y;
        return JGeometry::TUtil<f32>::sqrt((x_diff * x_diff) + (y_diff * y_diff));
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
};