#pragma once

class JKRHeap;

namespace MR {
    class FunctorBase {
    public:
        virtual void operator()() const = 0;
        virtual FunctorBase* clone(JKRHeap *) const = 0;
    };

    template<typename T, typename U>
    class FunctorV0M : public FunctorBase {
    public:

        FunctorV0M(T call, U callee) 
            : mCaller(call), mCallee(callee) {
        }

        virtual void operator()() const = 0;
        virtual FunctorBase* clone(JKRHeap *) const = 0;

        T mCaller;
        U mCallee;
    };

    template<class T>
    class Functor {
    public:
    
    };
};