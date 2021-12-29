#pragma once

class NerveExecutor;

namespace MR {
    bool isFirstStep(const NerveExecutor *);

    bool isStep(const NerveExecutor *, s32);
};