#pragma once

class Triangle;

typedef bool (*TriangleFunc)(const Triangle *);

class TriangleFilterBase {
public:
    TriangleFilterBase(TriangleFunc function) {
        mFunction = function;
    }

    virtual bool isInvalidTriangle(const Triangle *) const = 0;

    TriangleFunc mFunction; // _4
};

class TriangleFilterFunc : public TriangleFilterBase {
public:
    TriangleFilterFunc(TriangleFunc func) : TriangleFilterBase(func) {

    }

    virtual bool isInvalidTriangle(const Triangle *) const;
};

namespace MR {
    TriangleFilterFunc* createTriangleFilterFunc(TriangleFunc);
};
