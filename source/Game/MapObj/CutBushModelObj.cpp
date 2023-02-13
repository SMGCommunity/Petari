#include "Game/MapObj/CutBushModelObj.h"
#include "Game/Util.h"

CutBushModelObj::CutBushModelObj(const char *pName, const char *pObjName, bool useLight, MtxPtr mtx) : LiveActor(pName) {
    mMtx = mtx;
    initModelManagerWithAnm(pObjName, nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjWeakLightNoMovement(this);

    if (useLight) {
        MR::initLightCtrl(this);
    }

    initSound(8, false);

    if (mMtx) {
        mPosition.set(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
    }
}

void CutBushModelObj::init(const JMapInfoIter &rIter) {
    makeActorAppeared();
}

void CutBushModelObj::initAfterPlacement() {
    if (MR::getLightCtrl(this)) {
        MR::updateLightCtrlDirect(this);
    }
}

void CutBushModelObj::calcAndSetBaseMtx() {
    if (mMtx) {
        mPosition.set(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
        MR::setBaseTRMtx(this, mMtx);
    }
    else {
        LiveActor::calcAndSetBaseMtx();
    }
}

CutBushModelObj::~CutBushModelObj() {
    
}