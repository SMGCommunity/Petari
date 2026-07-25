#pragma once

#include "Game/Util/MathUtil.hpp"
#include <JSystem/JGeometry.hpp>

namespace MR {
    inline void getUpVecFromQuat(TVec3f* pOut, const TQuat4f& rQuat) {
        pOut->set< f32 >((2.0f * (rQuat.x * rQuat.y)) - (2.0f * (rQuat.w * rQuat.z)),
                         ((1.0f - (2.0f * (rQuat.x * rQuat.x))) - (2.0f * (rQuat.z * rQuat.z))),
                         ((2.0f * (rQuat.y * rQuat.z)) + (2.0f * (rQuat.w * rQuat.x))));
    }

    inline void crossToPolar(const TVec3f& rA, const TVec3f& rB, f32* pX, f32* pY, f32* pZ) NO_INLINE {
        f32 dx = rB.x - rA.x;
        f32 dy = rB.y - rA.y;
        f32 dz = rB.z - rA.z;
        *pX = MR::sqrt(dx * dx + dy * dy + dz * dz);
        *pY = MR::atan2(dy, MR::sqrt(dx * dx + dz * dz));
        *pZ = MR::atan2(dx, dz);
    }

    inline void polarToCross(const TVec3f& rSrc, TVec3f* pDst, f32 angleX, f32 angleY, f32 angleZ) NO_INLINE {
        pDst->x = rSrc.x + angleX * MR::cos(angleY) * MR::sin(angleZ);
        pDst->y = rSrc.y + angleX * MR::sin(angleY);
        pDst->z = rSrc.z + angleX * MR::cos(angleY) * MR::cos(angleZ);
    }

    inline void polarToCrossDegree(const TVec3f& rSrc, TVec3f* pDst, f32 angleX, f32 angleY, f32 angleZ) NO_INLINE {
        pDst->x = rSrc.x + angleX * MR::cosDegree(angleY) * MR::sinDegree(angleZ);
        pDst->y = rSrc.y + angleX * MR::sinDegree(angleY);
        pDst->z = rSrc.z + angleX * MR::cosDegree(angleY) * MR::cosDegree(angleZ);
    }

    inline f32 diffRadian(f32 angleA, f32 angleB) {
        f32 diff = MR::repeatDegree(angleB) - MR::repeatDegree(angleA);
        if (diff > 0.0f) {
            if (diff > MR::pi()) {
                diff = -(MR::pi() * 2.0f - diff);
            }
        } else {
            if (diff < -MR::pi()) {
                diff = diff + MR::pi() * 2.0f;
            }
        }
        return diff;
    }

    inline bool diffRadianAroundAxis(f32* pAngle, const TVec3f& rAxis, const TVec3f& rA, const TVec3f& rB) {
        TVec3f dirA = rA.killElement(rAxis);
        if (MR::isNearZero(dirA)) {
            return false;
        }

        TVec3f dirB = rB.killElement(rAxis);
        if (MR::isNearZero(dirB)) {
            return false;
        }

        MR::normalize(&dirA);
        MR::normalize(&dirB);
        s32 sign = rA.cross(rB).dot(rAxis) < 0.0f ? -1 : 1;
        *pAngle = MR::acos(dirA.dot(dirB)) * sign;
        return true;
    }

    inline void projectVecOnPlane(TVec3f* pDst, const TVec3f& rSrc, const TVec3f& rNorm) {
        pDst->set(rSrc - rNorm * rNorm.dot(rSrc));
    }

};  // namespace MR
