#pragma once

#include <revolution.h>
#include "Game/System/NerveExecutor.h"
#include "Inline.h"

namespace MR {
    bool isStep(const NerveExecutor *pExecutor, s32 step) NO_INLINE;
    bool isFirstStep(const NerveExecutor *);
    bool isLessStep(const NerveExecutor *, s32);
    bool isLessEqualStep(const NerveExecutor *, s32);
    bool isGreaterStep(const NerveExecutor *, s32);
    bool isGreaterEqualStep(const NerveExecutor *, s32);
    bool isIntervalStep(const NerveExecutor *, s32);
    bool isNewNerve(const NerveExecutor *);
    f32 calcNerveRate(const NerveExecutor *, s32);
    f32 calcNerveEaseInRate(const NerveExecutor *, s32);
    f32 calcNerveEaseOutRate(const NerveExecutor *, s32);
    f32 calcNerveValue(const NerveExecutor *, s32, f32, f32);
    f32 calcNerveEaseInOutValue(const NerveExecutor *, s32, f32, f32);
    f32 calcNerveEaseInOutValue(const NerveExecutor *, s32, s32, f32, f32);
    void setNerveAtStep(NerveExecutor *, const Nerve *, s32);
};