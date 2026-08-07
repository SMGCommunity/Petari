#pragma once

#include "Game/Util/MathUtil.hpp"
#include "Game/Util/VectorUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>

namespace MR {
    void crossToPolar(const TVec3f& rA, const TVec3f& rB, f32* pLength, f32* pAngleX, f32* pAngleY);
    void polarToCross(const TVec3f& rSrc, TVec3f* pDst, f32 length, f32 angleX, f32 angleY);
    void polarToCrossDegree(const TVec3f& rSrc, TVec3f* pDst, f32 length, f32 angleX, f32 angleY);
    f32 diffRadian(f32 angleA, f32 angleB);
    bool diffRadianAroundAxis(f32* pAngle, const TVec3f& rAxis, const TVec3f& rA, const TVec3f& rB);
    void projectVecOnPlane(TVec3f* pDst, const TVec3f& rSrc, const TVec3f& rNorm);
}  // namespace MR
