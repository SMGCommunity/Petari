#pragma once

class Binder {
public:
    void clear();
    void setTriangleFilter(TriangleFilterBase *);
    const Triangle* getPlane(int) const;

	int _0;
	int _4;
	int _8;
    TMtx34f *_c;
    const TVec3f *_10;
    const TVec3f *_14;
    f32 _18;
    f32 _1c;
    int _20;
    u32 _24;
    int _28;
    HitInfo** _2c;
    TVec3f _30;
    HitInfo _3c;
    f32 _c8;
    HitInfo _cc;
    f32 _158;
    HitInfo _15c;
    f32 _1e8;
    char _1ec;
};
