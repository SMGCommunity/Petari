#pragma once

#include "Inline.hpp"

class JKRHeap;

namespace MR {
    class FunctorBase {
    public:
        virtual void operator()() const = 0;
        virtual FunctorBase* clone(JKRHeap*) const = 0;
    };

    template < typename T, typename U >
    class FunctorV0M : public FunctorBase {
    public:
        inline FunctorV0M(T call, U callee) : mCaller(call), mCallee(callee) {}

        inline FunctorV0M() {}

        virtual void operator()() const { (mCaller->*mCallee)(); }

        virtual FunctorBase* clone(JKRHeap* pHeap) const { return new (pHeap, 0) FunctorV0M(*this); };

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

        inline FunctorV1M() {}

        virtual void operator()() const { (mCaller->*mCallee)(mArg0); }

        virtual FunctorBase* clone(JKRHeap* pHeap) const { return new (pHeap, 0x16) FunctorV1M(*this); };

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

        inline FunctorV2M() {}

        virtual void operator()() const { (mCaller->*mCallee)(mArg0, mArg1); }

        virtual FunctorBase* clone(JKRHeap* pHeap) const { return new (pHeap, 0x16) FunctorV2M(*this); };

        T mCaller;
        U mCallee;
        V mArg0;
        W mArg1;
    };

    template < class T >
    static FunctorV0M< T*, void (T::*)() > Functor(T* a1, void (T::*a2)()) NO_INLINE {
        return FunctorV0M< T*, void (T::*)() >(a1, a2);
    }

    template < class T >
    static FunctorV0M< T*, void (T::*)() const > Functor(T* a1, void (T::*a2)() const) NO_INLINE {
        return FunctorV0M< T*, void (T::*)() const >(a1, a2);
    }

    template < class T >
    inline static FunctorV0M< T*, void (T::*)() > Functor_Inline(T* a1, void (T::*a2)()) {
        return FunctorV0M< T*, void (T::*)() >(a1, a2);
    }

    template < class T >
    inline static FunctorV0M< T*, void (T::*)() const > Functor_Inline(T* a1, void (T::*a2)() const) {
        return FunctorV0M< T*, void (T::*)() const >(a1, a2);
    }

    template < class T, typename U >
    static FunctorV1M< T*, void (T::*)(U), U > Functor(T* a1, void (T::*a2)(U), U arg_0) NO_INLINE {
        return FunctorV1M< T*, void (T::*)(U), U >(a1, a2, arg_0);
    }

    template < class T, typename U >
    inline static FunctorV1M< T*, void (T::*)(U), U > Functor_Inline(T* a1, void (T::*a2)(U), U arg_0) {
        return FunctorV1M< T*, void (T::*)(U), U >(a1, a2, arg_0);
    }

    template < class T, typename U, typename V >
    static FunctorV2M< T*, void (T::*)(U, V), U, V > Functor(T* a1, void (T::*a2)(U, V), U arg_0, V arg_1) {
        return FunctorV2M< T*, void (T::*)(U, V), U, V >(a1, a2, arg_0, arg_1);
    }

    class FunctorV0F : public FunctorBase {
    public:
        inline FunctorV0F(void (*func)(void)) { mFunc = func; };

        virtual void operator()() const;
        virtual FunctorBase* clone(JKRHeap*) const;

        void* mFunc;  // 0x4
    };
};  // namespace MR
