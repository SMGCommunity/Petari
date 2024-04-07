#ifndef RVL_FACE_LIBRARY_MODEL_H
#define RVL_FACE_LIBRARY_MODEL_H
#include <RFL_Types.h>
#include <revolution/gx.h>
#include <revolution/mtx.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct RFLMiddleDB;

typedef enum {
    RFLCoordinateType_X = 0x01000000,
    RFLCoordinateType_Y = 0x00010000,
    RFLCoordinateType_Z = 0x00000100,

    RFLCoordinateType_RevX = 0x11000000,
    RFLCoordinateType_RevY = 0x00110000,
    RFLCoordinateType_RevZ = 0x00001100,

    RFLCoordinateType_RevMask = 0x10101000
} RFLCoordinateType;

typedef struct RFLCharModel {
    u8 dummy[0x88];
} RFLCharModel;

typedef struct RFLDrawSetting {
    u8 lightEnable;      // at 0x0
    GXLightID lightMask; // at 0x4
    GXDiffuseFn diffuse; // at 0x8
    GXAttnFn attn;       // at 0xC
    GXColor ambColor;    // at 0x10
    u8 compLoc;          // at 0x14
} RFLDrawSetting;

typedef struct RFLDrawCoreSetting {
    u8 txcGenNum;              // at 0x0
    GXTexCoordID txcID;        // at 0x4
    GXTexMapID texMapID;       // at 0x8
    u8 tevStageNum;            // at 0xC
    GXTevSwapSel tevSwapTable; // at 0x10
    GXTevKColorID tevKColorID; // at 0x14
    GXTevRegID tevOutRegID;    // at 0x18
    GXPosNrmMtx posNrmMtxID;   // at 0x1C
    u8 reverseCulling;         // at 0x20
} RFLDrawCoreSetting;

void RFLSetCoordinate(RFLCoordinateType t1, RFLCoordinateType t2);
u32 RFLGetModelBufferSize(RFLResolution res, u32 exprFlags);
RFLErrcode RFLInitCharModel(RFLCharModel* model, RFLDataSource src,
                            struct RFLMiddleDB* db, u16 id, void* work,
                            RFLResolution res, u32 exprFlags);
void RFLSetMtx(RFLCharModel* model, const Mtx mvMtx);
void RFLSetExpression(RFLCharModel* model, RFLExpression expr);
RFLExpression RFLGetExpression(const RFLCharModel* model);
GXColor RFLGetFavoriteColor(RFLFavoriteColor color);
void RFLLoadDrawSetting(const RFLDrawSetting* setting);
void RFLDrawOpa(const RFLCharModel* model);
void RFLDrawXlu(const RFLCharModel* model);
void RFLLoadVertexSetting(const RFLDrawCoreSetting* setting);
void RFLLoadMaterialSetting(const RFLDrawCoreSetting* setting)
    NO_INLINE;
void RFLDrawOpaCore(const RFLCharModel* model,
                    const RFLDrawCoreSetting* setting);
void RFLDrawXluCore(const RFLCharModel* model,
                    const RFLDrawCoreSetting* setting);
void RFLDrawShape(const RFLCharModel* model);

#ifdef __cplusplus
}
#endif
#endif
