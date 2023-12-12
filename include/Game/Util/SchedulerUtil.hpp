#pragma once

namespace MR {
    class ProhibitSchedulerAndInterrupts {
    public:
        ProhibitSchedulerAndInterrupts(bool);

        ~ProhibitSchedulerAndInterrupts();

        u32 _0;
    };
};