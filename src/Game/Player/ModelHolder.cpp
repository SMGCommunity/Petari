#include "Game/Player/ModelHolder.hpp"

ModelHolder::ModelHolder(const char *pName, bool a2) : LiveActor(pName) {
    initModelManagerWithAnm(pName, nullptr, a2);
    MR::invalidateClipping(this);
    kill();
}

ModelHolder::~ModelHolder() {}

void ModelHolder::updateMtx(MtxPtr mtx) {
    J3DModel* pModel = MR::getJ3DModel(this);
    PSMTXCopy(mtx, pModel->mBaseTransformMtx);
    pModel->calc();
    pModel->viewCalc();
}
