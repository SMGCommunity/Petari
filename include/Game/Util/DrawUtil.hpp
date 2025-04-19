#pragma once

#include "revolution/gx/GXStruct.h"
#include <revolution.h>
#include <JSystem/JGeometry.hpp>
#include <JSystem/J2DGraph/J2DGrafContext.hpp>

class J3DModelData;

class J2DOrthoGraphSimple : public J2DOrthoGraph {
    public:
        J2DOrthoGraphSimple();

        ~J2DOrthoGraphSimple() {
            
        }
    
        void setPort();
    };

namespace MR {
    void drawInit();
    void reinitGX();

    void setupShadowVolumeDraw();

    void drawSimpleModel(J3DModelData*);

    void loadTexProjectionMtx(u32);

    void reinitGX();

    void fillScreen(const _GXColor&);

    void fillScreenSetup(const _GXColor &);

    void fillScreenArea(const TVec2s &, const TVec2s &);

    void clearZBuffer();

    void setDefaultViewportAndScissor();

    void resetTextureCacheSize();

    void fillSilhouetteColor();
};  // namespace MR
