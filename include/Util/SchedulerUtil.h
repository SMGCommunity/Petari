#pragma once

namespace MR
{
    class ProhibitSchedulerAndInterrupts
    {
    public:
        ProhibitSchedulerAndInterrupts(bool);
        ~ProhibitSchedulerAndInterrupts();
    };
};