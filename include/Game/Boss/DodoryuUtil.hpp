#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class Dodoryu;

namespace DodoryuUtil {
    bool calcVerticalizedDir(Dodoryu*, TVec3f*, const TVec3f&);
    void turnToward(Dodoryu*, const TVec3f&, f32);
    void accelerate(Dodoryu*, const TVec3f&, f32, f32, f32);
    void addVelocity(Dodoryu*, bool);
    void rotateVelocityByWall(Dodoryu*);
    void calcEscapeDir(Dodoryu*, TVec3f*, f32);
};  // namespace DodoryuUtil
