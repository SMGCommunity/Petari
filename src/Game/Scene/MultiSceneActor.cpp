#include "Game/Scene/MultiSceneActor.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Util/MtxUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>

MultiSceneActor::MultiSceneActor(const char *pName, const char *pModelName, bool a3) : NameObj(pName) {
    mTranslation.x = 0.0f;
    mTranslation.y = 0.0f;
    mTranslation.z = 0.0f;
    mRotation.x = 0.0f;
    mRotation.y = 0.0f;
    mRotation.z = 0.0f;
    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;
    _30 = true;
    mModelMgr = nullptr;
    mEffectSystem = nullptr;
    mEffectKeeper = nullptr;
    mNerve = nullptr;
    mModelMgr = new ModelManager();
    mModelMgr->init(pModelName, nullptr, a3);
}

void MultiSceneActor::init(const JMapInfoIter &) {

}

void MultiSceneActor::movement() {
    if (!_30) {
        mModelMgr->update();

        if (mNerve != nullptr) {
            mNerve->update();
        }

        if (!_30) {
            control();
        }
    }
}

void MultiSceneActor::calcAnim() {
    if (!_30) {
        TMtx34f mtx;
        mtx.identity();
        MR::makeMtxTR(mtx.toMtxPtr(), mTranslation, mRotation);
        PSMTXCopy(mtx.toMtxPtr(), mModelMgr->getJ3DModel()->_24);
        J3DModel* model = mModelMgr->getJ3DModel();
        model->_18 = mScale.x;
        model->_1C = mScale.y;
        model->_20 = mScale.z;
        mModelMgr->calcAnim();
    }
}

void MultiSceneActor::calcViewAndEntry() {
    if (!_30) {
        mModelMgr->calcView();
        mModelMgr->entry();
    }
}

void MultiSceneActor::appear() {
    _30 = false;
    calcAnim();
}

void MultiSceneActor::initEffect(EffectSystem* pSystem, int unk, const char *pName) {
    mEffectSystem = pSystem;
    mEffectKeeper = new MultiSceneEffectKeeper(mName, mModelMgr, unk, pName);
    mEffectKeeper->init(this, pSystem);
}

void MultiSceneActor::setNerve(const Nerve *pNerve) {
    mNerve->setNerve(pNerve);
}

bool MultiSceneActor::isNerve(const Nerve *pNerve) const {
    return mNerve->getCurrentNerve() == pNerve;
}

void MultiSceneActor::initNerve(const Nerve *pNerve) {
    mNerve = new Spine(this, pNerve);
}