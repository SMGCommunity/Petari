#include "revolution/gd/GDBase.h"
#include "revolution/gd/GDGeometry.h"
#include "revolution/gx.h"

#ifdef __cplusplus
extern "C" {
#endif

void GDSetAlphaCompare(GXCompare, u8, GXAlphaOp, GXCompare, u8);
void GDSetZMode(GXBool, GXCompare, GXBool);
void GDSetTevDirect(GXTevStageID);
void GDSetGenMode(u8, u8, u8);
void GDSetChanCtrl(GXChannelID, GXBool, GXColorSrc, GXColorSrc, u32, GXDiffuseFn, GXAttnFn);
void GDSetTevAlphaCalcAndSwap(GXTevStageID, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg, GXTevOp, GXTevBias, GXTevScale, GXBool,
                              GXTevRegID, GXTevSwapSel, GXTevSwapSel);
void GDSetTevColor(GXTevRegID, GXColor);

void GDSetCullMode(GXCullMode mode);
void GDSetGenMode2(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm);
void GDSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp logic_op, GXBool color_update_enable,
                       GXBool alpha_update_enable, GXBool dither_enable);
void GDSetDstAlpha(GXBool enable, u8 alpha);
void GDSetTevColorCalc(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d, GXTevOp op, GXTevBias bias,
                       GXTevScale scale, GXBool clamp, GXTevRegID out_reg);
void GDSetTevOrder(GXTevStageID evenStage, GXTexCoordID coord0, GXTexMapID map0, GXChannelID color0, GXTexCoordID coord1, GXTexMapID map1,
                   GXChannelID color1);
void GDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, GXTexFilter min_filt, GXTexFilter mag_filt, f32 min_lod,
                        f32 max_lod, f32 lod_bias, GXBool bias_clamp, GXBool do_edge_lod, GXAnisotropy max_aniso);
void GDSetTexImgAttr(GXTexMapID id, u16 width, u16 height, GXTexFmt format);
void GDSetTexImgPtr(GXTexMapID id, void* image_ptr);
void GDSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func, GXTexGenSrc src_param, GXBool normalize, u32 postmtx);
void GDSetChanAmbColor(GXChannelID chan, GXColor color);
void GDSetChanMatColor(GXChannelID chan, GXColor color);
void GDSetBlendMode(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp);
void GDSetFog(GXFogType, f32, f32, f32, f32, GXColor);

#ifdef __cplusplus
}
#endif
