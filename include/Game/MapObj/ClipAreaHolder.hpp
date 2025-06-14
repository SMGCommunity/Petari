#pragma once

#include "Game/MapObj/ClipArea.hpp"
#include <revolution.h>
#include <JSystem/JGeometry/TVec.hpp>

namespace MR {
    void createClipAreaHolder();
    void addClipArea(ClipArea*);
    
    bool isActiveClipArea();
    bool isInClipArea(const TVec3f &, f32);
};
