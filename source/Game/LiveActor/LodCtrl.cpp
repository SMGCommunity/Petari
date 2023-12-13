#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/ClippingDirector.hpp"

#include <cstdio>

namespace {
    void LodFuntionCall(LodCtrl *pCtrl, void (*pFunc)(LiveActor *)) {
        pFunc(pCtrl->mActor);

        if (pCtrl->_10) {
            pFunc(pCtrl->_10);
        }

        if (pCtrl->_14) {
            pFunc(pCtrl->_14);
        }
    }

    template<typename T>
    void LodFuntionCall(LodCtrl *pCtrl, void (*pFunc)(LiveActor *, T), T arg) {
        pFunc(pCtrl->mActor, arg);

        if (pCtrl->_10) {
            pFunc(pCtrl->_10, arg);
        }

        if (pCtrl->_14) {
            pFunc(pCtrl->_14, arg);
        }
    }
};

const bool def = false;

LodCtrl::LodCtrl(LiveActor *pActor, const JMapInfoIter &rIter) {
    _0 = 2000.0f;
    _4 = 3000.0f;
    _8 = pActor;
    mActor = pActor;
    _10 = nullptr;
    _14 = nullptr;
    _18 = 0;
    _19 = 0;
    _1A = 1;
    _1B = 0;
    mViewGroupID = -1;
    mActorLightCtrl = nullptr;
    _1C = &def;
    _20 = &def;
    _24 = &def;
    _28 = &def;
    MR::getClippingDirector()->entryLodCtrl(this, rIter);
    mActorLightCtrl = mActor->mActorLightCtrl;
}

void LodCtrl::offSyncShadowHost() {
    MR::offShadowVisibleSyncHostAll(mActor);
    _1A = 0;
}

void LodCtrl::appear() {
    MR::showModel(mActor);
    mActorLightCtrl = mActor->mActorLightCtrl;

    if (_10) {
        if (MR::isDead(_10)) {
            _10->kill();
        }
    }

    if (_14) {
        if (MR::isDead(_14)) {
            _14->kill();
        }
    }

    if (_1A) {
        MR::offShadowVisibleSyncHostAll(mActor);
    }
}

void LodCtrl::kill() {
    MR::showModel(mActor);

    if (_10) {
        if (MR::isDead(_10)) {
            _10->kill();
        }
    }

    if (_14) {
        if (MR::isDead(_14)) {
            _14->kill();
        }
    }

    if (_1A) {
        MR::onShadowVisibleSyncHostAll(mActor);
    }

    mActorLightCtrl = 0;
}

void LodCtrl::validate() {
    appear();
    _18 = 1;
}

void LodCtrl::invalidate() {
    kill();
    _18 = 0;
}

bool LodCtrl::isShowLowModel() const {
    ModelObj* obj = _14;
    bool ret = false;

    if (obj && obj == _8) {
        ret = true;
    }

    return ret;
}

void LodCtrl::setDistanceToMiddle(f32 dist) {
    _0 = dist;
}

f32 LodCtrl::calcDistanceToCamera() const {
    if (_1B) {
        return MR::calcCameraDistanceZ(mActor->mPosition);
    }

    return MR::calcDistanceToPlayer(mActor);
}

void LodCtrl::setDistanceToLow(f32 dist) {
    _4 = dist;
}

void LodCtrl::setDistanceToMiddleAndLow(f32 mid_dist, f32 low_dist) {
    _0 = mid_dist;
    _4 = low_dist;
}

void LodCtrl::setClippingTypeSphereContainsModelBoundingBox(f32 bounds) {
    LodFuntionCall<f32>(this, MR::setClippingTypeSphereContainsModelBoundingBox, bounds);
}

void LodCtrl::setFarClipping(f32 clip) {
    LodFuntionCall<f32>(this, MR::setClippingFar, clip);
}

void LodCtrl::invalidateClipping() {
    LodFuntionCall(this, MR::invalidateClipping);
}

void LodCtrl::showHighModel() {
    if (MR::isHiddenModel(mActor)) {
        if (mActorLightCtrl) {
            mActor->mActorLightCtrl->copy(mActorLightCtrl);
        }

        mActorLightCtrl = mActor->mActorLightCtrl;
        MR::showModel(mActor);
    }
    else {
        if (_10) {
            if (!MR::isDead(_10)) {
                _10->makeActorDead();
            }
        }

        if (_14) {
            if (!MR::isDead(_14)) {
                _14->makeActorDead();
            }
        }   
    }

    _8 = mActor;
}

