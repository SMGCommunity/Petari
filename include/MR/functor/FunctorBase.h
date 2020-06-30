#pragma once

#include <revolution.h>

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