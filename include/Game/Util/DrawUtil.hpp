#pragma once

#include "revolution/gx/GXStruct.h"
#include <revolution.h>
#include <JSystem/JGeometry.hpp>

class J3DModelData;

namespace MR {
    void setupShadowVolumeDraw();

    void drawSimpleModel(J3DModelData*);

    void loadTexProjectionMtx(u32);

    void reinitGX();

    void fillScreen(const _GXColor&);

    void fillScreenSetup(const _GXColor &);

    void fillScreenArea(const TVec2s &, const TVec2s &);

    void clearZBuffer();
};  // namespace MR
