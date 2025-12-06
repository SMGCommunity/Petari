#pragma once

#include "Game/Util/Array.hpp"
#include <revolution/types.h>

class FogCtrl;
class J3DAnmBase;
class J3DGXColor;
class J3DModel;
class MarioShadowProjmapMtxSetter;
class MatColorCtrl;
class MaterialCtrl;
class MirrorReflectionMtxSetter;
class ProjmapEffectMtxSetter;
class ResourceHolder;
class TexMtxCtrl;
class ViewProjmapEffectMtxSetter;

class DisplayListMaker {
public:
    DisplayListMaker(J3DModel*, const ResourceHolder*);

    void update();
    void diff();
    void newDifferedDisplayList();
    bool isValidDiff();
    void onPrgFlag(u16, u32);
    void onCurFlag(u16, u32);
    u32 getDiffFlag(s32) const;
    FogCtrl* addFogCtrl(bool);
    MatColorCtrl* addMatColorCtrl(const char*, u32, const J3DGXColor*);
    TexMtxCtrl* addTexMtxCtrl(const char*);
    ProjmapEffectMtxSetter* addProjmapEffectMtxSetter();
    MirrorReflectionMtxSetter* addMirrorReflectionMtxSetter();
    void onCurFlagBpk(const J3DAnmBase*);
    void offCurFlagBpk(const J3DAnmBase*);
    void onCurFlagBtp(const J3DAnmBase*);
    void offCurFlagBtp(const J3DAnmBase*);
    void onCurFlagBtk(const J3DAnmBase*);
    void offCurFlagBtk(const J3DAnmBase*);
    void onCurFlagBrk(const J3DAnmBase*);
    void offCurFlagBrk(const J3DAnmBase*);
    void push(MaterialCtrl*);
    void onBothFlag(u16, u32);
    void onBothFlagAll(u32);
    void checkMaterial();
    void checkViewProjmapEffectMtx();
    ViewProjmapEffectMtxSetter* addViewProjmapEffectMtxSetter();
    MarioShadowProjmapMtxSetter* addMarioShadowProjmapMtxSetter();
    void checkTexture();
    bool isExistDiffMaterial(const J3DModelData*);

    /* 0x00 */ MR::Vector< MR::AssignableArray< MaterialCtrl* > > mMaterialCtrl;
    /* 0x0C */ u32 mPrgFlag;
    /* 0x10 */ u32 mCurFlag;
    /* 0x14 */ J3DModel* mModel;
    /* 0x18 */ FogCtrl* mFogCtrl;
    /* 0x1C */ ResourceHolder* mResHolder;
};
