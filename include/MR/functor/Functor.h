#pragma once

#include "MR/functor/FunctorBase.h"

class JKRHeap;

namespace MR
{
    template<typename T, typename T2>
    class FunctorV0M : public FunctorBase 
    {
    public:
        inline FunctorV0M() { }

        virtual void operator()(void);
        virtual FunctorV0M<T, T2>* clone(JKRHeap *) const;

        T _4;
        u32 _8;
        s32 _C;
        T2 mFuncPtr; // _10
    };
};