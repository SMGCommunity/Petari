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
    /*TriangleFilterFunc(TriangleFunc func) : TriangleFilterBase(func) {

    }*/

    virtual bool isInvalidTriangle(const Triangle*) const;
};

namespace MR {
    TriangleFilterFunc* createTriangleFilterFunc(TriangleFunc);
};

template < typename T >
class TriangleFilterDelegator : public TriangleFilterBase {
public:
    typedef bool (T::*DelegateFilter)(const Triangle*);
    TriangleFilterDelegator(T* parent, DelegateFilter filter) : mParent(parent), mFunc(filter) {}

    virtual bool isInvalidTriangle(const Triangle*) const;

    static inline TriangleFilterDelegator< T >* allocateDelegator(T* parent, DelegateFilter filter) {
        return new TriangleFilterDelegator< T >(parent, filter);
    }

    T* mParent;            // 0x4
    DelegateFilter mFunc;  // 0x10
};
