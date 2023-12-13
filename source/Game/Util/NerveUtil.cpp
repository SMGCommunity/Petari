#include "Game/Util/NerveUtil.hpp"
#include "Game/Util.hpp"

namespace MR {
    bool isStep(const NerveExecutor *pExecutor, s32 step) {
        return pExecutor->getNerveStep() == step;
    }

    bool isFirstStep(const NerveExecutor *pExecutor) {
        return MR::isStep(pExecutor, 0);
    }

    bool isLessStep(const NerveExecutor *pExecutor, s32 step) {
        return pExecutor->getNerveStep() < step;
    }

    bool isLessEqualStep(const NerveExecutor *pExecutor, s32 step) {
        return pExecutor->getNerveStep() <= step;
    }

    bool isGreaterStep(const NerveExecutor *pExecutor, s32 step) {
        return pExecutor->getNerveStep() > step;
    }

    bool isGreaterEqualStep(const NerveExecutor *pExecutor, s32 step) {
        return pExecutor->getNerveStep() >= step;
    }

    bool isIntervalStep(const NerveExecutor *pExecutor, s32 step) {
        s32 curStep = pExecutor->getNerveStep();
        s32 ratio = curStep / step;
        s32 mult = ratio * step;
        return mult == curStep;
    }

    bool isNewNerve(const NerveExecutor *pExecutor) {
        return pExecutor->getNerveStep() < 0;
    }

    f32 calcNerveRate(const NerveExecutor *pExecutor, s32 a2) {
        f32 rate;
        if (a2 <= 0) {
            rate = 1.0f;
        }
        else {
            rate = MR::clamp((f32)pExecutor->getNerveStep() / (f32)a2, 0.0f, 1.0f);
        }

        return rate;
    }

    f32 calcNerveEaseInRate(const NerveExecutor *pExecutor, s32 a2) {
        f32 rate;
        if (a2 <= 0) {
            rate = 1.0f;
        }
        else {
            rate = MR::clamp((f32)pExecutor->getNerveStep() / (f32)a2, 0.0f, 1.0f);
        }

        return MR::getEaseInValue(rate, 0.0f, 1.0f, 1.0f); 
    }

    f32 calcNerveEaseOutRate(const NerveExecutor *pExecutor, s32 a2) {
        f32 rate;
        if (a2 <= 0) {
            rate = 1.0f;
        }
        else {
            rate = MR::clamp((f32)pExecutor->getNerveStep() / (f32)a2, 0.0f, 1.0f);
        }

        return MR::getEaseOutValue(rate, 0.0f, 1.0f, 1.0f); 
    }

    f32 calcNerveValue(const NerveExecutor *pExecutor, s32 a2, f32 a3, f32 a4) {
        f32 rate;
        if (a2 <= 0) {
            rate = 1.0f;
        }
        else {
            rate = MR::clamp((f32)pExecutor->getNerveStep() / (f32)a2, 0.0f, 1.0f);
        }

        return MR::getLinerValue(rate, a3, a4, 1.0f);       
    }

    f32 calcNerveEaseInOutValue(const NerveExecutor *pExecutor, s32 a2, f32 a3, f32 a4) {
        f32 rate;
        if (a2 <= 0) {
            rate = 1.0f;
        }
        else {
            rate = MR::clamp((f32)pExecutor->getNerveStep() / (f32)a2, 0.0f, 1.0f);
        }

        return MR::getEaseInOutValue(rate, a3, a4, 1.0f);
    }

    f32 calcNerveEaseInOutValue(const NerveExecutor *pExecutor, s32 a2, s32 a3, f32 a4, f32 a5) {
        f32 norm = MR::normalize(pExecutor->getNerveStep(), a2, a3);
        f32 clamp = MR::clamp(norm, 0.0f, 1.0f);
        return MR::getEaseInOutValue(clamp, a4, a5, 1.0f);
    }

    void setNerveAtStep(NerveExecutor *pExecutor, const Nerve *pNerve, s32 step) {
        if (step == pExecutor->getNerveStep()) {
            pExecutor->setNerve(pNerve);
        }
    }
};