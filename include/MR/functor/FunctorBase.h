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

        u32 _4;
        u32 _8;
        u32 _C;
        u32 _10;
    };
};

#endif // FUNCTORBASE_H