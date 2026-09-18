#pragma once

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

u32 ODHEncodeRGB565(u8*, u8*, int, int, u32, int, u8*);

#ifdef __cplusplus
}
#endif

struct SArCDJ_HuffmanRequest {
    /* 0x00 */ s32* mPreviousDC[2];
    /* 0x08 */ const s32* mValueTable;
    /* 0x0C */ const s32* mRunTable;
    /* 0x10 */ u8* mOutput;
    /* 0x14 */ u32* mRemaining;
    /* 0x18 */ u32* mBitBuffer;
    /* 0x1C */ u32* mFreeBits;
    /* 0x20 */ u32 mLimitSize;
};

struct SArCDJ_OdhMaster {
    /* 0x000 */ u16 mSize[2];
    /* 0x004 */ u8 mQuality;
    /* 0x006 */ u16 mBlocksX;
    /* 0x008 */ u16 mBlocksY;
    /* 0x00A */ u16 mBlockX;
    /* 0x00C */ u16 mBlockY;
    /* 0x010 */ u32 mBitBuffer;
    /* 0x014 */ u32 mFreeBits;
    /* 0x018 */ u32 mLimitSize;
    /* 0x01C */ u32 mRemaining;
    /* 0x020 */ s32 mPreviousDC[3];
    /* 0x02C */ u8* mWork;
    /* 0x030 */ u32 mCoefficients[64];
    /* 0x130 */ u32 mDct[64];
    /* 0x230 */ u8* mOutput;
    /* 0x234 */ u32 mQuantization[2][64];
    /* 0x434 */ SArCDJ_HuffmanRequest mLumaRequest;
    /* 0x458 */ SArCDJ_HuffmanRequest mChromaRequest;
};

class CArGBAOdh {
public:
    u32 compressGbaOdh(u8*, u8*, int, int, int, u32, u8*, int);

private:
    /* 0x00 */ u8* mOutputCursor;

    u32 cdj_c_initializeCompressOdh(SArCDJ_OdhMaster*, u16*, u8, u8*, u8*, u32);
    u32 cdj_c_compressLoop(SArCDJ_OdhMaster*);
    u32 cdj_c_flashBuffer(SArCDJ_OdhMaster*);
    void cdj_c_setQuantizationTable(SArCDJ_OdhMaster*, u32);
    void cdj_c_makeHeader(SArCDJ_OdhMaster*, u32);
    u32 cdj_c_colorConv(SArCDJ_OdhMaster*, u8*, int);
    void LineConv11(u8*, u8*, u8*, u8*, u16, u16, const s32*, int);
    void fdct_fast(u32*, u8*, u32, u32*);
    u32 huffmanCoder(u16*, SArCDJ_HuffmanRequest*);
    u32 EmitBit(s32, s32, SArCDJ_HuffmanRequest*);
};
