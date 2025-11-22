#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util.hpp"

ModelObj::ModelObj(const char* pName, const char* pModelName, MtxPtr mtxPtr, int a4, int a5, int a6, bool a7)
    : LiveActor(pName),
      mMtx(mtxPtr) {
    if (a4 < -1) {
        a4 = 0x8;
    }

    if (a5 < -1) {
        a5 = 0x23;
    }

    if (a6 < -1) {
        a6 = 0xB;
    }

    initModelManagerWithAnm(pModelName, nullptr, a7);
    MR::connectToScene(this, a5, a6, a4, -1);
    initSound(8, false);
    initEffectKeeper(8, nullptr, false);

    if (mMtx) {
        mPosition.set<f32>(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
    }
}

void ModelObj::init(const JMapInfoIter&) {
    makeActorAppeared();
}

void ModelObj::calcAndSetBaseMtx() {
    if (mMtx) {
        mPosition.set<f32>(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
        MR::setBaseTRMtx(this, mMtx);
    } else {
        LiveActor::calcAndSetBaseMtx();
    }
}

void ModelObjNpc::init(const JMapInfoIter& rIter) {
    mLodCtrl = MR::createLodCtrlNPC(this, rIter);
    mJointCtrl = new ActorJointCtrl(this);
    makeActorAppeared();
}

void ModelObjNpc::control() {
    mLodCtrl->update();
    mJointCtrl->update();
}

void ModelObjNpc::calcAndSetBaseMtx() {
    if (mMtx) {
        mPosition.set<f32>(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
        MR::setBaseTRMtx(this, mMtx);
    } else {
        LiveActor::calcAndSetBaseMtx();
    }

    mJointCtrl->setCallBackFunction();
}

ModelObjNpc::~ModelObjNpc() {
}

ModelObjNpc::ModelObjNpc(const char* pName, const char* a2, MtxPtr mtx)
    : LiveActor(pName),
      mMtx(mtx),
      mLodCtrl(nullptr),
      mJointCtrl(nullptr) {
    if (mMtx != nullptr) {
        mPosition.set<f32>(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
    }

    initModelManagerWithAnm(a2, nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initShadowFromCSV(this, "Shadow");
    initSound(8, false);
    initEffectKeeper(8, nullptr, false);
}
