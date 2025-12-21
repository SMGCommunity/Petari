#include "Game/Player/ModelHolder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "revolution/mtx.h"

ModelHolder::ModelHolder(const char* pName, bool b) : LiveActor(pName) {
    initModelManagerWithAnm(pName, nullptr, b);
    MR::invalidateClipping(this);
    kill();
}

void ModelHolder::updateMtx(MtxPtr mtxPtr) {
    J3DModel* model = MR::getJ3DModel(this);
    PSMTXCopy(mtxPtr, model->mBaseTransformMtx);
    model->calc();      // 0x10
    model->viewCalc();  // 0x1C
}
