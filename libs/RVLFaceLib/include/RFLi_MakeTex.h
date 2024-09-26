#ifndef RVL_FACE_LIBRARY_INTERNAL_MAKE_TEX_H
#define RVL_FACE_LIBRARY_INTERNAL_MAKE_TEX_H
#include <RFLi_Types.h>
#include <revolution/gx.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct RFLiTexture;

typedef enum {
    RFL_ORIGIN_CENTER,
    RFL_ORIGIN_LEFT,
    RFL_ORIGIN_RIGHT
} RFL_ORIGIN;

typedef enum {
    RFLi_MASKRSL_32 = 32,
    RFLi_MASKRSL_64 = 64,
    RFLi_MASKRSL_128 = 128,
    RFLi_MASKRSL_256 = 256
} RFLi_MASKRSL;

typedef struct RFLiTexObj {
    struct RFLiTexture* texture; // at 0x0
    u8* image;                   // at 0x4
    GXTexObj tobj;               // at 0x8
} RFLiTexObj;

typedef struct RFLiPart {
    f32 x;             // at 0x0
    f32 y;             // at 0x4
    f32 width;         // at 0x8
    f32 height;        // at 0xC
    f32 angle;         // at 0x10
    RFL_ORIGIN origin; // at 0x14
    RFLiTexObj ngtobj; // at 0x18
} RFLiPart;

typedef struct RFLiFaceParts {
    RFLiPart eyeR;      // at 0x0
    RFLiPart eyeL;      // at 0x40
    RFLiPart eyebrowR;  // at 0x80
    RFLiPart eyebrowL;  // at 0xC0
    RFLiPart mouth;     // at 0x100
    RFLiPart mustacheR; // at 0x140
    RFLiPart mustacheL; // at 0x180
    RFLiPart mole;      // at 0x1C0
} RFLiFaceParts;

void RFLiSetupCopyTex(GXTexFmt fmt, u16 width, u16 height, void* buffer,
                      GXColor clearColor) NO_INLINE;
void RFLiMakeTexture(const RFLiCharInfo* info, u8** buffer,
                     RFLResolution resolution);
void RFLiInitRFLTexture(RFLiTexObj* tobj);
void RFLiSetup2DCameraAndParam(void);
void RFLiSetTev4Mole(void);
void RFLiSetTev4Mouth(u32 color);
void RFLiSetTev4Eye(u32 color, u32 type);
void RFLiSetTev4Eyebrow(u32 color);
void RFLiSetTev4Mustache(u32 color);
void RFLiSetFaceParts(const RFLiCharInfo* info, RFLiFaceParts* face,
                      RFLi_MASKRSL resolution);
void RFLiCapture(u8* buffer, const RFLiCharInfo* info, RFLiFaceParts* face,
                 RFLi_MASKRSL resolution);
void RFLiDrawFaceParts(RFLiPart* part);
void RFLiDrawQuad(f32 x, f32 y, f32 width, f32 height, f32 rotZ,
                  RFL_ORIGIN origin);
RFLi_MASKRSL RFLiGetMaxMaskRsl(RFLResolution resolution);
u32 RFLiGetMaskSize(RFLi_MASKRSL resolution);
u32 RFLiGetMaskBufSize(RFLResolution resolution);

#ifdef __cplusplus
}
#endif
#endif
