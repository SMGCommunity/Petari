#include "Game/MapObj/FallOutFieldDraw.hpp"
#include "Game/Util/Color.hpp"

/*
void FallOutFieldDraw::setUpFillScreen() const {
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1u, GX_TEVPREV);
    //Color8 color = ;
    //Color8 color(0, 0, 0, 1);
    GXSetTevKColor(GX_KCOLOR0, Color8(0, 0, 0, 1));
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevAlphaIn(GX_TEVSTAGE0, (_C) ? GX_CA_KONST : GX_CA_TEXA, (_C) ? GX_CA_KONST : GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_RGB8_GT, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(1u, GX_ALWAYS, 1u);
    GXSetZCompLoc(0);
    GXSetColorUpdate(1u);
    GXSetAlphaUpdate(0);
    GXSetZScaleOffset(0.0f, 1.0f); 
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);

}*/