#include "Game/Screen/odh.hpp"

static const u8 sZigZag[] = {
    0,  1,  5,  6,  14, 15, 27, 28, 2,  4,  7,  13, 16, 26, 29, 42, 3,  8,  12, 17, 25, 30, 41, 43, 9,  11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54, 20, 22, 33, 38, 46, 51, 55, 60, 21, 34, 37, 47, 50, 56, 59, 61, 35, 36, 48, 49, 57, 58, 62, 63,
};

static const u8 sQuantization[] = {
    16, 11, 10, 16, 24, 40,  51,  61, 12, 12, 14, 19, 26, 58,  60,  55, 14, 13, 16, 24, 40,  57,  69,  56,  14, 17, 22, 29, 51,  87,  80,  62,
    18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99,
    17, 18, 24, 47, 99, 99,  99,  99, 18, 21, 26, 66, 99, 99,  99,  99, 24, 26, 56, 99, 99,  99,  99,  99,  47, 66, 99, 99, 99,  99,  99,  99,
    99, 99, 99, 99, 99, 99,  99,  99, 99, 99, 99, 99, 99, 99,  99,  99, 99, 99, 99, 99, 99,  99,  99,  99,  99, 99, 99, 99, 99,  99,  99,  99,
};

static const u16 sDctScale[] = {
    16384, 22725, 21407, 19266, 16384, 12873, 8867,  4520, 22725, 31521, 29692, 26722, 22725, 17855, 12299, 6270,
    21407, 29692, 27969, 25172, 21407, 16819, 11585, 5906, 19266, 26722, 25172, 22654, 19266, 15137, 10426, 5315,
    16384, 22725, 21407, 19266, 16384, 12873, 8867,  4520, 12873, 17855, 16819, 15137, 12873, 10114, 6967,  3552,
    8867,  12299, 11585, 10426, 8867,  6967,  4799,  2446, 4520,  6270,  5906,  5315,  4520,  3552,  2446,  1247,
};

static const s32 gArDC_L_Table[] = {
    33554432, 50331650, 50331651, 50331652, 50331653, 50331654, 67108878, 83886110, 100663358, 117440638, 134217982, 150995454, 0, 0, 0, 0,
};

static const s32 gArDC_C_Table[] = {
    33554432, 33554433, 33554434, 50331654, 67108878, 83886110, 100663358, 117440638, 134217982, 150995454, 167773182, 184551422, 0, 0, 0, 0,
};

static const s32 gArConvPlttTbl[] = {
    0,        156760,   313520,   470280,   627040,   783800,   940560,   1097320,  1254080,  1410840,  1567600,  1724360,  1881120,  2037880,
    2194640,  2351400,  2508160,  2664920,  2821680,  2978440,  3135200,  3291960,  3448720,  3605480,  3762240,  3919000,  4075760,  4232520,
    4389280,  4546040,  4702800,  4859560,  0,        307760,   615520,   923280,   1231040,  1538800,  1846560,  2154320,  2462080,  2769840,
    3077600,  3385360,  3693120,  4000880,  4308640,  4616400,  4924160,  5231920,  5539680,  5847440,  6155200,  6462960,  6770720,  7078480,
    7386240,  7694000,  8001760,  8309520,  8617280,  8925040,  9232800,  9540560,  32768,    92536,    152304,   212072,   271840,   331608,
    391376,   451144,   510912,   570680,   630448,   690216,   749984,   809752,   869520,   929288,   989056,   1048824,  1108592,  1168360,
    1228128,  1287896,  1347664,  1407432,  1467200,  1526968,  1586736,  1646504,  1706272,  1766040,  1825808,  1885576,  0,        -88472,
    -176944,  -265416,  -353888,  -442360,  -530832,  -619304,  -707776,  -796248,  -884720,  -973192,  -1061664, -1150136, -1238608, -1327080,
    -1415552, -1504024, -1592496, -1680968, -1769440, -1857912, -1946384, -2034856, -2123328, -2211800, -2300272, -2388744, -2477216, -2565688,
    -2654160, -2742632, 0,        -173672,  -347344,  -521016,  -694688,  -868360,  -1042032, -1215704, -1389376, -1563048, -1736720, -1910392,
    -2084064, -2257736, -2431408, -2605080, -2778752, -2952424, -3126096, -3299768, -3473440, -3647112, -3820784, -3994456, -4168128, -4341800,
    -4515472, -4689144, -4862816, -5036488, -5210160, -5383832, 8421375,  8683519,  8945663,  9207807,  9469951,  9732095,  9994239,  10256383,
    10518527, 10780671, 11042815, 11304959, 11567103, 11829247, 12091391, 12353535, 12615679, 12877823, 13139967, 13402111, 13664255, 13926399,
    14188543, 14450687, 14712831, 14974975, 15237119, 15499263, 15761407, 16023551, 16285695, 16547839, 8421375,  8683519,  8945663,  9207807,
    9469951,  9732095,  9994239,  10256383, 10518527, 10780671, 11042815, 11304959, 11567103, 11829247, 12091391, 12353535, 12615679, 12877823,
    13139967, 13402111, 13664255, 13926399, 14188543, 14450687, 14712831, 14974975, 15237119, 15499263, 15761407, 16023551, 16285695, 16547839,
    0,        -219512,  -439024,  -658536,  -878048,  -1097560, -1317072, -1536584, -1756096, -1975608, -2195120, -2414632, -2634144, -2853656,
    -3073168, -3292680, -3512192, -3731704, -3951216, -4170728, -4390240, -4609752, -4829264, -5048776, -5268288, -5487800, -5707312, -5926824,
    -6146336, -6365848, -6585360, -6804872, 0,        -42632,   -85264,   -127896,  -170528,  -213160,  -255792,  -298424,  -341056,  -383688,
    -426320,  -468952,  -511584,  -554216,  -596848,  -639480,  -682112,  -724744,  -767376,  -810008,  -852640,  -895272,  -937904,  -980536,
    -1023168, -1065800, -1108432, -1151064, -1193696, -1236328, -1278960, -1321592,
};

