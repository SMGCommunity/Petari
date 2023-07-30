#ifndef GXTYPES_H
#define GXTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#define PHY_ADDR_MASK ~((~0x3FFF) << 16)
#define GX_PHY_ADDR(a)  ((u32)a & PHY_ADDR_MASK)

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
    GXTexRegion *(*texRegionCallback)(const GXTexObj *, GXTexMapID);
    GXTlutRegion *(*tlutRegionCallback)(u32 tlut_name);
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
    GXBool xfDisable;
    u8 dirtyVAT;
    u32 dirtyState;
} GXData;

extern GXData *const __GXData;
#define gx __GXData

extern void __GXFlushTextureState();

#ifdef __cplusplus
}
#endif

#endif // GXTYPES_H