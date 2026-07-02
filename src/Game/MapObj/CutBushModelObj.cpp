#include "Game/MapObj/CutBushModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

CutBushModelObj::CutBushModelObj(const char* pName, const char* pModelName, bool useLight, MtxPtr pMtx) : LiveActor(pName), mMtx(pMtx) {
    initModelManagerWithAnm(pModelName, nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjWeakLightNoMovement(this);

    if (useLight) {
        MR::initLightCtrl(this);
    }

    initSound(8, false);

    if (mMtx != nullptr) {
        mPosition.set< f32 >(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
    }
}

void CutBushModelObj::init(const JMapInfoIter& rIter) {
    makeActorAppeared();
}

void CutBushModelObj::initAfterPlacement() {
    if (MR::getLightCtrl(this)) {
        MR::updateLightCtrlDirect(this);
    }
}

void CutBushModelObj::calcAndSetBaseMtx() {
    if (mMtx != nullptr) {
        mPosition.set< f32 >(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
        MR::setBaseTRMtx(this, mMtx);
    } else {
        LiveActor::calcAndSetBaseMtx();
    }
}

CutBushModelObj::~CutBushModelObj() {
}
