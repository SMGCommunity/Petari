#pragma once

class NerveExecutor;

namespace MR {
    bool isFirstStep(const NerveExecutor *);

    bool isStep(const NerveExecutor *, s32);

    f32 calcNerveEaseInRate(const NerveExecutor *, s32);
    f32 calcNerveEaseOutRate(const NerveExecutor *, s32);
};