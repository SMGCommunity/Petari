#pragma once

#include <revolution.h>
#include "RVLFaceLibrary/RFL_Types.h"
#include "RVLFaceLibrary/RFL_MakeTex.h"
#include "RVLFaceLibrary/RFL_Model.h"
#include "RVLFaceLibrary/RFL_DataUtility.h"

class MiiFaceRecipe { 
public:
    MiiFaceRecipe(RFLDataSource, int, RFLResolution, u32);

    void setDatabase(RFLMiddleDB *);
    bool hasExpression(RFLExpression) const;
    u32 getModelBufferSize() const;
    void initFaceModel(RFLCharModel *, void *) const;
    RFLErrcode makeIcon(void *, int, int, _GXColor) const;
    RFLErrcode makeIconWithFavoriteColor(void *, int, int) const;

    RFLDataSource mDataSource;  // _0
    RFLMiddleDB* mMiddleDB;     // _4
    u16 mIndex;                 // _8
    u16 _A;
    RFLResolution mResolution;  // _C
    u32 mExpressionFlag;        // _10
};