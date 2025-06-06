#pragma once

#include "JSystem/JUtility/JUTTexture.hpp"
#include "revolution/gx/GXStruct.h"
#include <revolution.h>
#include <JSystem/JGeometry.hpp>
#include <JSystem/J2DGraph/J2DGrafContext.hpp>

class J3DModelData;
class J3DModel;
class J3DMaterial;

class J2DOrthoGraphSimple : public J2DOrthoGraph {
public:
    J2DOrthoGraphSimple();

    ~J2DOrthoGraphSimple() {
            
    }
    
    void setPort();
};

namespace {
    GXTexObj clear_z_tobj;
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
    void clearAlphaBuffer(u8, const TVec2f &, const TVec2f &);
    void fillScreenSetup(const _GXColor &);
    void fillScreenArea(const TVec2s &, const TVec2s &);
    void fillScreen(const _GXColor &);
    void setupShadowVolumeDraw();
    void drawSimpleModel(J3DModelData*);
    void fillSilhouetteColor();
    JUTTexture* getMarioShadowTex();
    JUTTexture* getMarioShadowTexForLoad();
    TVec3f* getMarioShadowVec();
    void setMarioShadowTex(const JUTTexture *);
    void setMarioShadowVec(const TVec3f &);
    void loadTexProjectionMtx(u32);
    void nonFilteredCapture(JUTTexture *, s16, s16);
    void simpleDraw(J3DModel *, J3DMaterial *);
    void activateGameSceneDraw3D();
    void deactivateGameSceneDraw3D();  
};  // namespace MR

static _GXTexCacheSize sReinitTextureCacheSize;

JUTTexture* mShadowTex;
TVec3f mShadowVec;
