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

        inline FunctorV0M(T call, U callee) 
            : mCaller(call), mCallee(callee) {
        }

        inline FunctorV0M() {

        }

        virtual void operator()() const {
            
        }

        virtual FunctorBase* clone(JKRHeap *pHeap) const {
            return new (pHeap, 0x16) FunctorV0M(*this);
        };

        T mCaller;
        U mCallee;
    };

    template<class T>
    static FunctorV0M<T *, void (T::*)()> Functor(T* a1, void (T::*a2)()) {
        return FunctorV0M<T *, void (T::*)()>(a1, a2);
    }
};