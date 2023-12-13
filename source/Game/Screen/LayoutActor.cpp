#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/PaneEffectKeeper.hpp"
#include "Game/Util/LayoutUtil.hpp"

LayoutActor::LayoutActor(const char *pName, bool) : NameObj(pName),
    mManager(nullptr), mSpine(nullptr), mPaneEffectKeeper(nullptr), mStarPointerTargetKeeper(nullptr) {
}

void LayoutActor::movement() {
    if (!MR::isDead(this)) {
        updateSpine();

        if (!MR::isDead(this)) {
            control();
            if (mManager && !MR::isStoppedAnimFrame(this)) {
                mManager->movement();
            }
        }
    }
}

void LayoutActor::calcAnim() {
    if (MR::isExecuteCalcAnimLayout(this)) {
        mManager->calcAnim();
    }
}

void LayoutActor::draw() const {
    if (MR::isExecuteDrawLayout(this)) {
        mManager->draw();
    }
}

void LayoutActor::appear() {
    mFlags.mIsDead = false;
    calcAnim();
}

void LayoutActor::kill() {
    if (mPaneEffectKeeper) {
        mPaneEffectKeeper->clear();
    }

    mFlags.mIsDead = true;
}

void LayoutActor::setNerve(const Nerve *pNerve) const {
    mSpine->setNerve(pNerve);
}

bool LayoutActor::isNerve(const Nerve *pNerve) const {
    return !(mSpine->getCurrentNerve() != pNerve);
}

s32 LayoutActor::getNerveStep() const {
    return mSpine->mStep;
}

/*
const TVec2f LayoutActor::getTrans() const {
    TVec2f trans;
    MR::copyPaneTrans(&trans, this, nullptr);
    MR::convertLayoutPosToScreenPos(&trans, trans);
    return trans;
}*/

// LayoutActor::setTrans

LayoutManager* LayoutActor::getLayoutManager() const {
    return mManager;
}

// LayoutActor::createPaneMtxRef
// LayoutActor::getPaneMtxRef

void LayoutActor::initLayoutManager(const char *pName, u32 a2) {
    mManager = new LayoutManager(pName, true, a2, 0x100);
}

void LayoutActor::initLayoutManagerNoConvertFilename(const char *pName, u32 a2) {
    mManager = new LayoutManager(pName, false, a2, 0x100);
}

void LayoutActor::initLayoutManagerWithTextBoxBufferLength(const char *pName, u32 textBoxBufLen, u32 a3) {
    mManager = new LayoutManager(pName, false, a3, textBoxBufLen);
}

void LayoutActor::initNerve(const Nerve *pNerve) {
    mSpine = new Spine(this, pNerve);
}

// LayoutActor::initEffectKeeper
// LayoutActor::initPointingTarget

void LayoutActor::updateSpine() {
    if (mSpine) {
        mSpine->update();
    }
}