void LodCtrl::showMiddleModel() {
    if (MR::isDead(_10)) {
        if (mActorLightCtrl) {
            _10->mActorLightCtrl->copy(mActorLightCtrl);
        }

        mActorLightCtrl = _10->mActorLightCtrl;
        _10->makeActorAppeared();
        MR::calcAnimDirect(mActor);
    }
    else {
        if (!MR::isHiddenModel(mActor)) {
            MR::hideModelAndOnCalcAnim(mActor);
        }

        if (_14) {
            if (!MR::isDead(_14)) {
                _14->makeActorDead();
            }
        }
    }

    _8 = _10;
}

void LodCtrl::showLowModel() {
    if (MR::isDead(_14)) {
        if (mActorLightCtrl) {
            _14->mActorLightCtrl->copy(mActorLightCtrl);
        }

        mActorLightCtrl = _14->mActorLightCtrl;
        _14->makeActorAppeared();
        MR::calcAnimDirect(mActor);
    }
    else {
        if (!MR::isHiddenModel(mActor)) {
            MR::hideModelAndOnCalcAnim(mActor);
        }

        if (_10 && !MR::isDead(_10)) {
            _10->makeActorDead();
        }
    }

    _8 = _14;
}

void LodCtrl::hideAllModel() {
    if (!MR::isHiddenModel(mActor)) { 
        MR::hideModelAndOnCalcAnim(mActor);
    }

    if (_10) {
        if (!MR::isDead(_10)) { 
            _10->makeActorDead();
        }
    }

    if (_14) {
        if (!MR::isDead(_14)) {
            _14->makeActorDead();
        }
    }

    _8 = 0; 
}

void LodCtrl::setViewCtrlPtr(const bool *a1, const bool *a2, const bool *a3, const bool *a4) {
    _1C = a1;
    _20 = a2;
    _24 = a3;
    _28 = a4;
}

void LodCtrl::createLodModel(int a1, int a2, int a3) {
    _10 = initLodModel(a1, a2, a3, false);
    _14 = initLodModel(a1, a2, a3, true);

    if (_10 || _14) {
        appear();
        _18 = 1;
    }
    else {
        kill();
        _18 = 0;
    }
}

void LodCtrl::syncMaterialAnimation() {
    if (_10) {
        MR::syncMaterialAnimation(_10, mActor);
    }

    if (_14) {
        MR::syncMaterialAnimation(_14, mActor);
    }
}

void LodCtrl::syncJointAnimation() {
    if (_10) {
        MR::syncJointAnimation(_10, mActor);
    }

    if (_14) {
        MR::syncJointAnimation(_14, mActor);
    }
}

void LodCtrl::initLightCtrl() {
    if (_10) {
        MR::initLightCtrl(_10);
    }

    if (_14) {
        MR::initLightCtrl(_14);
    }
}

ModelObj* LodCtrl::initLodModel(int a1, int a2, int a3, bool isLowModel) const {
    const char* res = MR::getModelResName(mActor);
    const char* type = isLowModel ? "Low" : "Middle";
    char buf[0x100];
    snprintf(buf, 0x100, "/ObjectData/%s%s.arc", res, type);

    if (!MR::isFileExist(buf, false)) {
        return nullptr;
    }

    const char* objName = isLowModel ? MR::createLowModelObjName(mActor) : MR::createMiddleModelObjName(mActor);
    snprintf(buf, 0x100, "%s%s", res, type);
    ModelObj* obj = new ModelObj(objName, buf, mActor->getBaseMtx(), a1, a2, a3, false);
    obj->initWithoutIter();
    obj->makeActorDead();
    MR::setClippingTypeSphereContainsModelBoundingBox(obj, 100.0f);
    MR::copyTransRotateScale(mActor, obj);
    return obj;
}

bool LodCtrlFunction::isExistLodLowModel(const char *pName) {
    char buf[0x100];
    snprintf(buf, 0x100, "/ObjectData/%sLow.arc", pName);
    return MR::isFileExist(buf, false);
}