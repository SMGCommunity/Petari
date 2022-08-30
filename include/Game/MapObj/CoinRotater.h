#pragma once

#include "JSystem/JGeometry.h"
#include "Game/NameObj/NameObj.h"

class CoinRotater : public NameObj {
public:
    CoinRotater(const char *);

    virtual ~CoinRotater();
    virtual void movement();

    f32 _C;
    f32 _10;
    f32 _14;
    TMtx34f mRotateYMtx;        // _18
    TMtx34f mHiSpeedRotateYMtx; // _48
    TMtx34f mWaterRotateMtx;    // _78
};

namespace MR {
    void createCoinRotater();
    TMtx34f& getCoinRotateYMatrix();
    TMtx34f& getCoinHiSpeedRotateYMatrix();
    TMtx34f& getCoinInWaterRotateYMatrix();

};