u32 ODHEncodeRGB565(u8* pSrc, u8* pDst, int width, int height, u32 limitSize, int quality, u8* pWork) {
    CArGBAOdh odh;

    return odh.compressGbaOdh(pSrc, pDst, width, height, quality, limitSize, pWork, 0);
}

u32 CArGBAOdh::compressGbaOdh(u8* pSrc, u8* pDst, int width, int height, int quality, u32 limitSize, u8* pWork, int format) {
    if (limitSize == 0) {
        limitSize = width * height * 2;
    }

    SArCDJ_OdhMaster master;
    u16 size[2] = {width, height};

    if (cdj_c_initializeCompressOdh(&master, size, quality, pWork, pDst, limitSize) != 0) {
        return 0;
    }

    if (cdj_c_colorConv(&master, pSrc, format) != 0) {
        return 0;
    }

    u32 result = cdj_c_compressLoop(&master);

    while (result == 0x80000004) {
        quality -= 5;

        if (quality > 0) {
            if (cdj_c_initializeCompressOdh(&master, size, quality, pWork, pDst, limitSize) != 0) {
                return 0;
            }

            result = cdj_c_compressLoop(&master);
            continue;
        }

        return 0;
    }

    return result;
}

u32 CArGBAOdh::cdj_c_initializeCompressOdh(SArCDJ_OdhMaster* pMaster, u16* pSize, u8 quality, u8* pWork, u8* pOutput, u32 limitSize) {
    if (pSize[0] == 0 || pSize[0] > 2047 || pSize[1] == 0 || pSize[1] > 2047) {
        return 0x80000001;
    }

    if (quality > 100) {
        return 0x80000002;
    }

    pMaster->mWork = pWork;
    pMaster->mOutput = pOutput;
    pMaster->mSize[0] = pSize[0];
    pMaster->mSize[1] = pSize[1];
    pMaster->mQuality = quality;
    pMaster->mBlocksX = (pSize[0] - 1) / 8 + 1;
    pMaster->mBlocksY = (pSize[1] - 1) / 8 + 1;
    pMaster->mLumaRequest.mPreviousDC[0] = &pMaster->mPreviousDC[0];
    pMaster->mLumaRequest.mPreviousDC[1] = &pMaster->mPreviousDC[0];
    pMaster->mLumaRequest.mValueTable = gArDC_L_Table;
    pMaster->mLumaRequest.mRunTable = gArDC_L_Table;
    pMaster->mLumaRequest.mOutput = pOutput;
    pMaster->mLumaRequest.mRemaining = &pMaster->mRemaining;
    pMaster->mLumaRequest.mBitBuffer = &pMaster->mBitBuffer;
    pMaster->mLumaRequest.mFreeBits = &pMaster->mFreeBits;
    pMaster->mLumaRequest.mLimitSize = limitSize;
    pMaster->mChromaRequest.mPreviousDC[0] = &pMaster->mPreviousDC[1];
    pMaster->mChromaRequest.mPreviousDC[1] = &pMaster->mPreviousDC[2];
    pMaster->mChromaRequest.mValueTable = gArDC_C_Table;
    pMaster->mChromaRequest.mRunTable = gArDC_L_Table;
    pMaster->mChromaRequest.mOutput = pOutput;
    pMaster->mChromaRequest.mRemaining = &pMaster->mRemaining;
    pMaster->mChromaRequest.mBitBuffer = &pMaster->mBitBuffer;
    pMaster->mChromaRequest.mFreeBits = &pMaster->mFreeBits;
    pMaster->mChromaRequest.mLimitSize = limitSize;

    u32 quantQuality = quality;
    if (quantQuality == 0) {
        quantQuality = 1;
    }

    u32 scale = quantQuality < 50 ? 5000 / quantQuality : 200 - quantQuality * 2;
    cdj_c_setQuantizationTable(pMaster, scale);
    pMaster->mBlockX = 0;
    pMaster->mBlockY = 0;
    pMaster->mBitBuffer = 0;
    pMaster->mFreeBits = 32;
    pMaster->mLimitSize = limitSize;
    pMaster->mRemaining = limitSize - 16;
    pMaster->mPreviousDC[0] = 0;
    pMaster->mPreviousDC[1] = 0;
    pMaster->mPreviousDC[2] = 0;
    return 0;
}

