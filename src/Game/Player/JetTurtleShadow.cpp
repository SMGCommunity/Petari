#include "Game/Player/JetTurtleShadow.hpp"
#include "Game/Player/J3DModelX.hpp"
#include <revolution/os/OSMutex.h>
#include <revolution/gx/GXTransform.h>

JetTurtleShadow::JetTurtleShadow(const char *pName) : LiveActor(pName) {}

JetTurtleShadow::~JetTurtleShadow() {}

void JetTurtleShadow::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("Koura", nullptr, false);
    makeActorDead();
}

void JetTurtleShadow::calcType0(MtxPtr mtx) {
    J3DModel* pModel = MR::getJ3DModel(this);
    MR::setBaseTRMtx(this, mtx);
    OSLockMutex(&MR::MutexHolder<0>::sMutex);
    pModel->calc();
    pModel->viewCalc();
    OSUnlockMutex(&MR::MutexHolder<0>::sMutex);
}

void JetTurtleShadow::drawType0() const {
    J3DModelX* pModel = (J3DModelX*)MR::getJ3DModel(this);
    J3DModelX::Flags flagsSet;
    flagsSet.clear();
    flagsSet._14 = 1;
    pModel->mFlags = flagsSet;
    GXSetZScaleOffset(1.0f, 0.0f);
    pModel->directDraw(nullptr);
}

void JetTurtleShadow::drawType1() const {
    J3DModelX* pModel = (J3DModelX*)MR::getJ3DModel(this);
    J3DModelX::Flags flagsSet;
    flagsSet.clear();
    flagsSet._19 = 1;
    pModel->mFlags = flagsSet;
    pModel->directDraw(nullptr);
}
