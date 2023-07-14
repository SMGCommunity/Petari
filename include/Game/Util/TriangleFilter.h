#pragma once

class Triangle;

typedef bool (*TriangleFunc)(const Triangle *);

class TriangleFilterBase {
public:
    /*TriangleFilterBase(TriangleFunc function) {
        mFunction = function;
    }*/

    virtual bool isInvalidTriangle(const Triangle *) const = 0;

//    TriangleFunc mFunction; // _4 
};

class TriangleFilterFunc : public TriangleFilterBase {
public:
    /*TriangleFilterFunc(TriangleFunc func) : TriangleFilterBase(func) {

    }*/

    virtual bool isInvalidTriangle(const Triangle *) const;
};

namespace MR {
    TriangleFilterFunc* createTriangleFilterFunc(TriangleFunc);
};

template<typename T>
class TriangleFilterDelegator: public TriangleFilterBase {
	public:
	typedef bool (T::*DelegateFilter)(const Triangle *);
	TriangleFilterDelegator(T *parent, DelegateFilter filter) : mParent(parent), mFunc(filter) {}

	virtual bool isInvalidTriangle(const Triangle *) const;

	T *mParent; // _4
	DelegateFilter mFunc; // _10
};

/*template<typename T>
struct DataWrapper {
	typedef bool (T::*DelegateFilter)(const Triangle *);
	template<DelegateFilter F>
	struct Data {
		DelegateFilter filter;
		u32 _0;
		s32 _4;
		inline Data() : filter(F) {}
	};
	//inline Data(u32 _0, s32 _4) : _0(_0), _4(_4), filter(F) const {}
};*/

