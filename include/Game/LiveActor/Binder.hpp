#pragma once

#include "Game/Map/HitInfo.hpp"

class Binder {
public:
    void clear();
    void setTriangleFilter(TriangleFilterBase *);
    const Triangle* getPlane(int) const;

    int _0;
    int _4;
    int _8;
    TMtx34f *_C;
    const TVec3f *_10;
    const TVec3f *_14;
    f32 _18;
    f32 _1C;
    int _20;
    u32 _24;
    int _28;
    HitInfo** _2C;
    TVec3f _30;
    HitInfo _3C;
    f32 _C8;
    HitInfo _CC;
    f32 _158;
    HitInfo _15C;
    f32 _1E8;
    char _1EC;
};
