#pragma once

#include <revolution.h>
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphBase/J3DStruct.hpp"

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

    J3DFogInfo mFogInfo;            // _C
    s32 mNumMaterials;              // _38
    J3DMaterial** mMaterials;       // _3C
};

class MatColorCtrl : public MaterialCtrl {
public:
    MatColorCtrl(J3DModelData *, const char *, u32, const J3DGXColor *);

    virtual void updateMaterial(J3DMaterial *);
    
    u32 mColorChoice;       // _C
    const J3DGXColor* mColor;     // _10
};

class TexMtxCtrl : public MaterialCtrl {
public:
    TexMtxCtrl(J3DModelData *, const char *);

    void setTexMtx(u32, J3DTexMtx *);

    J3DTexMtx* mMatricies[8];       // _C
};

class ProjmapEffectMtxSetter {
public:
    void updateMtxUseBaseMtx();
};