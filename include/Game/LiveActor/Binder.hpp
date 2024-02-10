#pragma once

#include "Game/Map/HitInfo.hpp"
#include "Game/Util/TriangleFilter.hpp"

class BinderParent {
public:
    inline BinderParent(MtxPtr mtx)
    {
        _C = mtx;
        _0 = 0;
        _4 = 0;
        _8 = 0;
    }

    int _0;
    int _4;
    int _8;
    MtxPtr _C;
};

class Binder : BinderParent {
public:
    Binder(MtxPtr, const TVec3f *, const TVec3f *, f32, f32, u32);
    void clear();
    void setTriangleFilter(TriangleFilterBase *);
    const Triangle *getPlane(int) const;

    const TVec3f *_10;
    const TVec3f *_14;
    f32 _18;
    f32 _1C;
    int _20;
    u32 _24;
    int _28;
    HitInfo *_2C;
    TVec3f _30;
    HitInfo _3C;
    f32 _C8;
    HitInfo _CC;
    f32 _158;
    HitInfo _15C;
    f32 _1E8;

    struct {
        bool _0 : 1;
        bool _1 : 1;
        bool _2 : 1;
        bool _3 : 1;
        bool _4 : 1;
        bool _5 : 1;
    } _1EC;
};
