#include "Game/LiveActor/Binder.hpp"
#include "Game/Map/HitInfo.hpp"

HitInfo &HitInfo::operator=(const HitInfo &rOther)
{
    mParentTriangle.mParts = rOther.mParentTriangle.mParts;
    mParentTriangle.mIdx = rOther.mParentTriangle.mIdx;
    mParentTriangle.mSensor = rOther.mParentTriangle.mSensor;
    mParentTriangle.mNormals[0] = rOther.mParentTriangle.mNormals[0];
    mParentTriangle.mNormals[1] = rOther.mParentTriangle.mNormals[1];
    mParentTriangle.mNormals[2] = rOther.mParentTriangle.mNormals[2];
    mParentTriangle.mNormals[3] = rOther.mParentTriangle.mNormals[3];
    mParentTriangle.mPos[0] = rOther.mParentTriangle.mPos[0];
    mParentTriangle.mPos[1] = rOther.mParentTriangle.mPos[1];
    mParentTriangle.mPos[2] = rOther.mParentTriangle.mPos[2];
    _60 = rOther._60;
    _64 = rOther._64;
    _70 = rOther._70;
    _7C = rOther._7C;
    _88 = rOther._88;

    return *this;
}

Binder::Binder(MtxPtr mtx, const TVec3f *v1, const TVec3f *v2, f32 a, f32 b, u32 c) : BinderParent(mtx), _10(v1), _14(v2), _18(a), _1C(b), _20(0), _24(c), _28(0), _2C(0), _30(0, 0, 0), _3C(), _CC(), _15C(), _C8(131076.953125f), _158(131076.953125f), _1E8(131076.953125f)
{
    if (!_24) {
        _2C = nullptr;
    }
    else {
        _2C = new HitInfo[_24];
    }
    clear();
    _1EC._0 = true;
    _1EC._1 = true;
    _1EC._2 = false;
    _1EC._3 = false;
    _1EC._4 = false;
    _1EC._5 = false;
}
