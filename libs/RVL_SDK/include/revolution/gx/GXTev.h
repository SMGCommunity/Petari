#ifndef GXTEV_H
#define GXTEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

void GXSetTevOp(GXTevStageID, GXTevMode);
void GXSetTevColorIn(GXTevStageID, GXTevColorArg, GXTevColorArg, GXTevColorArg, GXTevColorArg);
void GXSetTevAlphaIn(GXTevStageID, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg);
void GXSetTevColorOp(GXTevStageID, GXTevOp, GXTevBias, GXTevScale, GXBool, GXTevRegID);
void GXSetTevAlphaOp(GXTevStageID, GXTevOp, GXTevBias, GXTevScale, GXBool, GXTevRegID);
void GXSetTevColor(GXTevRegID, GXColor);
void GXSetTevColorS10(GXTevRegID, GXColorS10);

void GXSetAlphaCompare(GXCompare, u8, GXAlphaOp, GXCompare, u8);
void GXSetTevOrder(GXTevStageID, GXTexCoordID, GXTexMapID, GXChannelID);
void GXSetZTexture(GXZTexOp, GXTexFmt, u32);
void GXSetNumTevStages(u8);

void GXSetTevKColor(GXTevKColorID, GXColor);
void GXSetTevKColorSel(GXTevStageID, GXTevKColorSel);
void GXSetTevKAlphaSel(GXTevStageID, GXTevKAlphaSel);

void GXSetTevSwapModeTable(GXTevSwapSel, GXTevColorChan, GXTevColorChan, GXTevColorChan, GXTevColorChan);

void GXSetTevSwapMode(GXTevStageID, GXTevSwapSel, GXTevSwapSel);

#ifdef __cplusplus
}
#endif

#endif // GXTEV_H