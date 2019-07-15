#ifndef FUNCTOR_H
#define FUNCTOR_H

#include "MR/functor/FunctorBase.h"

class JKRHeap;

namespace MR
{
    template<typename T, void* T2>
    class FunctorV0M : public FunctorBase 
    {
    public:
        virtual void operator()(void);
        virtual FunctorV0M<T, T2>* clone(JKRHeap *) const;
    };
};

#endif // FUNCTOR_H