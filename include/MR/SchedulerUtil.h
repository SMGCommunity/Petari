#ifndef SCHEDULERUTIL_H
#define SCHEDULERUTIL_H

namespace MR
{
    class ProhibitSchedulerAndInterrupts
    {
    public:
        ProhibitSchedulerAndInterrupts(bool);
        ~ProhibitSchedulerAndInterrupts();
    };
};

#endif // SCHEDULERUTIL_H