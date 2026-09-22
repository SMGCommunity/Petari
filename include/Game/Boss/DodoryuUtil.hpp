#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class Dodoryu;

namespace DodoryuUtil {
    bool calcVerticalizedDir(Dodoryu* pDodoryu, TVec3f* pVec, const TVec3f& rVec);
    void turnToward(Dodoryu* pDodoryu, const TVec3f& rDirection, f32 maxAngle);
    void accelerate(Dodoryu* pDodoryu, const TVec3f& rDirection, f32 acceleration, f32 lateralDamping, f32 maxSpeed);
    void addVelocity(Dodoryu* pDodoryu, bool isSnap);
    void rotateVelocityByWall(Dodoryu* pDodoryu);
    void calcEscapeDir(Dodoryu* pDodoryu, TVec3f* pDirection, f32 maxAngle);
}  // namespace DodoryuUtil
