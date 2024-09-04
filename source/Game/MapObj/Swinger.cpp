#include "Game/MapObj/Swinger.hpp"
#include "Game/Util/MathUtil.hpp"

Swinger::Swinger(const TVec3f *a1, MtxPtr a2, float a3, float a4, float a5, const TVec3f *a6)
    : _0(a1), _4(0), _8(0.0f), mAcceleration(0.0f), _20(0.0f, -1.0f, 0.0f), _2C(a3), _30(a4), _34(a5), _38(a6),
      _3C((*a2)[0], (*a2)[4], (*a2)[8]), _48((*a2)[1], (*a2)[5], (*a2)[9]), _54((*a2)[2], (*a2)[6], (*a2)[10])
{
    _8.set(_0->x, (_0->y - _2C), _0->z);
    _60.identity();
    _60.setInline(a2);
    _60.setTrans(*_0);
}

void Swinger::accel(const TVec3f& a1)
{
    mAcceleration.add(a1);
}

void Swinger::update()
{
    TVec3f v20;
    if (_0) {
        v20.set(*_0);
    }
    else {
        v20.set((*_4)[3], (*_4)[7], (*_4)[11]);
    }
    mAcceleration.scale(_34);
    if (_38) {
        TVec3f v19(*_38);
        v19.scale(_30);
        mAcceleration.add(v19);
    }
    else {
        mAcceleration.y -= _30;
    }
    _8.add(mAcceleration);
    TVec3f v18(_8);
    v18.subInline4(v20);
    _20.set(v18);

    if ((_20.squaredInline() <= 0.0000038146973f) > 0) {
        _20.setInline(0.0f, -1.0f, 0.0f);
    }
    else {
        MR::normalize(&_20);
    }
    _8.set(_20);
    _8.scale(_2C);
    _8.add(v20);

    f32 xval = mAcceleration.x - (_20.x * ((_20.z * mAcceleration.z) + ((_20.x * mAcceleration.x) + (_20.y * mAcceleration.y))));
    f32 yval = mAcceleration.y - (_20.y * ((_20.z * mAcceleration.z) + ((_20.x * mAcceleration.x) + (_20.y * mAcceleration.y))));
    f32 zval = mAcceleration.z - (_20.z * ((_20.z * mAcceleration.z) + ((_20.x * mAcceleration.x) + (_20.y * mAcceleration.y))));
    mAcceleration.x = xval;
    mAcceleration.y = yval;
    mAcceleration.z = zval;

    updateSwingMtx(v20);
}

void Swinger::updateSwingMtx(const TVec3f &a1)
{
    _48.set(_20);
    _48.x *= -1.0f;
    _48.y *= -1.0f;
    _48.z *= -1.0f;
    TVec3f v14(_3C);
    PSVECCrossProduct(_48.toCVec(), _54.toCVec(), _3C.toVec());
    if (MR::isNearZero(_3C, 0.001f)) {
        _3C.set(v14);
    }
    MR::normalize(&_3C);
    PSVECCrossProduct(_3C.toCVec(), _48.toCVec(), _54.toVec());
    MR::normalize(&_54);

    _60.mMtx[0][0] = _3C.x;
    _60.mMtx[1][0] = _3C.y;
    _60.mMtx[2][0] = _3C.z;
    _60.mMtx[0][1] = _48.x;
    _60.mMtx[1][1] = _48.y;
    _60.mMtx[2][1] = _48.z;
    _60.mMtx[0][2] = _54.x;
    _60.mMtx[1][2] = _54.y;
    _60.mMtx[2][2] = _54.z;

    _60.setTrans(a1);
}