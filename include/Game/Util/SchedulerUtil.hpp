#pragma once

namespace MR {
    class ProhibitSchedulerAndInterrupts {
    public:
        ProhibitSchedulerAndInterrupts(bool unused);

        ~ProhibitSchedulerAndInterrupts();

        int mInterruptsStatus;
    };
};  // namespace MR
