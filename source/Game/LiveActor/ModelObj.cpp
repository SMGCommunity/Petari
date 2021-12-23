#include "Game/LiveActor/ModelObj.h"
#include "Game/Util.h"

ModelObj::ModelObj(const char *pName, const char *pModelName, MtxPtr mtxPtr, int a4, int a5, int a6, bool a7) : LiveActor(pName) {
    mMtx = mtxPtr;

    if (a4 < -1) {
        a4 = 0x8;
    }

    if (a5 < -1) {
        a5 = 0x23;
    }

    if (a6 < -1) {
        a6 = 0xB;
    }

    initModelManagerWithAnm(pModelName, 0, a7);
    MR::connectToScene(this, a5, a6, a4, -1);
    initSound(8, 0);
    initEffectKeeper(8, 0, 0);

    if (mMtx) {
        mPosition.set(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
    }
}

void ModelObj::init(const JMapInfoIter &) {
    makeActorAppeared();
}

void ModelObj::calcAndSetBaseMtx() {
    if (mMtx) {
        mPosition.set(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
        MR::setBaseTRMtx(this, mMtx);
    }
    else {
        LiveActor::calcAndSetBaseMtx();
    }
}