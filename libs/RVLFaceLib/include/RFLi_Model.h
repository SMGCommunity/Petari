#ifndef RVL_FACE_LIBRARY_INTERNAL_MODEL_H
#define RVL_FACE_LIBRARY_INTERNAL_MODEL_H
#include <RFL_Model.h>
#include <RFLi_Types.h>
#include <revolution/gx.h>
#include <revolution/mtx.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define VTX_COORDS_IN_POS 3
#define VTX_COORDS_IN_NRM 3
#define VTX_COORDS_IN_TXC 2

#define VTX_COORD_SIZE sizeof(s16)
#define VTX_POS_SIZE (VTX_COORD_SIZE * VTX_COORDS_IN_POS)
#define VTX_NRM_SIZE (VTX_COORD_SIZE * VTX_COORDS_IN_NRM)
#define VTX_TXC_SIZE (VTX_COORD_SIZE * VTX_COORDS_IN_TXC)

typedef struct RFLiCoordinateData {
    u8 uOff;   // at 0x0
    u8 fOff;   // at 0x1
    u8 rOff;   // at 0x2
    BOOL uRev; // at 0x4
    BOOL fRev; // at 0x8
    BOOL rRev; // at 0xC
} RFLiCoordinateData;

typedef struct RFLiCharModelRes {
    u8 noseDl[0xC0];     // at 0x0
    u8 capDl[0x560];     // at 0xC0
    u8 faceDl[0x2E0];    // at 0x620
    u8 beardDl[0x160];   // at 0x900
    u8 noselineDl[0x60]; // at 0xA60
    u8 maskDl[0x380];    // at 0xAC0
    u8 glassesDl[0x40];  // at 0xE40

    u8 faceTex[0x4000];    // at 0xE80
    u8 capTex[0x400];      // at 0x4E80
    u8 noseTex[0x400];     // at 0x5280
    u8 glassesTex[0x1000]; // at 0x5680

    s16 noseVtxPos[23 * VTX_COORDS_IN_POS]; // at 0x6680
    s16 noseVtxNrm[23 * VTX_COORDS_IN_NRM]; // at 0x670A

    s16 capVtxPos[173 * VTX_COORDS_IN_POS]; // at 0x6794
    s16 capVtxNrm[246 * VTX_COORDS_IN_NRM]; // at 0x6BA2
    s16 capVtxTxc[95 * VTX_COORDS_IN_TXC];  // at 0x7166

    s16 faceVtxPos[66 * VTX_COORDS_IN_POS];  // at 0x72E2
    s16 faceVtxNrm[66 * VTX_COORDS_IN_NRM];  // at 0x746E
    s16 faceVtxTxc[115 * VTX_COORDS_IN_TXC]; // at 0x75FA

    s16 beardVtxPos[40 * VTX_COORDS_IN_POS]; // at 0x77C6
    s16 beardVtxNrm[68 * VTX_COORDS_IN_NRM]; // at 0x78B6

    s16 noselineVtxPos[6 * VTX_COORDS_IN_POS]; // at 0x7A4E
    s16 noselineVtxNrm[2 * VTX_COORDS_IN_NRM]; // at 0x7A72
    s16 noselineVtxTxc[7 * VTX_COORDS_IN_TXC]; // at 0x7A7E

    s16 maskVtxPos[88 * VTX_COORDS_IN_POS];  // at 0x7A9A
    s16 maskVtxNrm[86 * VTX_COORDS_IN_NRM];  // at 0x7CAA
    s16 maskVtxTxc[176 * VTX_COORDS_IN_TXC]; // at 0x7EAE

    s16 glassesVtxPos[4 * VTX_COORDS_IN_POS]; // at 0x816E
    s16 glassesVtxNrm[1 * VTX_COORDS_IN_NRM]; // at 0x8186
    s16 glassesVtxTxc[4 * VTX_COORDS_IN_TXC]; // at 0x818C

    GXTexObj faceTexObj;    // at 0x819C
    GXTexObj capTexObj;     // at 0x81BC
    GXTexObj noseTexObj;    // at 0x81DC
    GXTexObj glassesTexObj; // at 0x81FC

    s16* hairVtxPos;     // at 0x821C
    s16* hairVtxNrm;     // at 0x8220
    u8* hairDl;          // at 0x8224
    s16* foreheadVtxPos; // at 0x8228
    s16* foreheadVtxNrm; // at 0x822C
    u8* foreheadDl;      // at 0x8230

    u16 noseDlSize;     // at 0x8234
    u16 faceDlSize;     // at 0x8236
    u16 hairDlSize;     // at 0x8238
    u16 capDlSize;      // at 0x823A
    u16 foreheadDlSize; // at 0x823C
    u16 beardDlSize;    // at 0x823E
    u16 noselineDlSize; // at 0x8240
    u16 maskDlSize;     // at 0x8242
    u16 glassesDlSize;  // at 0x8244

    u8 facelineColor; // at 0x8246
    u8 hairColor;     // at 0x8247
    u8 beardColor;    // at 0x8248
    u8 glassesColor;  // at 0x8249
    u8 favoriteColor; // at 0x824A

    BOOL flipHair; // at 0x824C

    char UNK_0x8250[0x10];
} RFLiCharModelRes;

