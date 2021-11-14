#include "Game/System/NerveExecutor.h"

NerveExecutor::NerveExecutor(const char *a1) : mSpine(0) {}

NerveExecutor::~NerveExecutor() {}

void NerveExecutor::initNerve(const Nerve *pNerve) {
    mSpine = new Spine(this, pNerve);
}

void NerveExecutor::updateNerve() {
    if (mSpine  != NULL) {
        mSpine->update();
    }
}

void NerveExecutor::setNerve(const Nerve* pNerve) {
    mSpine->setNerve(pNerve);
}

bool NerveExecutor::isNerve(const Nerve* pNerve) const {
    return !(mSpine->getCurrentNerve() != pNerve);
}

u32 NerveExecutor::getNerveStep() const {
    return mSpine->mStep;
}