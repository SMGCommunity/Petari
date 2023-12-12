#include "JSystem/J3DGraphBase/J3DMaterial.hpp"

J3DAlphaComp::J3DAlphaComp() {
    u16 val = sUnk;
    _2 = 0;
    _0 = val;
    _3 = 0;
}

#ifdef NON_MATCHING
// some scheduling issues
void J3DCurrentMtx::setCurrentTexMtx(u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, u8 a8) {
    _0 = (a4 << 24) | (a3 << 18) & 0x3FC0000 | (a1 << 6) & 0x3FC0 | (a2 << 12) & 0xFF000;
    _4 = (a8 << 18) | (a7 << 12) & 0xFF000 | a5 & 0xFFF00FFF | (a6 << 6) & 0x3FC0;
}
#endif