typedef struct RFLiShapeRes {
    RFLiPartsShp part; // at 0x0
    u16 file;          // at 0x4
    BOOL transform;    // at 0x8
    BOOL flipX;        // at 0xC
    s16* vtxPosBuf;    // at 0x10
    s16* vtxNrmBuf;    // at 0x14
    s16* vtxTxcBuf;    // at 0x18
    u8* dlBuf;         // at 0x1C
    u16 vtxPosBufSize; // at 0x20
    u16 vtxNrmBufSize; // at 0x22
    u16 vtxTxcBufSize; // at 0x24
    u16 dlBufSize;     // at 0x26
    u16 numVtxPos;     // at 0x28
    u16 numVtxNrm;     // at 0x2A
    u16 numVtxTxc;     // at 0x2C
    u16 dlSize;        // at 0x2E
    f32 posScale;      // at 0x30
    Vec* posTrans;     // at 0x34
    Vec* noseTrans;    // at 0x38
    Vec* beardTrans;   // at 0x3C
    Vec* hairTrans;    // at 0x40
} RFLiShapeRes;

typedef struct RFLiCharModel {
    Mtx posMtx;                       // at 0x0
    Mtx nrmMtx;                       // at 0x30
    RFLExpression expression;         // at 0x60
    RFLResolution resolution;         // at 0x64
    RFLiCharModelRes* res;            // at 0x68
    GXTexObj* maskTexObj[RFLExp_Max]; // at 0x6C
} RFLiCharModel;

extern RFLiCoordinateData coordinateData;

inline void RFLiSetCoordinateData(const RFLiCoordinateData* data) {
    coordinateData = *data;
}

inline RFLiCoordinateData* RFLiGetCoordinateData(void) {
    return &coordinateData;
}

u32 RFLiGetExpressionNum(u32 exprFlags);
void RFLiInitCharModel(RFLCharModel* model, RFLiCharInfo* info, void* work,
                       RFLResolution res, u32 exprFlags);
GXColor RFLiGetFacelineColor(const RFLiCharInfo* info);
void RFLiInitCharModelRes(RFLiCharModelRes* res, const RFLiCharInfo* info);
void RFLiInitShapeRes(RFLiShapeRes* shape);
void RFLiInitTexRes(GXTexObj* texObj, RFLiPartsShpTex part, u16 file,
                    void* buffer) NO_INLINE;
void RFLiTransformCoordinate(s16* to, const s16* from);
void RFLSetModelDrawDoneCallback(RFLCallback cb);

#ifdef __cplusplus
}
#endif
#endif
