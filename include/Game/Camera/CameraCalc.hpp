#pragma once

#include "JSystem/JGeometry/TVec.hpp"

namespace MR {
    void polarToCross(const TVec3f&, TVec3f*, f32, f32, f32);
    void crossToPolar(const TVec3f&, const TVec3f&, f32*, f32*, f32*);
}  // namespace MR
