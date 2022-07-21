#pragma once

#include <revolution.h>
#include "JSystem/J3DGraphAnimator/J3DModelData.h"
#include "JSystem/J3DGraphBase/J3DStruct.h"

class MaterialCtrl {
public:
    MaterialCtrl(J3DModelData *, const char *);

    virtual void update();
    virtual void updateMaterial(J3DMaterial *);

    J3DModelData* mModelData;   // _4
    J3DMaterial* mMaterial;     // _8
};

class FogCtrl : public MaterialCtrl {
public:
    FogCtrl(J3DModelData *, bool);

    virtual void update();

    J3DFogInfo mFogInfo;    // _C
};

class MatColorCtrl : public MaterialCtrl {
public:
    MatColorCtrl(J3DModelData *, const char *, u32, const J3DGXColor *);

    virtual void updateMaterial(J3DMaterial *);
    
    u32 mColorChoice;       // _C
    J3DGXColor* mColor;     // _10
};

class ProjmapEffectMtxSetter {
public:
    void updateMtxUseBaseMtx();
};