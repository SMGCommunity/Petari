#include "Game/Util/NerveUtil.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/MathUtil.hpp"

namespace MR {
    bool isStep(const NerveExecutor* pExecutor, s32 step) {
        return pExecutor->getNerveStep() == step;
    }

    bool isFirstStep(const NerveExecutor* pExecutor) {
        return isStep(pExecutor, 0);
    }

    bool isLessStep(const NerveExecutor* pExecutor, s32 step) {
        return pExecutor->getNerveStep() < step;
    }

    bool isLessEqualStep(const NerveExecutor* pExecutor, s32 step) {
        return pExecutor->getNerveStep() <= step;
    }

    bool isGreaterStep(const NerveExecutor* pExecutor, s32 step) {
        return pExecutor->getNerveStep() > step;
    }

    bool isGreaterEqualStep(const NerveExecutor* pExecutor, s32 step) {
        return pExecutor->getNerveStep() >= step;
    }

    bool isIntervalStep(const NerveExecutor* pExecutor, s32 step) {
        return pExecutor->getNerveStep() % step == 0;
    }

    bool isNewNerve(const NerveExecutor* pExecutor) {
        return pExecutor->getNerveStep() < 0;
    }

    f32 calcNerveRate(const NerveExecutor* pExecutor, s32 stepMax) {
        return stepMax <= 0 ? 1.0f : clamp(static_cast< f32 >(pExecutor->getNerveStep()) / stepMax, 0.0f, 1.0f);
    }

    f32 calcNerveEaseInRate(const NerveExecutor* pExecutor, s32 stepMax) {
        return getEaseInValue(calcNerveRate(pExecutor, stepMax), 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveEaseOutRate(const NerveExecutor* pExecutor, s32 stepMax) {
        return getEaseOutValue(calcNerveRate(pExecutor, stepMax), 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveValue(const NerveExecutor* pExecutor, s32 stepMax, f32 valueStart, f32 valueEnd) {
        return getLinerValue(calcNerveRate(pExecutor, stepMax), valueStart, valueEnd, 1.0f);
    }

    f32 calcNerveEaseInOutValue(const NerveExecutor* pExecutor, s32 stepMax, f32 valueStart, f32 valueEnd) {
        return getEaseInOutValue(calcNerveRate(pExecutor, stepMax), valueStart, valueEnd, 1.0f);
    }

    f32 calcNerveEaseInOutValue(const NerveExecutor* pExecutor, s32 stepMin, s32 stepMax, f32 valueStart, f32 valueEnd) {
        f32 rate = clamp(normalize(pExecutor->getNerveStep(), stepMin, stepMax), 0.0f, 1.0f);

        return getEaseInOutValue(rate, valueStart, valueEnd, 1.0f);
    }

    void setNerveAtStep(NerveExecutor* pExecutor, const Nerve* pNerve, s32 step) {
        if (pExecutor->getNerveStep() == step) {
            pExecutor->setNerve(pNerve);
        }
    }
};  // namespace MR
