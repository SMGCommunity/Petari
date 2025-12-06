#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry.hpp"

class CoinRotater : public NameObj {
public:
    CoinRotater(const char*);

    virtual ~CoinRotater();
    virtual void movement();

    f32 _C;
    f32 _10;
    f32 _14;
    TMtx34f mRotateYMtx;         // 0x18
    TMtx34f mHiSpeedRotateYMtx;  // 0x48
    TMtx34f mWaterRotateMtx;     // 0x78
};

namespace MR {
    void createCoinRotater();
    TMtx34f& getCoinRotateYMatrix();
    TMtx34f& getCoinHiSpeedRotateYMatrix();
    TMtx34f& getCoinInWaterRotateYMatrix();

};  // namespace MR