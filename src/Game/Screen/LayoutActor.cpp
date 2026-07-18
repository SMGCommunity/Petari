#include "Game/Screen/LayoutActor.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Screen/LayoutCoreUtil.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include "Game/Screen/PaneEffectKeeper.hpp"
#include "Game/Screen/StarPointerTarget.hpp"
#include "Game/Util/LayoutUtil.hpp"

LayoutActor::LayoutActor(const char* pName, bool) : NameObj(pName), mLayoutManager(), mSpine(), mEffectKeeper(), mPointingTarget() {
}

void LayoutActor::movement() {
    if (MR::isDead(this)) {
        return;
    }

    updateSpine();

    if (MR::isDead(this)) {
        return;
    }

    control();

    if (mLayoutManager != nullptr && !MR::isStopAnimFrame(this)) {
        mLayoutManager->movement();
    }
}

void LayoutActor::calcAnim() {
    if (MR::isExecuteCalcAnimLayout(this)) {
        mLayoutManager->calcAnim();
    }
}

void LayoutActor::draw() const {
    if (MR::isExecuteDrawLayout(this)) {
        mLayoutManager->draw();
    }
}

void LayoutActor::appear() {
    mFlag.mIsDead = false;

    calcAnim();
}

void LayoutActor::kill() {
    if (mEffectKeeper != nullptr) {
        mEffectKeeper->clear();
    }

    mFlag.mIsDead = true;
}

void LayoutActor::setNerve(const Nerve* pNerve) const {
    mSpine->setNerve(pNerve);
}

bool LayoutActor::isNerve(const Nerve* pNerve) const {
    return mSpine->getCurrentNerve() == pNerve;
}

s32 LayoutActor::getNerveStep() const {
    return mSpine->mStep;
}

TVec2f LayoutActor::getTrans() const {
    TVec2f trans;

    MR::copyPaneTrans(&trans, this, nullptr);
    MR::convertLayoutPosToScreenPos(&trans, trans);

    return trans;
}

void LayoutActor::setTrans(const TVec2f& rTrans) {
    TVec2f trans;
    nw4r::lyt::Pane* rootPane;

    MR::convertScreenPosToLayoutPos(&trans, rTrans);

    rootPane = MR::getRootPane(this);
    rootPane->mTranslate.x = trans.x;
    rootPane->mTranslate.y = trans.y;
    rootPane->mTranslate.z = 0.0f;
}

LayoutManager* LayoutActor::getLayoutManager() const {
    return mLayoutManager;
}

void LayoutActor::createPaneMtxRef(const char* pParam1) {
    mLayoutManager->createPaneMtxRef(pParam1);
}

MtxPtr LayoutActor::getPaneMtxRef(const char* pParam1) {
    return mLayoutManager->getPaneMtxRef(pParam1);
}

void LayoutActor::initLayoutManager(const char* pName, u32 a2) {
    mLayoutManager = new LayoutManager(pName, true, a2, 0x100);
}

void LayoutActor::initLayoutManagerNoConvertFilename(const char* pName, u32 a2) {
    mLayoutManager = new LayoutManager(pName, false, a2, 0x100);
}

void LayoutActor::initLayoutManagerWithTextBoxBufferLength(const char* pName, u32 textBoxBufferLength, u32 a3) {
    mLayoutManager = new LayoutManager(pName, false, a3, textBoxBufferLength);
}

void LayoutActor::initNerve(const Nerve* pNerve) {
    mSpine = new Spine(this, pNerve);
}

void LayoutActor::initEffectKeeper(int param1, const char* pParam2, const EffectSystem* pEffectSystem) {
    mEffectKeeper = new PaneEffectKeeper(this, mLayoutManager, param1, pParam2);
    mEffectKeeper->init(this, pEffectSystem);
}

void LayoutActor::initPointingTarget(int maxNumTargets) {
    mPointingTarget = new StarPointerLayoutTargetKeeper(maxNumTargets);
}

void LayoutActor::updateSpine() {
    if (mSpine != nullptr) {
        mSpine->update();
    }
}
