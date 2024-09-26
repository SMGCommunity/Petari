#ifndef RVL_FACE_LIBRARY_INTERNAL_TEXTURE_H
#define RVL_FACE_LIBRARY_INTERNAL_TEXTURE_H
#include <RFLi_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLiTexture {
    u8 format;          // at 0x0
    u8 alpha;           // at 0x1
    u16 width;          // at 0x2
    u16 height;         // at 0x4
    u8 wrapS;           // at 0x6
    u8 wrapT;           // at 0x7
    u8 indexTexture;    // at 0x8
    u8 colorFormat;     // at 0x9
    u16 numColors;      // at 0xA
    u32 paletteOfs;     // at 0xC
    u8 enableLOD;       // at 0x10
    u8 enableEdgeLOD;   // at 0x11
    u8 enableBiasClamp; // at 0x12
    u8 enableMaxAniso;  // at 0x13
    u8 minFilt;         // at 0x14
    u8 magFilt;         // at 0x15
    s8 minLOD;          // at 0x16
    s8 maxLOD;          // at 0x16
    u8 mipmapLevel;     // at 0x18
    s8 reserved;        // at 0x19
    s16 lodBias;        // at 0x1A
    u32 imageOfs;       // at 0x1C
} RFLiTexture;

inline void* RFLiGetTexImage(const RFLiTexture* tex) {
    return (u8*)tex + tex->imageOfs;
}

#ifdef __cplusplus
}
#endif
#endif
