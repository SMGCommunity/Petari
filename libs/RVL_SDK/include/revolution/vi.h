#ifndef VI_H
#define VI_H

#include <revolution/vi/vi3in1.h>
#include <revolution/vi/vi3in1types.h>
#include <revolution/vi/vifuncs.h>
#include <revolution/vi/vitypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u8 equ;
    u16 acv;
    u16 prbOdd;
    u16 prbEven;
    u16 psbOdd;
    u16 psbEven;
    u8 bs1;
    u8 bs2;
    u8 bs3;
    u8 bs4;
    u16 be1;
    u16 be2;
    u16 be3;
    u16 be4;
    u16 nhlines;
    u16 hlw;
    u8 hsy;
    u8 hcs;
    u8 hce;
    u8 hbe640;
    u16 hbs640;
    u8 hbeCCIR656;
    u16 hbsCCIR656;
} timing_s;

typedef struct {
    u16 DispPosX;
    u16 DispPosY;
    u16 DispSizeX;
    u16 DispSizeY;
    u16 AdjustedDispPosX;
    u16 AdjustedDispPosY;
    u16 AdjustedDispSizeY;
    u16 AdjustedPanPosY;
    u16 AdjustedPanSizeY;
    u16 FBSizeX;
    u16 FBSizeY;
    u16 PanPosX;
    u16 PanPosY;
    u16 PanSizeX;
    u16 PanSizeY;
    VIXFBMode FBMode;
    u32 nonInter;
    u32 tv;
    u8 wordPerLine;
    u8 std;
    u8 wpl;
    u32 bufAddr;
    u32 tfbb;
    u32 bfbb;
    u8 xof;
    BOOL black;
    BOOL threeD;
    u32 rbufAddr;
    u32 rtfbb;
    u32 rbfbb;
    timing_s* timing;
} HorVer_s;

typedef enum _VITiming { VI_TMG_GAME = 0, VI_TMG_DVD = 1 } VITiming;

typedef enum _VIVideo { VI_VMODE_NTSC = 0, VI_VMODE_MPAL = 1, VI_VMODE_PAL = 2, VI_VMODE_RGB = 3 } VIVideo;

typedef struct VIGammaObj {
    u16 a[6];
    u8 yin[7];
    u16 yout[7];
} VIGammaObj;

typedef enum _VIACPType { VI_ACP_OFF = 1, VI_ACP_TYPE1 = 2, VI_ACP_TYPE2 = 3, VI_ACP_TYPE3 = 4 } VIACPType;

typedef enum _VIOverDrive { VI_ODV_L1 = 0, VI_ODV_L2 = 1, VI_ODV_L3 = 2, VI_ODV_L4 = 3, VI_ODV_L5 = 4, VI_ODV_L6 = 5 } VIOverDrive;

typedef struct VIMacroVisionObj {
    u8 m[26];
} VIMacroVisionObj;

typedef void (*VIPositionCallback)(s16, s16);

extern VIMacroVisionObj VINtscACPType1;
extern VIMacroVisionObj VINtscACPType2;
extern VIMacroVisionObj VINtscACPType3;
extern VIMacroVisionObj VIPalACPType1;
extern VIMacroVisionObj VIPalACPType2;
extern VIMacroVisionObj VIPalACPType3;
extern VIMacroVisionObj VIEurgb60ACPType1;
extern VIMacroVisionObj VIEurgb60ACPType2;
extern VIMacroVisionObj VIEurgb60ACPType3;
extern VIMacroVisionObj VIMpalACPType1;
extern VIMacroVisionObj VIMpalACPType2;
extern VIMacroVisionObj VIMpalACPType3;
extern VIMacroVisionObj VIProgressiveACPType;
extern VIMacroVisionObj VIZeroACPType;

void __VIGetCurrentPosition(s16*, s16*);
void __VISetRevolutionModeSimple();
BOOL __VIResetRFIdle(void);

#define VI_3D 3
#define VI_TVMODE_NTSC_3D ((VITVMode)VI_TVMODE(VI_NTSC, VI_3D))
#define VI_GCA 6
#define VI_TVMODE_GCA_INT (VITVMode) VI_TVMODE(VI_GCA, VI_INTERLACE)
#define VI_TVMODE_GCA_PROG (VITVMode) VI_TVMODE(VI_GCA, VI_PROGRESSIVE)
#define VI_TVMODE_PAL_PROG (VITVMode)6

#define VI_EXTRA 7
#define VI_TVMODE_EXTRA_INT ((VITVMode)VI_TVMODE(VI_EXTRA, VI_INTERLACE))
#define VI_TVMODE_EXTRA_DS ((VITVMode)VI_TVMODE(VI_EXTRA, VI_NON_INTERLACE))
#define VI_TVMODE_EXTRA_PROG ((VITVMode)VI_TVMODE(VI_EXTRA, VI_PROGRESSIVE))

#define VI_HD720 8
#define VI_TVMODE_HD720_PROG ((VITVMode)VI_TVMODE(VI_HD720, VI_PROGRESSIVE))

#ifdef __cplusplus
}
#endif

#endif  // VI_H