u32 CArGBAOdh::cdj_c_compressLoop(SArCDJ_OdhMaster* pMaster) {
    do {
        u32 offset = pMaster->mBlockX * 8 + pMaster->mBlocksX * (pMaster->mBlockY * 64);
        fdct_fast(pMaster->mDct, pMaster->mWork + offset, pMaster->mBlocksX * 8, pMaster->mQuantization[0]);
        for (u32 i = 0; i < 64; i++) {
            pMaster->mCoefficients[sZigZag[i]] = pMaster->mDct[i] << 16;
        }

        pMaster->mDct[0] = 0x40004000;
        if (huffmanCoder(reinterpret_cast< u16* >(pMaster->mCoefficients) - 1, &pMaster->mLumaRequest) == 0x80000004) {
            return 0x80000004;
        }

        offset += pMaster->mBlocksY * (pMaster->mBlocksX * 64);
        fdct_fast(pMaster->mDct, pMaster->mWork + offset, pMaster->mBlocksX * 8, pMaster->mQuantization[1]);
        for (u32 i = 0; i < 64; i++) {
            pMaster->mCoefficients[sZigZag[i]] = pMaster->mDct[i] & 0xFFFF;
        }

        offset += pMaster->mBlocksY * (pMaster->mBlocksX * 64);
        fdct_fast(pMaster->mDct, pMaster->mWork + offset, pMaster->mBlocksX * 8, pMaster->mQuantization[1]);
        for (u32 i = 0; i < 64; i++) {
            pMaster->mCoefficients[sZigZag[i]] |= pMaster->mDct[i] << 16;
        }

        pMaster->mDct[0] = 0x40004000;
        if (huffmanCoder(reinterpret_cast< u16* >(pMaster->mCoefficients), &pMaster->mChromaRequest) == 0x80000004) {
            return 0x80000004;
        }

        pMaster->mBlockX++;
        if (pMaster->mBlockX == pMaster->mBlocksX) {
            pMaster->mBlockX = 0;
            pMaster->mBlockY++;
        }
    } while (pMaster->mBlockY < pMaster->mBlocksY);

    if (cdj_c_flashBuffer(pMaster) == 0x80000004) {
        return 0x80000004;
    }

    cdj_c_makeHeader(pMaster, pMaster->mLimitSize - pMaster->mRemaining);
    return pMaster->mLimitSize - pMaster->mRemaining;
}

