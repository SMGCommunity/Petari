#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/MathUtil.hpp"

namespace MR {
    bool isStep(const NerveExecutor* pExecutor, s32 step) {
        return pExecutor->getNerveStep() == step;
    }

    bool isFirstStep(const NerveExecutor* pExecutor) {
        return MR::isStep(pExecutor, 0);
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

    f32 calcNerveRate(const NerveExecutor* pExecutor, s32 maxStep) {
        f32 rate;

        if (maxStep <= 0) {
            rate = 1.0f;
        }
        else {
            rate = MR::clamp(static_cast<f32>(pExecutor->getNerveStep()) / maxStep, 0.0f, 1.0f);
        }

        return rate;
    }

    f32 calcNerveEaseInRate(const NerveExecutor* pExecutor, s32 maxStep) {
        f32 rate;

        if (maxStep <= 0) {
            rate = 1.0f;
        }
        else {
            rate = MR::clamp(static_cast<f32>(pExecutor->getNerveStep()) / maxStep, 0.0f, 1.0f);
        }

        return MR::getEaseInValue(rate, 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveEaseOutRate(const NerveExecutor* pExecutor, s32 maxStep) {
        f32 rate;

        if (maxStep <= 0) {
            rate = 1.0f;
        }
        else {
            rate = MR::clamp(static_cast<f32>(pExecutor->getNerveStep()) / maxStep, 0.0f, 1.0f);
        }

        return MR::getEaseOutValue(rate, 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveValue(const NerveExecutor* pExecutor, s32 maxStep, f32 startValue, f32 endValue) {
        f32 rate;

        if (maxStep <= 0) {
            rate = 1.0f;
        }
        else {
            rate = MR::clamp(static_cast<f32>(pExecutor->getNerveStep()) / maxStep, 0.0f, 1.0f);
        }

        return MR::getLinerValue(rate, startValue, endValue, 1.0f);
    }

    f32 calcNerveEaseInOutValue(const NerveExecutor* pExecutor, s32 maxStep, f32 startValue, f32 endValue) {
        f32 rate;

        if (maxStep <= 0) {
            rate = 1.0f;
        }
        else {
            rate = MR::clamp(static_cast<f32>(pExecutor->getNerveStep()) / maxStep, 0.0f, 1.0f);
        }

        return MR::getEaseInOutValue(rate, startValue, endValue, 1.0f);
    }

    f32 calcNerveEaseInOutValue(const NerveExecutor* pExecutor, s32 minStep, s32 maxStep, f32 startValue, f32 endValue) {
        f32 rate = MR::clamp(MR::normalize(pExecutor->getNerveStep(), minStep, maxStep), 0.0f, 1.0f);

        return MR::getEaseInOutValue(rate, startValue, endValue, 1.0f);
    }

    void setNerveAtStep(NerveExecutor* pExecutor, const Nerve* pNerve, s32 step) {
        if (pExecutor->getNerveStep() == step) {
            pExecutor->setNerve(pNerve);
        }
    }
};
