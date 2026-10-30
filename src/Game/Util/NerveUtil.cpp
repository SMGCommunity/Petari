#include "Game/Util/NerveUtil.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/MathUtil.hpp"

bool MR::isStep(const NerveExecutor* pExecutor, s32 step) {
    return pExecutor->getNerveStep() == step;
}

bool MR::isFirstStep(const NerveExecutor* pExecutor) {
    return isStep(pExecutor, 0);
}

bool MR::isLessStep(const NerveExecutor* pExecutor, s32 step) {
    return pExecutor->getNerveStep() < step;
}

bool MR::isLessEqualStep(const NerveExecutor* pExecutor, s32 step) {
    return pExecutor->getNerveStep() <= step;
}

bool MR::isGreaterStep(const NerveExecutor* pExecutor, s32 step) {
    return pExecutor->getNerveStep() > step;
}

bool MR::isGreaterEqualStep(const NerveExecutor* pExecutor, s32 step) {
    return pExecutor->getNerveStep() >= step;
}

bool MR::isIntervalStep(const NerveExecutor* pExecutor, s32 step) {
    return pExecutor->getNerveStep() % step == 0;
}

bool MR::isNewNerve(const NerveExecutor* pExecutor) {
    return pExecutor->getNerveStep() < 0;
}

f32 MR::calcNerveRate(const NerveExecutor* pExecutor, s32 stepMax) {
    return stepMax <= 0 ? 1.0f : clamp(static_cast< f32 >(pExecutor->getNerveStep()) / stepMax, 0.0f, 1.0f);
}

f32 MR::calcNerveEaseInRate(const NerveExecutor* pExecutor, s32 stepMax) {
    return getEaseInValue(calcNerveRate(pExecutor, stepMax), 0.0f, 1.0f, 1.0f);
}

f32 MR::calcNerveEaseOutRate(const NerveExecutor* pExecutor, s32 stepMax) {
    return getEaseOutValue(calcNerveRate(pExecutor, stepMax), 0.0f, 1.0f, 1.0f);
}

f32 MR::calcNerveValue(const NerveExecutor* pExecutor, s32 stepMax, f32 valueStart, f32 valueEnd) {
    return getLinerValue(calcNerveRate(pExecutor, stepMax), valueStart, valueEnd, 1.0f);
}

f32 MR::calcNerveEaseInOutValue(const NerveExecutor* pExecutor, s32 stepMax, f32 valueStart, f32 valueEnd) {
    return getEaseInOutValue(calcNerveRate(pExecutor, stepMax), valueStart, valueEnd, 1.0f);
}

f32 MR::calcNerveEaseInOutValue(const NerveExecutor* pExecutor, s32 stepMin, s32 stepMax, f32 valueStart, f32 valueEnd) {
    f32 rate = clamp(normalize(pExecutor->getNerveStep(), stepMin, stepMax), 0.0f, 1.0f);

    return getEaseInOutValue(rate, valueStart, valueEnd, 1.0f);
}

void MR::setNerveAtStep(NerveExecutor* pExecutor, const Nerve* pNerve, s32 step) {
    if (pExecutor->getNerveStep() == step) {
        pExecutor->setNerve(pNerve);
    }
}