u32 CArGBAOdh::cdj_c_flashBuffer(SArCDJ_OdhMaster* pMaster) {
    pMaster->mBitBuffer |= 0x7F << (pMaster->mFreeBits -= 7);
    while (pMaster->mFreeBits <= 24) {
        if (pMaster->mRemaining == 0) {
            return 0x80000004;
        }

        pMaster->mOutput[pMaster->mLimitSize - pMaster->mRemaining] = pMaster->mBitBuffer >> 24;
        pMaster->mRemaining--;
        pMaster->mFreeBits += 8;
        pMaster->mBitBuffer <<= 8;
    }

    while ((pMaster->mLimitSize - pMaster->mRemaining) & 3) {
        pMaster->mOutput[pMaster->mLimitSize - pMaster->mRemaining] = 0xFF;
        pMaster->mRemaining--;
    }

    return 0;
}

void CArGBAOdh::cdj_c_setQuantizationTable(SArCDJ_OdhMaster* pMaster, u32 scale) {
    u8 table[2][64];
    u32 j, divisor, i;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 64; j++) {
            u32 value = (sQuantization[i * 64 + j] * scale + 50) / 100;
            if (value == 0) {
                value = 1;
            }

            if (value > 255) {
                value = 255;
            }

            table[i][j] = value;
        }
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 64; j++) {
            divisor = sDctScale[j];
            divisor *= table[i][j];
            divisor = 0x4000000 / divisor;
            pMaster->mQuantization[i][j] = divisor;
        }
    }
}

void CArGBAOdh::cdj_c_makeHeader(SArCDJ_OdhMaster* pMaster, u32 size) {
    pMaster->mOutput[0] = 'A';
    pMaster->mOutput[1] = 'J';
    pMaster->mOutput[2] = 'P';
    pMaster->mOutput[3] = 'G';
    reinterpret_cast< u32* >(pMaster->mOutput)[1] = (pMaster->mQuality << 24) | (pMaster->mSize[0] | (pMaster->mSize[1] << 11));
    reinterpret_cast< u32* >(pMaster->mOutput)[2] = size;
    reinterpret_cast< u32* >(pMaster->mOutput)[3] = 0;
}

u32 CArGBAOdh::cdj_c_colorConv(SArCDJ_OdhMaster* pMaster, u8* pSrc, int format) {
    u8* pY;
    u32 stride;
    u8* pV;
    u8* pU;
    int height;
    if ((pMaster->mSize[0] & 1) || (pMaster->mSize[1] & 1)) {
        return 1;
    }

    u16 size[2];
    u16* pSize = pMaster->mSize;
    for (int i = 0; i < 2; i++) {
        u16 remainder = pSize[i] & 7;
        if (remainder != 0) {
            size[i] = pSize[i] + 8 - remainder;
        } else {
            size[i] = pSize[i];
        }
    }

    pY = pMaster->mWork;
    pU = pY + size[0] * size[1];
    pV = pU + size[0] * size[1];

    if (format == 0) {
        stride = (pMaster->mSize[0] & ~3) * 8;
    } else if (format == 1) {
        stride = (pMaster->mSize[0] & ~3) * 16;
    } else {
        stride = (pMaster->mSize[0] & ~7) * 4;
    }

    height = pMaster->mSize[1];
    for (int y = 0; y < height; y++) {
        LineConv11(pSrc + (y & 3) * 8 + (y / 4) * stride, pY, pU, pV, pMaster->mSize[0], pMaster->mSize[1], gArConvPlttTbl, format);
        pY += size[0];
        pU += size[0];
        pV += size[0];
    }

    return 0;
}

void CArGBAOdh::LineConv11(u8* pSrc, u8* pY, u8* pU, u8* pV, u16 width, u16 height, const s32* pTable, int format) {
    u8* pSrcU = pSrc + width * height;
    u8* pSrcV = pSrc + width * height * 2;
    for (int x = 0; x < width; x++) {
        s32 red;
        s32 green;
        s32 blue;
        if (format == 0) {
            int offset = (x & ~3) * 8 + (x & 3) * 2;
            u16 pixel = pSrc[offset];
            pixel <<= 8;
            pixel |= pSrc[offset + 1];
            red = (pixel >> 11) & 31;
            green = (pixel >> 6) & 31;
            blue = pixel & 31;
        } else if (format == 1) {
            int offset = (x & ~3) * 16 + (x & 3) * 2;
            red = pSrc[offset + 1] >> 3;
            green = pSrc[offset + 32] >> 3;
            blue = pSrc[offset + 33] >> 3;
        } else {
            int offset = (x & 7) + (x & ~7) * 4;
            f32 y = pSrc[offset] - 16.0f;
            f32 u = pSrcU[offset] - 128.0f;
            f32 v = pSrcV[offset] - 128.0f;
            red = 1.16438356f * y + 1.59602715f * v;
            green = 1.16438356f * y - 0.39176160f * u - 0.81296807f * v;
            blue = 1.16438356f * y + 2.01723105 * u;
            if (red < 0) {
                red = 0;
            }

            if (red > 255) {
                red = 255;
            }

            if (green < 0) {
                green = 0;
            }

            if (green > 255) {
                green = 255;
            }

            if (blue < 0) {
                blue = 0;
            }

            if (blue > 255) {
                blue = 255;
            }

            red >>= 3;
            green >>= 3;
            blue >>= 3;
        }

        s32 u = (pTable[red + 96] + pTable[green + 128] + pTable[blue + 160]) >> 16;
        s32 v = (pTable[red + 192] + pTable[green + 224] + pTable[blue + 256]) >> 16;
        *pY++ = (pTable[red] + pTable[green + 32] + pTable[blue + 64]) >> 16;
        *pU++ = u;
        *pV++ = v;
    }
}

