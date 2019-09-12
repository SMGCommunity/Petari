#ifndef FUNCTORBASE_H
#define FUNCTORBASE_H

#include "types.h"

class JKRHeap;

namespace MR
{
    class FunctorBase 
    {
    public:
        virtual void operator()(void) = 0;
        virtual FunctorBase* clone(JKRHeap *) const = 0;
    };
};

#endif // FUNCTORBASE_H