#include "Game/System/NerveExecutor.hpp"
#include "Game/LiveActor/Spine.hpp"

NerveExecutor::NerveExecutor(const char* pName) : mSpine(nullptr) {}

NerveExecutor::~NerveExecutor() {
    delete mSpine;
}

void NerveExecutor::initNerve(const Nerve* pNerve) {
    mSpine = new Spine(this, pNerve);
}

void NerveExecutor::updateNerve() {
    if (mSpine != nullptr) {
        mSpine->update();
    }
}

void NerveExecutor::setNerve(const Nerve* pNerve) {
    mSpine->setNerve(pNerve);
}

bool NerveExecutor::isNerve(const Nerve* pNerve) const {
    return mSpine->getCurrentNerve() == pNerve;
}

s32 NerveExecutor::getNerveStep() const {
    return mSpine->mStep;
}
