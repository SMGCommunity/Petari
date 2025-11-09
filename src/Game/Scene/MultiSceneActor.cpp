#include "Game/LiveActor/ModelManager.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Scene/MultiSceneActor.hpp"
#include "Game/Scene/MultiSceneEffectKeeper.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>

MultiSceneActor::MultiSceneActor(const char* pName, const char* pModelName, bool param3) :
    NameObj(pName),
    mTranslation(0.0f, 0.0f, 0.0f),
    mRotation(0.0f, 0.0f, 0.0f),
    mScale(1.0f, 1.0f, 1.0f),
    _30(true),
    mModelManager(nullptr),
    mEffectSystem(nullptr),
    mEffectKeeper(nullptr),
    mSpine(nullptr)
{
    mModelManager = new ModelManager();
    mModelManager->init(pModelName, nullptr, param3);
}

void MultiSceneActor::init(const JMapInfoIter& rIter) {
    
}

void MultiSceneActor::movement() {
    if (_30) {
        return;
    }

    mModelManager->update();

    if (mSpine != nullptr) {
        mSpine->update();
    }

    if (_30) {
        return;
    }

    control();
}

void MultiSceneActor::calcAnim() {
    if (_30) {
        return;
    }

    TMtx34f mtx;
    mtx.identity();

    MR::makeMtxTR(mtx, mTranslation, mRotation);
    PSMTXCopy(mtx, mModelManager->getJ3DModel()->mBaseTransformMtx);

    J3DModel* pModel = mModelManager->getJ3DModel();
    pModel->_18 = mScale.x;
    pModel->_1C = mScale.y;
    pModel->_20 = mScale.z;

    mModelManager->calcAnim();
}

void MultiSceneActor::calcViewAndEntry() {
    if (_30) {
        return;
    }

    mModelManager->calcView();
    mModelManager->entry();
}

void MultiSceneActor::appear() {
    _30 = false;

    calcAnim();
}

void MultiSceneActor::kill() {
    if (mEffectKeeper != nullptr) {
        mEffectKeeper->clear();
    }

    _30 = true;
}

ModelManager* MultiSceneActor::getModelManager() const {
    return mModelManager;
}

EffectSystem* MultiSceneActor::getEffectSystem() const {
    return mEffectSystem;
}

MultiSceneEffectKeeper* MultiSceneActor::getEffectKeeper() const {
    return mEffectKeeper;
}

void MultiSceneActor::initEffect(EffectSystem* pEffectSystem, int param2, const char* pName) {
    mEffectSystem = pEffectSystem;

    mEffectKeeper = new MultiSceneEffectKeeper(mName, mModelManager, param2, pName);
    mEffectKeeper->init(this, pEffectSystem);
}

void MultiSceneActor::setNerve(const Nerve* pNerve) {
    mSpine->setNerve(pNerve);
}

bool MultiSceneActor::isNerve(const Nerve* pNerve) const {
    return mSpine->getCurrentNerve() == pNerve;
}

void MultiSceneActor::initNerve(const Nerve* pNerve) {
    mSpine = new Spine(this, pNerve);
}

s32 MultiSceneActor::getNerveStep() const {
    return mSpine->mStep;
}

namespace MultiScene {
    void startBck(MultiSceneActor* pActor, const char* pBckName) {
        pActor->getModelManager()->startBck(pBckName, nullptr);
    }

    void startBtk(MultiSceneActor* pActor, const char* pBtkName) {
        pActor->getModelManager()->startBtk(pBtkName);
    }

    void startBtp(MultiSceneActor* pActor, const char* pBtpName) {
        pActor->getModelManager()->startBtp(pBtpName);
    }

    void startBva(MultiSceneActor* pActor, const char* pBvaName) {
        pActor->getModelManager()->startBva(pBvaName);
    }

    void setBtpFrameAndStop(MultiSceneActor* pActor, f32 frame) {
        J3DFrameCtrl* pBtpCtrl;

        pBtpCtrl = pActor->getModelManager()->getBtpCtrl();
        pBtpCtrl->mCurrentFrame = frame;

        pBtpCtrl = pActor->getModelManager()->getBtpCtrl();
        pBtpCtrl->mSpeed = 0.0f;
    }

    void setBvaFrameAndStop(MultiSceneActor* pActor, f32 frame) {
        J3DFrameCtrl* pBvaCtrl;

        pBvaCtrl = pActor->getModelManager()->getBvaCtrl();
        pBvaCtrl->mCurrentFrame = frame;

        pBvaCtrl = pActor->getModelManager()->getBvaCtrl();
        pBvaCtrl->mSpeed = 0.0f;
    }

    bool isBckStopped(MultiSceneActor* pActor) {
        return pActor->getModelManager()->isBckStopped();
    }

    bool isStep(const MultiSceneActor* pActor, s32 step) {
        return pActor->getNerveStep() == step;
    }

    bool isFirstStep(const MultiSceneActor* pActor) {
        return pActor->getNerveStep() == 0;
    }

    bool isGreaterStep(const MultiSceneActor* pActor, s32 step) {
        return pActor->getNerveStep() > step;
    }

    bool isLessStep(const MultiSceneActor* pActor, s32 step) {
        return pActor->getNerveStep() < step;
    }

    bool isLessEqualStep(const MultiSceneActor* pActor, s32 step) {
        return pActor->getNerveStep() <= step;
    }

    void setNerveAtStep(MultiSceneActor* pActor, const Nerve* pNerve, s32 step) {
        if (isStep(pActor, step)) {
            pActor->setNerve(pNerve);
        }
    }

    void setNerveAtBckStopped(MultiSceneActor* pActor, const Nerve* pNerve) {
        if (isBckStopped(pActor)) {
            pActor->setNerve(pNerve);
        }
    }

    f32 calcNerveRate(const MultiSceneActor* pActor, s32 stepMax) {
        return stepMax <= 0 ? 1.0f : MR::clamp(static_cast<f32>(pActor->getNerveStep()) / stepMax, 0.0f, 1.0f);
    }

    f32 calcNerveRate(const MultiSceneActor* pActor, s32 stepMin, s32 stepMax) {
        return MR::clamp(MR::normalize(pActor->getNerveStep(), stepMin, stepMax), 0.0f, 1.0f);
    }

    f32 calcNerveEaseOutRate(const MultiSceneActor* pActor, s32 stepMax) {
        return MR::getEaseOutValue(calcNerveRate(pActor, stepMax), 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveValue(const MultiSceneActor* pActor, s32 stepMax, f32 valueStart, f32 valueEnd) {
        return MR::getLinerValue(calcNerveRate(pActor, stepMax), valueStart, valueEnd, 1.0f);
    }

    void emitEffect(const MultiSceneActor* pActor, const char* pParam2) {
        pActor->getEffectKeeper()->create(pParam2, pActor->getEffectSystem());
    }

    void deleteEffectAll(const MultiSceneActor* pActor) {
        pActor->getEffectKeeper()->deleteAll();
    }

    void forceDeleteEffectAll(const MultiSceneActor* pActor) {
        pActor->getEffectKeeper()->forceDeleteAll(pActor->getEffectSystem());
    }

    MtxPtr getJointMtx(const MultiSceneActor* pActor, const char* pParam2) {
        return MR::getJointMtx(pActor->getModelManager()->getJ3DModel(), (const char*)pParam2);
    }
};

void MultiSceneActor::control() {
    
}
