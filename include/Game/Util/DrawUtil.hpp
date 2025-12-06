#pragma once

#include <JSystem/J2DGraph/J2DOrthoGraph.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/gx.h>

class J3DModelData;
class J3DModel;
class J3DMaterial;
class JUTTexture;

class J2DOrthoGraphSimple : public J2DOrthoGraph {
public:
    J2DOrthoGraphSimple();

    ~J2DOrthoGraphSimple() {}

    void setPort();
};

namespace MR {
    void drawInit();
    void reinitGX();
    void resetTextureCacheSize();
    void loadViewMtxFor2DModel();
    void drawInitFor2DModel();
    void setDefaultViewportAndScissor();
    void loadProjectionMtxFor2D();
    void setupDrawForNW4RLayout(f32, bool);
    void clearZBuffer();
    void clearAlphaBuffer(u8);
    void clearAlphaBuffer(u8, const TVec2f&, const TVec2f&);
    void fillScreenSetup(const GXColor&);
    void fillScreenArea(const TVec2s&, const TVec2s&);
    void fillScreen(const GXColor&);
    void setupShadowVolumeDraw();
    void drawSimpleModel(J3DModelData*);
    void fillSilhouetteColor();
    const JUTTexture* getMarioShadowTex();
    const JUTTexture* getMarioShadowTexForLoad();
    const TVec3f& getMarioShadowVec();
    void setMarioShadowTex(const JUTTexture*);
    void setMarioShadowVec(const TVec3f&);
    void loadTexProjectionMtx(u32);
    void nonFilteredCapture(JUTTexture*, s16, s16);
    void simpleDraw(J3DModel*, J3DMaterial*);
    void activateGameSceneDraw3D();
    void deactivateGameSceneDraw3D();
};  // namespace MR

static GXTexCacheSize sReinitTextureCacheSize;
