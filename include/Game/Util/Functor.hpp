#pragma once

#include "Inline.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>

namespace MR {
    class FunctorBase {
    public:
        virtual void operator()() const = 0;
        virtual FunctorBase* clone(JKRHeap*) const = 0;
    };

    template < typename T, typename U >
    class FunctorV0M : public FunctorBase {
    public:
        inline FunctorV0M(T call, U callee) : mCaller(call), mCallee(callee) {
        }

        inline FunctorV0M() {
        }

        virtual void operator()() const {
            (mCaller->*mCallee)();
        }

        virtual FunctorBase* clone(JKRHeap* pHeap) const {
            return new (pHeap, 0) FunctorV0M(*this);
        };

        T mCaller;
        U mCallee;
    };

    template < typename T, typename U, typename V >
    class FunctorV1M : public FunctorBase {
    public:
        inline FunctorV1M(T call, U callee, V arg_0) {
            mCaller = call;
            mCallee = callee;
            mArg0 = arg_0;
        }

        inline FunctorV1M() {
        }

        virtual void operator()() const {
            (mCaller->*mCallee)(mArg0);
        }

        virtual FunctorBase* clone(JKRHeap* pHeap) const {
            return new (pHeap, 0) FunctorV1M(*this);
        };

        T mCaller;
        U mCallee;
        V mArg0;
    };

    template < typename T, typename U, typename V, typename W >
    class FunctorV2M : public FunctorBase {
    public:
        inline FunctorV2M(T call, U callee, V arg_0, W arg_1) {
            mCaller = call;
            mCallee = callee;
            mArg0 = arg_0;
            mArg1 = arg_1;
        }

        inline FunctorV2M() {
        }

        virtual void operator()() const {
            (mCaller->*mCallee)(mArg0, mArg1);
        }

        virtual FunctorBase* clone(JKRHeap* pHeap) const {
            return new (pHeap, 0) FunctorV2M(*this);
        };

        T mCaller;
        U mCallee;
        V mArg0;
        W mArg1;
    };

    template < class T >
    FunctorV0M< T*, void (T::*)() > Functor(T* pObject, void (T::*pFunction)()) {
        return FunctorV0M< T*, void (T::*)() >(pObject, pFunction);
    }

    template < class T >
    static FunctorV0M< const T*, void (T::*)() const > Functor(const T* pObject, void (T::*pFunction)() const) {
        return FunctorV0M< const T*, void (T::*)() const >(pObject, pFunction);
    }

    template < class T, typename U >
    static FunctorV1M< T*, void (T::*)(U), U > Functor(T* pObject, void (T::*pFunction)(U), U arg_0) {
        return FunctorV1M< T*, void (T::*)(U), U >(pObject, pFunction, arg_0);
    }

    template < class T, typename U >
    static FunctorV1M< const T*, void (T::*)(U), U > Functor(const T* pObject, void (T::*pFunction)(U) const, U arg_0) {
        return FunctorV1M< const T*, void (T::*)(U) const, U >(pObject, pFunction, arg_0);
    }

    template < class T, typename U, typename V >
    static FunctorV2M< T*, void (T::*)(U, V), U, V > Functor(T* pObject, void (T::*pFunction)(U, V), U arg_0, V arg_1) {
        return FunctorV2M< T*, void (T::*)(U, V), U, V >(pObject, pFunction, arg_0, arg_1);
    }

    template < class T, typename U, typename V >
    static FunctorV2M< const T*, void (T::*)(U, V) const, U, V > Functor(const T* pObject, void (T::*pFunction)(U, V) const, U arg_0, V arg_1) {
        return FunctorV2M< const T*, void (T::*)(U, V) const, U, V >(pObject, pFunction, arg_0, arg_1);
    }

    class FunctorV0F : public FunctorBase {
    public:
        inline FunctorV0F(void (*pFunction)(void)) {
            mFunc = pFunction;
        };

        inline FunctorV0F() {
        }

        virtual void operator()() const {
            (*mFunc)();
        }
        virtual FunctorBase* clone(JKRHeap* pHeap) const {
            return new (pHeap, 0) FunctorV0F(*this);
        }

        /* 0x04 */ void (*mFunc)();
    };

    static FunctorV0F Functor(void (*pFunction)()) {
        return FunctorV0F(pFunction);
    }
};  // namespace MR
