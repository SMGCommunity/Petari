#pragma once

#include "Game/Util/MathUtil.hpp"
#include <JSystem/JGeometry.hpp>

namespace MR {
    inline void getUpVecFromQuat(TVec3f* pOut, const TQuat4f& rQuat) {
        pOut->set< f32 >((2.0f * (rQuat.x * rQuat.y)) - (2.0f * (rQuat.w * rQuat.z)),
                         ((1.0f - (2.0f * (rQuat.x * rQuat.x))) - (2.0f * (rQuat.z * rQuat.z))),
                         ((2.0f * (rQuat.y * rQuat.z)) + (2.0f * (rQuat.w * rQuat.x))));
    }

    inline void crossToPolar(const TVec3f& rA, const TVec3f& rB, f32* pLength, f32* pAngleX, f32* pAngleY) NO_INLINE {
        f32 dx = rB.x - rA.x;
        f32 dy = rB.y - rA.y;
        f32 dz = rB.z - rA.z;
        *pLength = MR::sqrt(dx * dx + dy * dy + dz * dz);
        *pAngleX = MR::atan2(dy, MR::sqrt(dx * dx + dz * dz));
        *pAngleY = MR::atan2(dx, dz);
    }

    inline void polarToCross(const TVec3f& rSrc, TVec3f* pDst, f32 length, f32 angleX, f32 angleY) NO_INLINE {
        pDst->x = rSrc.x + length * MR::cos(angleX) * MR::sin(angleY);
        pDst->y = rSrc.y + length * MR::sin(angleX);
        pDst->z = rSrc.z + length * MR::cos(angleX) * MR::cos(angleY);
    }

    inline void polarToCrossDegree(const TVec3f& rSrc, TVec3f* pDst, f32 length, f32 angleX, f32 angleY) NO_INLINE {
        pDst->x = rSrc.x + length * MR::cosDegree(angleX) * MR::sinDegree(angleY);
        pDst->y = rSrc.y + length * MR::sinDegree(angleX);
        pDst->z = rSrc.z + length * MR::cosDegree(angleX) * MR::cosDegree(angleY);
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

    inline bool diffRadianAroundAxis(f32* pAngle, const TVec3f& rAxis, const TVec3f& rA, const TVec3f& rB) NO_INLINE {
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

    inline void projectVecOnPlane(TVec3f* pDst, const TVec3f& rSrc, const TVec3f& rNorm) NO_INLINE {
        pDst->set(rSrc - rNorm * rNorm.dot(rSrc));
    }

};  // namespace MR
