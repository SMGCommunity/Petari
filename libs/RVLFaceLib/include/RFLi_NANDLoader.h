#ifndef RVL_FACE_LIBRARY_INTERNAL_NAND_LOADER_H
#define RVL_FACE_LIBRARY_INTERNAL_NAND_LOADER_H
#include <RFL_NANDLoader.h>
#include <RFLi_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct RFLiTexture;

typedef struct RFLiArchive {
    u16 numFiles;    // at 0x0
    u16 biggestSize; // at 0x2
    u32 offset;      // at 0x4
} RFLiArchive;

typedef struct RFLiLoader {
    RFLiArchive archives[RFLiArcID_Max]; // at 0x0
    u32 cacheSize;                       // at 0x90
    void* cache;                         // at 0x94
    BOOL cached;                         // at 0x98
    u16 version;                         // at 0x9C
    void* headerBuf1;                    // at 0xA0
    void* headerBuf2;                    // at 0xA4
    u8 numResources;                     // at 0xA8
} RFLiLoader;

void RFLiInitLoader(void);
RFLErrcode RFLiLoadResourceHeaderAsync(void);
u32 RFLiGetTexSize(RFLiPartsTex part, u16 file);
struct RFLiTexture* RFLiLoadTexture(RFLiPartsTex part, u16 file, void* dst);
u32 RFLiGetShpTexSize(RFLiPartsShpTex part, u16 file);
struct RFLiTexture* RFLiLoadShpTexture(RFLiPartsShpTex part, u16 file,
                                       void* dst);
u32 RFLiGetShapeSize(RFLiPartsShp part, u16 file);
void* RFLiLoadShape(RFLiPartsShp part, u16 file, void* dst);

#ifdef __cplusplus
}
#endif
#endif