void CArGBAOdh::fdct_fast(u32* pOutput, u8* pInput, u32 stride, u32* pQuantization) {
    s32 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp10, tmp11, tmp12, tmp13, z1, z2, z3, z4, z5, z11, z13;
    s32* pData = reinterpret_cast< s32* >(pOutput);
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            pData[y * 8 + x] = pInput[y * stride + x] - 128;
        }
    }

    {
        s32* pBlock = pData;
        for (int i = 0; i < 8; i++) {
            tmp0 = pBlock[0] + pBlock[7];
            tmp7 = pBlock[0] - pBlock[7];
            tmp1 = pBlock[1] + pBlock[6];
            tmp6 = pBlock[1] - pBlock[6];
            tmp2 = pBlock[2] + pBlock[5];
            tmp5 = pBlock[2] - pBlock[5];
            tmp3 = pBlock[3] + pBlock[4];
            tmp4 = pBlock[3] - pBlock[4];

            tmp10 = tmp0 + tmp3;
            tmp13 = tmp0 - tmp3;
            tmp11 = tmp1 + tmp2;
            tmp12 = tmp1 - tmp2;
            pBlock[0] = tmp10 + tmp11;
            pBlock[4] = tmp10 - tmp11;
            z1 = ((tmp12 + tmp13) * 181) >> 8;
            pBlock[2] = tmp13 + z1;
            pBlock[6] = tmp13 - z1;

            tmp10 = tmp4 + tmp5;
            tmp11 = tmp5 + tmp6;
            tmp12 = tmp6 + tmp7;
            z5 = ((tmp10 - tmp12) * 98) >> 8;
            z2 = z5 + ((tmp10 * 139) >> 8);
            z4 = z5 + ((tmp12 * 334) >> 8);
            z3 = (tmp11 * 181) >> 8;
            z11 = tmp7 + z3;
            z13 = tmp7 - z3;
            pBlock[5] = z13 + z2;
            pBlock[3] = z13 - z2;
            pBlock[1] = z11 + z4;
            pBlock[7] = z11 - z4;
            pBlock += 8;
        }
    }

    {
        s32* pBlock = pData;
        for (int i = 0; i < 8; i++) {
            tmp0 = pBlock[0] + pBlock[56];
            tmp7 = pBlock[0] - pBlock[56];
            tmp1 = pBlock[8] + pBlock[48];
            tmp6 = pBlock[8] - pBlock[48];
            tmp2 = pBlock[16] + pBlock[40];
            tmp5 = pBlock[16] - pBlock[40];
            tmp3 = pBlock[24] + pBlock[32];
            tmp4 = pBlock[24] - pBlock[32];

            tmp10 = tmp0 + tmp3;
            tmp13 = tmp0 - tmp3;
            tmp11 = tmp1 + tmp2;
            tmp12 = tmp1 - tmp2;
            pBlock[0] = tmp10 + tmp11;
            pBlock[32] = tmp10 - tmp11;
            z1 = ((tmp12 + tmp13) * 181) >> 8;
            pBlock[16] = tmp13 + z1;
            pBlock[48] = tmp13 - z1;

            tmp10 = tmp4 + tmp5;
            tmp11 = tmp5 + tmp6;
            tmp12 = tmp6 + tmp7;
            z5 = ((tmp10 - tmp12) * 98) >> 8;
            z2 = z5 + ((tmp10 * 139) >> 8);
            z4 = z5 + ((tmp12 * 334) >> 8);
            z3 = (tmp11 * 181) >> 8;
            z11 = tmp7 + z3;
            z13 = tmp7 - z3;
            pBlock[40] = z13 + z2;
            pBlock[24] = z13 - z2;
            pBlock[8] = z11 + z4;
            pBlock[56] = z11 - z4;
            pBlock += 1;
        }
    }

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            pData[y * 8 + x] = static_cast< s32 >(pData[y * 8 + x] * pQuantization[y * 8 + x] + 0x4000) >> 15;
        }
    }
}

