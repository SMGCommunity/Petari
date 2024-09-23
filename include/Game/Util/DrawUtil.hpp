#pragma once

#include <revolution.h>

class J3DModelData;

namespace MR {
    void setupShadowVolumeDraw();

    void drawSimpleModel(J3DModelData*);

    void loadTexProjectionMtx(u32);

    void reinitGX();

    void fillScreen(const _GXColor&);

    void clearZBuffer();
};  // namespace MR