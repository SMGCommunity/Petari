#pragma once

class Triangle;

typedef bool (*TriangleFunc)(const Triangle*);

class TriangleFilterBase {
public:
    /*TriangleFilterBase(TriangleFunc function) {
        mFunction = function;
    }*/

    virtual bool isInvalidTriangle(const Triangle*) const = 0;

    //    TriangleFunc mFunction; // 0x4
};

class TriangleFilterFunc : public TriangleFilterBase {
public:
    TriangleFilterFunc(TriangleFunc func) : mFunction(func) {
    }

    virtual bool isInvalidTriangle(const Triangle* pTriangle) const {
        return mFunction(pTriangle);
    }

    /* 0x04 */ TriangleFunc mFunction;
};

namespace MR {
    TriangleFilterFunc* createTriangleFilterFunc(TriangleFunc);
};  // namespace MR

template < typename T >
class TriangleFilterDelegator : public TriangleFilterBase {
public:
    typedef bool (T::*DelegateFilter)(const Triangle*);
    TriangleFilterDelegator(T* parent, DelegateFilter filter) : mParent(parent), mFunc(filter) {
    }

    virtual bool isInvalidTriangle(const Triangle* pTriangle) const {
        return (mParent->*mFunc)(pTriangle);
    }

    /* 0x04 */ T* mParent;
    /* 0x10 */ DelegateFilter mFunc;
};

namespace MR {
    template < class T >
    static inline TriangleFilterDelegator< T >* createTriangleFilterDelegator(T* parent, bool (T::*filter)(const Triangle*)) {
        return new TriangleFilterDelegator< T >(parent, filter);
    }
};  // namespace MR
