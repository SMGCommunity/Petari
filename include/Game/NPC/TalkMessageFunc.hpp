#pragma once

#include "Inline.h"
#include <JSystem/JKernel/JKRHeap.h>

class TalkMessageFuncBase {
public:
    virtual void operator()() const = 0;
    virtual TalkMessageFuncBase* clone() const = 0;
    virtual TalkMessageFuncBase* clone(JKRHeap *) const = 0;
};

template<typename T, typename U>
class TalkMessageFuncM : public TalkMessageFuncBase {
public:

    inline TalkMessageFuncM(T call, U callee) 
        : mCaller(call), mCallee(callee) {
    }

    inline TalkMessageFuncM() {
        
    }

    virtual void operator()() const {
        (mCaller->*mCallee)();
    }

    virtual TalkMessageFuncM* clone() const {
        return new TalkMessageFuncM(*this);
    }

    virtual TalkMessageFuncM* clone(JKRHeap *pHeap) const {
        return new (pHeap, 0) TalkMessageFuncM(*this);
    };

    T mCaller;
    U mCallee;
};

template<class T>
static TalkMessageFuncM<T *, void (T::*)()> TalkMessageFunc(T* a1, void (T::*a2)()) NO_INLINE {
    return TalkMessageFuncM<T *, void (T::*)()>(a1, a2);
}