#include "Game/LiveActor/DisplayListMaker.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/Util.hpp"
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>

FogCtrl* DisplayListMaker::addFogCtrl(bool a1) {
    FogCtrl* fog = new FogCtrl(mModel->mModelData, a1);
    push(fog);
    mFogCtrl = fog;
    onBothFlagAll(0x10000000);
    return fog;
}

MatColorCtrl* DisplayListMaker::addMatColorCtrl(const char* a1, u32 a2, const J3DGXColor* a3) {
    MatColorCtrl* ctrl = new MatColorCtrl(mModel->mModelData, a1, a2, a3);
    push(ctrl);
    s16 materialNo = MR::getMaterialNo(mModel->mModelData, a1);
    onBothFlag(materialNo, 1);
    return ctrl;
}

TexMtxCtrl* DisplayListMaker::addTexMtxCtrl(const char* a1) {
    TexMtxCtrl* ctrl = new TexMtxCtrl(mModel->mModelData, a1);
    push(ctrl);
    s16 materialNo = MR::getMaterialNo(mModel->mModelData, a1);
    onBothFlag(materialNo, 0x200);
    return ctrl;
}

ProjmapEffectMtxSetter* DisplayListMaker::addProjmapEffectMtxSetter() {
    ProjmapEffectMtxSetter* setter = new ProjmapEffectMtxSetter(mModel, mResHolder);
    push(setter);
    return setter;
}

// DisplayListMaker::addMirrorReflectionMtxSetter

void DisplayListMaker::push(MaterialCtrl* pCtrl) {
    mMaterialCtrl.push_back(pCtrl);
}

// ...