u32 CArGBAOdh::huffmanCoder(u16* pCoefficients, SArCDJ_HuffmanRequest* pRequest) {
    s32 value;
    u16* pStart;

    pStart = pCoefficients;
    pCoefficients++;
    mOutputCursor = pRequest->mOutput + (pRequest->mLimitSize - *pRequest->mRemaining);
    int component = 0;
    do {
        s32 dc = static_cast< s32 >(*pCoefficients << 16) >> 16;
        pCoefficients += 2;
        s32 difference = dc - *pRequest->mPreviousDC[component];
        *pRequest->mPreviousDC[component] = dc;
        s32 magnitude;
        if (difference < 0) {
            magnitude = -difference;
            difference--;
        } else {
            magnitude = difference;
        }

        s32 count;
        if (magnitude == 0) {
            count = 0;
        } else {
            count = 1;
            while ((magnitude >>= 1) != 0) {
                count++;
            }
        }

        s32 code = pRequest->mValueTable[count];
        s32 differenceBits = difference & ((1 << count) - 1);
        s32 differenceCount = code >> 24;
        differenceBits |= (code & 0xFFFFFF) << count;
        differenceCount += count;
        if (EmitBit(differenceBits, differenceCount, pRequest) == 0x80000004) {
            return 0x80000004;
        }

        s32 run;
        for (;;) {
            run = 0;

            for (;;) {
                value = *pCoefficients << 16;
                value >>= 16;
                pCoefficients += 2;
                if (value != 0) {
                    break;
                }

                run++;
            }

            if (value == 0x4000) {
                break;
            }

            magnitude = run;
            if (magnitude == 0) {
                count = 0;
            } else {
                count = 1;
                while ((magnitude >>= 1) != 0) {
                    count++;
                }
            }

            code = pRequest->mRunTable[count];
            s32 runBits = run & ((1 << count) - 1);
            s32 runCount = code >> 24;
            runBits |= (code & 0xFFFFFF) << count;
            runCount += count;
            if (EmitBit(runBits, runCount, pRequest) == 0x80000004) {
                return 0x80000004;
            }

            if (value < 0) {
                magnitude = -value;
                value--;
            } else {
                magnitude = value;
            }

            if (magnitude == 0) {
                count = 0;
            } else {
                count = 1;
                while ((magnitude >>= 1) != 0) {
                    count++;
                }
            }

            code = pRequest->mValueTable[count];
            s32 valueBits = value & ((1 << count) - 1);
            s32 valueCount = code >> 24;
            valueBits |= (code & 0xFFFFFF) << count;
            valueCount += count;
            if (EmitBit(valueBits, valueCount, pRequest) == 0x80000004) {
                return 0x80000004;
            }
        }

        if (run != 0) {
            code = pRequest->mRunTable[7];
            if (EmitBit(code & 0xFFFFFF, code >> 24, pRequest) == 0x80000004) {
                return 0x80000004;
            }
        }

        if ((reinterpret_cast< u32 >(pCoefficients) & 2) == 0) {
            break;
        }

        pCoefficients = pStart;
        component = 1;
    } while (true);

    return 0;
}

u32 CArGBAOdh::EmitBit(s32 bits, s32 count, SArCDJ_HuffmanRequest* pRequest) {
    *pRequest->mBitBuffer |= bits << (*pRequest->mFreeBits - count);
    *pRequest->mFreeBits -= count;
    while (*pRequest->mFreeBits <= 24) {
        if (*pRequest->mRemaining == 0) {
            return 0x80000004;
        }

        *mOutputCursor = *pRequest->mBitBuffer >> 24;
        mOutputCursor++;
        (*pRequest->mRemaining)--;
        *pRequest->mBitBuffer <<= 8;
        *pRequest->mFreeBits += 8;
    }

    return 0;
}
