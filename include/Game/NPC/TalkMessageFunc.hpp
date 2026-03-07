#pragma once

#include "Inline.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>

class TalkMessageFuncBase {
public:
    virtual bool operator()(u32) const = 0;
    virtual TalkMessageFuncBase* clone() const = 0;
    virtual TalkMessageFuncBase* clone(JKRHeap*) const = 0;
};

template < typename T, typename U >
class TalkMessageFuncM : public TalkMessageFuncBase {
public:
    inline TalkMessageFuncM(T call, U callee) : mCaller(call), mCallee(callee) {
    }

    inline TalkMessageFuncM() {
    }

    virtual bool operator()(u32 arg) const {
        (mCaller->*mCallee)(arg);
    }

    virtual TalkMessageFuncM* clone() const {
        return new TalkMessageFuncM(*this);
    }

    virtual TalkMessageFuncM* clone(JKRHeap* pHeap) const {
        return new (pHeap, 0) TalkMessageFuncM(*this);
    };

    T mCaller;
    U mCallee;
};

template < class T >
static TalkMessageFuncM< T*, bool (T::*)(u32) > TalkMessageFunc(T* a1, bool (T::*a2)(u32)) NO_INLINE {
    return TalkMessageFuncM< T*, bool (T::*)(u32) >(a1, a2);
}

template < class T >
static inline TalkMessageFuncM< T*, bool (T::*)(u32) > TalkMessageFunc_Inline(T* a1, bool (T::*a2)(u32)) {
    return TalkMessageFuncM< T*, bool (T::*)(u32) >(a1, a2);
}
