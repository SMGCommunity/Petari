#pragma once

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

u32 ODHEncodeRGB565(u8*, u8*, int, int, u32, int, u8*);

#ifdef __cplusplus
}
#endif

class SArCDJ_OdhMaster {
private:
    /* 0x000 */ u8 _0[0x480];
};
class SArCDJ_HuffmanRequest;

class CArGBAOdh {
public:
    u32 compressGbaOdh(u8*, u8*, int, int, int, u32, u8*, int);

private:
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
