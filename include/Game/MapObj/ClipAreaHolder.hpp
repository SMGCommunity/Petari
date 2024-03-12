#pragma once

#include <revolution.h>
#include <JSystem/JGeometry/TVec.hpp>

namespace MR {
    void createClipAreaHolder();
    
    bool isInClipArea(const TVec3f &, f32);
};