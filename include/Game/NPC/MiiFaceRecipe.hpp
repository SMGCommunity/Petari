#pragma once

#include <revolution.h>
#include <RVLFaceLib.h>

class MiiFaceRecipe { 
public:
    MiiFaceRecipe(RFLDataSource, int, RFLResolution, u32);

    void setDatabase(RFLMiddleDB *);
    bool hasExpression(RFLExpression) const;
    u32 getModelBufferSize() const;
    void initFaceModel(RFLCharModel *, void *) const;
    RFLErrcode makeIcon(void *, int, int, _GXColor) const;
    RFLErrcode makeIconWithFavoriteColor(void *, int, int) const;

    RFLDataSource mDataSource;  // 0x0
    RFLMiddleDB* mMiddleDB;     // 0x4
    u16 mIndex;                 // 0x8
    u16 _A;
    RFLResolution mResolution;  // 0xC
    u32 mExpressionFlag;        // 0x10
};