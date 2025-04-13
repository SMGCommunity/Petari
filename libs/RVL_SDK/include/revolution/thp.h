#ifndef THP_H
#define THP_H

#include <revolution.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u8 THPSample;
typedef s16 THPCoeff;
typedef float THPQuantTab[64];

typedef struct _THPHuffmanTab {
    u8 quick[32];
    u8 increment[32];
    u8* Vij;
    s32 maxCode[18];
    s32 valPtr[18];
    u8 Vij1;
    u8 pad[11];
} THPHuffmanTab;

typedef struct _THPComponent {
    u8 quantizationTableSelector;
    u8 DCTableSelector;
    u8 ACTableSelector;
    THPCoeff predDC;
} THPComponent;

typedef struct _THPFileInfo {
    THPQuantTab quantTabs[3];
    THPHuffmanTab huffmanTabs[4];
    THPComponent components[3];
    u16 xPixelSize;
    u16 yPixelSize;
    u16 MCUsPerRow;
    u16 decompressedY;
    u8* c;
    u32 currByte;
    u32 cnt;
    u8 validHuffmanTabs;
    u8 RST;
    u16 nMCU;
    u16 currMCU;
    u8* dLC[3];
} THPFileInfo;

typedef struct {
  u32 offsetNextChannel;
  u32 sampleSize;
  s16 lCoef[8][2];
  s16 rCoef[8][2];
  s16 lYn1;
  s16 lYn2;
  s16 rYn1;
  s16 rYn2;
} THPAudioRecordHeader;

typedef struct {
  u8* encodeData;
  u32 offsetNibbles;
  u8 predictor;
  u8 scale;
  s16 yn1;
  s16 yn2;
} THPAudioDecodeInfo;

typedef struct {
    char magic[4];
    u32 version;
    u32 bufSize;
    u32 audioMaxSamples;
    f32 frameRate;
    u32 numFrames;
    u32 firstFrameSize;
    u32 movieDataSize;           
    u32 compInfoDataOffsets;
    u32 offsetDataOffsets;
    u32 movieDataOffsets;
    u32 finalFrameDataOffsets;
} THPHeader;

typedef struct {
    u32 xSize;
    u32 ySize;
    u32 videoType;
} THPVideoInfo;

typedef struct {
    u32 numComponents;
    u8 frameComp[16];
} THPFrameCompInfo;

typedef struct {
    u32 sndChannels;
    u32 sndFrequency;
    u32 sndNumSamples;
    u32 sndNumTracks;
} THPAudioInfo;

typedef struct {
    u8* ptr;
    s32 frameNumber;
    volatile BOOL isValid;
} THPReadBuffer;

typedef struct {
    u8* ytexture;
    u8* utexture;
    u8* vtexture;
    s32 frameNumber;
} THPTextureSet;

typedef struct {
    s16* buffer;
    s16* curPtr;
    u32 validSample;
} THPAudioBuffer;

s32 __THPAudioGetNewSample(THPAudioDecodeInfo *);
void __THPAudioInitialize(THPAudioDecodeInfo *, u8 *);

void __THPSetupBuffers(void);
u8 __THPReadFrameHeader(void);
u8 __THPReadScaneHeader(void);
u8 __THPReadQuantizationTable(void);
u8 __THPReadHuffmanTableSpecification(void);
void __THPHuffGenerateSizeTable(void);
void __THPHuffGenerateCodeTable(void);
void __THPHuffGenerateDecoderTables(u8 tabIndex);
void __THPRestartDefinition(void);
void __THPPrepBitStream(void);
static void __THPDecompressYUV(void *, void *, void *);
void __THPGQRRestore(void);
void __THPDecompressiMCURow512x448(void);
void __THPDecompressiMCURow640x480(void);
void __THPDecompressiMCURowNxN(void);
void __THPInverseDCTNoYPos(THPCoeff *, u32);
void __THPHuffDecodeDCTCompY(THPFileInfo *, THPCoeff *);
void __THPHuffDecodeDCTCompU(THPFileInfo *, THPCoeff *);
void __THPHuffDecodeDCTCompV(THPFileInfo *, THPCoeff *);

s32 THPVideoDecode(void *file, void *tileY, void *tileU, void *tileV, void *work);
u32 THPAudioDecode(s16 *, u8 *, s32);

BOOL THPInit(void);

void THPGXYuv2RgbSetup(GXRenderModeObj *);
void THPGXYuv2RgbDraw(u8 *, u8 *, u8 *, s16, s16, s16, s16, s16, s16); 
void THPGXRestore(void);

void THPGXSetTexObjFilter(GXTexFilter);

static const u8 __THPJpegNaturalOrder[80] = {
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63,
    63, 63, 63, 63, 63, 63, 63, 63,
    63, 63, 63, 63, 63, 63, 63, 63
};

static const f64 __THPAANScaleFactor[8] = {
    1.0f, 1.387039845f, 1.306562965f, 1.175875602f,
    1.0f, 0.785694958f, 0.541196100f, 0.275899379f,
};

#ifdef __cplusplus
}
#endif

#endif // THP_H
