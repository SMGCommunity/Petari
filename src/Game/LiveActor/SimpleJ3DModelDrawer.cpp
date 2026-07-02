#include "Game/LiveActor/SimpleJ3DModelDrawer.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

SimpleJ3DModelDrawer::SimpleJ3DModelDrawer(NameObj* pObj, const char* pName, const char* pModelName, int drawType) : LiveActor(pName) {
    mMaterial = nullptr;
    mShape = nullptr;
    mShapeDraw = nullptr;

    if (drawType >= 0) {
        MR::registerPreDrawFunction(MR::Functor_InlineC(this, &SimpleJ3DModelDrawer::initDraw), drawType);
    }

    initModelManagerWithAnm(pModelName, nullptr, false);
    mMaterial = MR::getMaterial(this, 0);
    mShape = mMaterial->getShape();
    mShapeDraw = mShape->getShapeDraw(0);
    makeActorDead();
}

void SimpleJ3DModelDrawer::initDraw() const {
    J3DModelData* data = MR::getJ3DModelData(this);
    j3dSys.mVtxPos = data->getVtxPosArray();
    j3dSys.mVtxNrm = data->getVtxNrmArray();
    j3dSys.mVtxCol = data->getVtxColorArray(0);
    J3DShape::sOldVcdVatCmd = 0;
    mMaterial->loadSharedDL();
    mShape->loadPreDrawSetting();
    mShape->loadVtxArray();
}

SimpleJ3DModelDrawer::~SimpleJ3DModelDrawer() {
}
