#ifndef GXTYPES_H
#define GXTYPES_H

#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXStruct.h"
#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PHY_ADDR_MASK ~((~0x3FFF) << 16)
#define GX_PHY_ADDR(a) ((u32)a & PHY_ADDR_MASK)

typedef struct _GXData {
    u16 vNumNot;
    u16 bpSentNot;
    u16 vNum;
    u16 vLim;
    u32 cpEnable;
    u32 cpStatus;
    u32 cpClr;
    u32 vcdLo;
    u32 vcdHi;
    u32 vatA[8];
    u32 vatB[8];
    u32 vatC[8];
    u32 lpSize;
    u32 matIdxA;
    u32 matIdxB;
    u32 indexBase[4];
    u32 indexStride[4];
    u32 ambColor[2];
    u32 matColor[2];
    u32 chanCtrl[4];
    u32 texGenCtrl[8];
    u32 dualTexGenCtrl[8];
    u32 suTs0[8];
    u32 suTs1[8];
    u32 suScis0;
    u32 suScis1;
    u32 tref[8];
    u32 iref;
    u32 bpMask;
    u32 IndTexScale0;
    u32 IndTexScale1;
    u32 tevc[16];
    u32 teva[16];
    u32 tevKsel[8];
    u32 cmode0;
    u32 cmode1;
    u32 zmode;
    u32 peCtrl;
    u32 cpDispSrc;
    u32 cpDispSize;
    u32 cpDispStride;
    u32 cpDisp;
    u32 cpTexSrc;
    u32 cpTexSize;
    u32 cpTexStride;
    u32 cpTex;
    GXBool cpTexZ;
    u32 genMode;
    GXTexRegion TexRegions0[8];
    GXTexRegion TexRegions1[8];
    GXTexRegion TexRegions2[8];
    GXTlutRegion TlutRegions[20];
    GXTexRegion* (*texRegionCallback)(const GXTexObj*, GXTexMapID);
    GXTlutRegion* (*tlutRegionCallback)(u32 tlut_name);
    GXAttrType nrmType;
    GXBool hasNrms;
    GXBool hasBiNrms;
    u32 projType;
    f32 projMtx[6];
    f32 vpLeft;
    f32 vpTop;
    f32 vpWd;
    f32 vpHt;
    f32 vpNearz;
    f32 vpFarz;
    f32 zOffset;
    f32 zScale;
    u32 tImage0[8];
    u32 tMode0[8];
    u32 texmapId[16];
    u32 tcsManEnab;
    u32 tevTcEnab;
    GXPerf0 perf0;
    GXPerf1 perf1;
    u32 perfSel;
    GXBool inDispList;
    GXBool dlSaveContext;
    GXBool abtWaitPECopy;
    u8 dirtyVAT;
    u32 dirtyState;
} GXData;

extern GXData* const __GXData;
#define gx __GXData

#define BP_SENT() gx->bpSentNot = GX_FALSE;
#define XF_SENT() gx->bpSentNot = GX_TRUE;

extern void __GXFlushTextureState();
extern void __GXAbort();
extern void __GXSetTmemConfig(u32);

extern void __GetImageTileCount(GXTexFmt fmt, u16 wd, u16 ht, u32* rowTiles, u32* colTiles, u32* cmpTiles);

typedef struct __GXLightObjInt_struct {
    u32 reserved[3];
    u32 Color;
    f32 a[3];
    f32 k[3];
    f32 lpos[3];
    f32 ldir[3];
} GXLightObjInt;

typedef struct __GXTexObjInt_struct {
    u32 mode0;
    u32 mode1;
    u32 image0;
    u32 image3;
    void* userData;
    GXTexFmt fmt;
    u32 tlutName;
    u16 loadCnt;
    u8 loadFmt;
    u8 flags;
} GXTexObjInt;

typedef struct __GXTexRegionInt_struct {
    u32 image1;
    u32 image2;
    u16 sizeEven;
    u16 sizeOdd;
    u8 is32bMipmap;
    u8 isCached;
} GXTexRegionInt;

typedef struct __GXTlutObjInt_struct {
    u32 tlut;
    u32 loadTlut0;
    u16 numEntries;
} GXTlutObjInt;

typedef struct __GXTlutRegionInt_struct {
    u32 loadTlut1;
    GXTlutObjInt tlutObj;
} GXTlutRegionInt;

#define GX_SETUP_LIGHT(l, p)                                                                                                                         \
    GXLightObjInt* l;                                                                                                                                \
    l = (GXLightObjInt*)p;

#define GX_SETUP_TEXOBJ(l, p) GXTexObjInt* l = (GXTexObjInt*)p;

#define GX_SETUP_ALL_TEXOBJS(l, p, m, q)                                                                                                             \
    GXTexObjInt* l = (GXTexObjInt*)p;                                                                                                                \
    GXTexRegionInt* m = (GXTexRegionInt*)q;

#define GX_SETUP_TLUTOBJ(l, p) GXTlutObjInt* l = (GXTlutObjInt*)p;

#define GX_SETUP_TREGOBJ(l, p) GXTexRegionInt* l = (GXTexRegionInt*)p;

#define GX_SETUP_TLUTREGOBJ(l, p) GXTlutRegionInt* l = (GXTlutRegionInt*)p;

#ifdef __cplusplus
}
#endif

#endif  // GXTYPES_H
