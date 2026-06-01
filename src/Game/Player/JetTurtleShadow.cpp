#include "Game/Player/JetTurtleShadow.hpp"

JetTurtleShadow::JetTurtleShadow(const char* pName) : LiveActor(pName) {
}

JetTurtleShadow::~JetTurtleShadow() {
}

void JetTurtleShadow::calcType0(MtxPtr pMtx) {
    J3DModel* model = MR::getJ3DModel(this);
    MR::setBaseTRMtx(this, pMtx);
    OSLockMutex(&MR::MutexHolder< 0 >::sMutex);
    model->calc();
    model->viewCalc();
    OSUnlockMutex(&MR::MutexHolder< 0 >::sMutex);
}

void JetTurtleShadow::drawType0() const {
    J3DModel* model = MR::getJ3DModel(this);
    J3DModelX* modelX = (J3DModelX*)model;
    modelX->mFlags.clear();
    *(int*)&modelX->mFlags |= 0x800;
    GXSetZScaleOffset(1.0f, 0.0f);
    modelX->directDraw(nullptr);
}

void JetTurtleShadow::drawType1() const {
    J3DModel* model = MR::getJ3DModel(this);
    J3DModelX* modelX = (J3DModelX*)model;
    modelX->mFlags.clear();
    *(int*)&modelX->mFlags |= 0x40;
    modelX->directDraw(nullptr);
}

void JetTurtleShadow::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("Koura", nullptr, false);
    makeActorDead();
}
