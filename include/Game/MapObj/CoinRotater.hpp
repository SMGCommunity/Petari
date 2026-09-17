#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CoinRotater : public NameObj {
public:
    CoinRotater(const char*);

    virtual void movement();

    /* 0x0C */ f32 _C;
    /* 0x10 */ f32 _10;
    /* 0x14 */ f32 _14;
    /* 0x18 */ TMtx34f mRotateYMtx;
    /* 0x48 */ TMtx34f mHiSpeedRotateYMtx;
    /* 0x78 */ TMtx34f mWaterRotateMtx;
};

namespace MR {
    void createCoinRotater();
    TMtx34f& getCoinRotateYMatrix();
    TMtx34f& getCoinHiSpeedRotateYMatrix();
    TMtx34f& getCoinInWaterRotateYMatrix();
};  // namespace MR
