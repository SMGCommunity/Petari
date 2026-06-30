#include "Game/Screen/odh.hpp"

u32 ODHEncodeRGB565(u8* pParam1, u8* pParam2, int width, int height, u32 limitSize, int quality, u8* pParam7) {
    CArGBAOdh odh;

    return odh.compressGbaOdh(pParam1, pParam2, width, height, quality, limitSize, pParam7, 0);
}

u32 CArGBAOdh::compressGbaOdh(u8* pParam1, u8* pParam2, int width, int height, int quality, u32 limitSize, u8* pParam7, int format) {
    if (limitSize == 0) {
        limitSize = width * height * 2;
    }

    SArCDJ_OdhMaster master;
    u16 size[2] = {width, height};

    if (cdj_c_initializeCompressOdh(&master, size, quality, pParam7, pParam2, limitSize) != 0) {
        return 0;
    }

    if (cdj_c_colorConv(&master, pParam1, format) != 0) {
        return 0;
    }

    u32 result = cdj_c_compressLoop(&master);

    while (result == 0x80000004) {
        quality -= 5;

        if (quality > 0) {
            if (cdj_c_initializeCompressOdh(&master, size, quality, pParam7, pParam2, limitSize) != 0) {
                return 0;
            }

            result = cdj_c_compressLoop(&master);
            continue;
        }

        return 0;
    }

    return result;
}
