#include "Game/Util/MathUtil.hpp"
#include "Game/Util/VectorUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include <JSystem/JGeometry/TVec.hpp>

void CameraCalc_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
    (void)MR::pi();
    (void)1023.5f;
    (void)(MR::pi() / 2.0f);
}

namespace MR {
    void crossToPolar(const TVec3f& rA, const TVec3f& rB, f32* pLength, f32* pAngleX, f32* pAngleY) {
        f32 dx = rB.x - rA.x;
        f32 dy = rB.y - rA.y;
        f32 dz = rB.z - rA.z;
        *pLength = MR::sqrt(dx * dx + dy * dy + dz * dz);
        *pAngleX = MR::atan2(dy, MR::sqrt(dx * dx + dz * dz));
        *pAngleY = MR::atan2(dx, dz);
    }

    void polarToCross(const TVec3f& rSrc, TVec3f* pDst, f32 length, f32 angleX, f32 angleY) {
        pDst->x = rSrc.x + length * MR::cos(angleX) * MR::sin(angleY);
        pDst->y = rSrc.y + length * MR::sin(angleX);
        pDst->z = rSrc.z + length * MR::cos(angleX) * MR::cos(angleY);
    }

    void polarToCrossDegree(const TVec3f& rSrc, TVec3f* pDst, f32 length, f32 angleX, f32 angleY) {
        pDst->x = rSrc.x + length * MR::cosDegree(angleX) * MR::sinDegree(angleY);
        pDst->y = rSrc.y + length * MR::sinDegree(angleX);
        pDst->z = rSrc.z + length * MR::cosDegree(angleX) * MR::cosDegree(angleY);
    }

    f32 diffRadian(f32 angleA, f32 angleB) {
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

    bool diffRadianAroundAxis(f32* pAngle, const TVec3f& rAxis, const TVec3f& rA, const TVec3f& rB) {
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

    void projectVecOnPlane(TVec3f* pDst, const TVec3f& rSrc, const TVec3f& rNorm) {
        pDst->set(rSrc - rNorm * rNorm.dot(rSrc));
    }
};  // namespace MR

void CameraCalc_DUMMY() {
    TVec3f d(JMAAcosRadian(0.0f));
    TVec3f a, b, c;
    b *= 2.0f;
}
