#pragma once

#include "Game/Util/StringUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphBase/J3DStruct.hpp"
#include "JSystem/J3DGraphBase/J3DTexture.hpp"
#include "JSystem/JUtility/JUTNameTab.hpp"
#include <revolution.h>

class J3DTexMtx;
class ResourceHolder;

class MaterialCtrl {
public:
    MaterialCtrl(J3DModelData*, const char*);

    virtual void update();
    virtual void updateMaterial(J3DMaterial*);

    /* 0x04 */ J3DModelData* mModelData;
    /* 0x08 */ J3DMaterial* mMaterial;
};

class FogCtrl : public MaterialCtrl {
public:
    FogCtrl(J3DModelData*, bool);

    virtual void update() override;

    /* 0x0C */ J3DFogInfo mFogInfo;
    /* 0x38 */ s32 mNumMaterials;
    /* 0x3C */ J3DMaterial** mMaterials;
};

class MatColorCtrl : public MaterialCtrl {
public:
    MatColorCtrl(J3DModelData*, const char*, u32, const J3DGXColor*);

    virtual void updateMaterial(J3DMaterial*) override;

    /* 0x0C */ u32 mColorChoice;
    /* 0x10 */ const J3DGXColor* mColor;
};

class ViewProjmapEffectMtxSetter : public MaterialCtrl {
public:
    ViewProjmapEffectMtxSetter(J3DModelData*);

    virtual void update() override;

    /* 0x0C */ J3DTexMtxInfo** mMatricies;
    /* 0x10 */ s32 mNumMatricies;
};

class ProjmapEffectMtxSetter : public MaterialCtrl {
public:
    struct UpdateEffectMtxInfo {
        UpdateEffectMtxInfo() {
        }

        /* 0x00 */ J3DTexMtx* mTexMtx;
        /* 0x04 */ TPos3f mInitEffectMtx;
    };

    ProjmapEffectMtxSetter(J3DModel*, const ResourceHolder*);

    virtual void update() override;
    void getBaseTrans(TVec3f*) const;
    void updateMtxUseBaseMtx();
    void updateMtxUseBaseMtxWithLocalOffset(const TVec3f&);

    /* 0x0C */ UpdateEffectMtxInfo* mUpdatingMtxInfo;
    /* 0x10 */ s32 mNumUpdatingMtx;
    /* 0x14 */ TPos3f mEffectMtx;
    /* 0x44 */ J3DModel* mModel;
};

class MirrorReflectionMtxSetter : public MaterialCtrl {
public:
    MirrorReflectionMtxSetter(J3DModel*, const ResourceHolder*);

    void addUpdatingTexMtxFromName(J3DModelData* pModelData) NO_INLINE {
        u16 textureNum = pModelData->getTexture()->getNum();
        for (u16 i = 0; i < textureNum; i++) {
            if (MR::isEqualString(pModelData->getTextureName()->getName(i), sMirrorTextureName)) {
                addUpdatingTexMtxFromTexNo(pModelData, i);
            }
        }
    }
    void addUpdatingTexMtxFromTexNo(J3DModelData*, u16);
    void addUpdatingTexMtxFromTexCoord(J3DMaterial*);
    void addUpdatingTexMtx(J3DTexMtx*);
    virtual void update() override;

    static char sMirrorTextureName[];

    /* 0x0C */ J3DTexMtx* mUpdatingTexMtx[8];
    /* 0x2C */ s32 mNumUpdatingTexMtx;
};

class MarioShadowProjmapMtxSetter : public MaterialCtrl {
public:
    MarioShadowProjmapMtxSetter(J3DModel*, const ResourceHolder*);

    virtual void update() override;

    /* 0x0C */ ProjmapEffectMtxSetter* mProjmapMtxSetter;
};

class TexMtxCtrl : public MaterialCtrl {
public:
    TexMtxCtrl(J3DModelData*, const char*);

    void setTexMtx(u32, J3DTexMtx*);
    virtual void updateMaterial(J3DMaterial*) override;

    /* 0x0C */ J3DTexMtx* mMatricies[8];
};
