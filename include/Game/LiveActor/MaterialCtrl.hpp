#pragma once

#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphBase/J3DStruct.hpp"
#include <revolution.h>

class J3DTexMtx;

class MaterialCtrl {
public:
    MaterialCtrl(J3DModelData*, const char*);

    virtual void update();
    virtual void updateMaterial(J3DMaterial*);

    J3DModelData* mModelData;  // 0x4
    J3DMaterial* mMaterial;    // 0x8
};

class FogCtrl : public MaterialCtrl {
public:
    FogCtrl(J3DModelData*, bool);

    virtual void update() override;

    J3DFogInfo mFogInfo;       // 0xC
    s32 mNumMaterials;         // 0x38
    J3DMaterial** mMaterials;  // 0x3C
};

class MatColorCtrl : public MaterialCtrl {
public:
    MatColorCtrl(J3DModelData*, const char*, u32, const J3DGXColor*);

    virtual void updateMaterial(J3DMaterial*) override;

    u32 mColorChoice;          // 0xC
    const J3DGXColor* mColor;  // 0x10
};

class ViewProjmapEffectMtxSetter : public MaterialCtrl {
public:
    ViewProjmapEffectMtxSetter(J3DModelData*);

    virtual void update() override;

    J3DTexMtxInfo** mMatricies;  // 0xC
    s32 mNumMatricies;           // 0x10
};

class TexMtxCtrl : public MaterialCtrl {
public:
    TexMtxCtrl(J3DModelData*, const char*);

    void setTexMtx(u32, J3DTexMtx*);

    J3DTexMtx* mMatricies[8];  // 0xC
};

class ProjmapEffectMtxSetter {
public:
    void updateMtxUseBaseMtx();

    void updateMtxUseBaseMtxWithLocalOffset(const TVec3f&);
};
