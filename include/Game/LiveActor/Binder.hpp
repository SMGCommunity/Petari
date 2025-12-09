#pragma once

#include "Game/Map/HitInfo.hpp"
#include "Game/Util/TriangleFilter.hpp"

class BinderParent {
public:
    inline BinderParent(MtxPtr mtx) {
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
    Binder(MtxPtr, const TVec3f*, const TVec3f*, f32, f32, u32);
    void clear();
    void setTriangleFilter(TriangleFilterBase*);
    const Triangle* getPlane(int) const;
    u32 copyPlaneArrayAndSortingSensor(HitInfo**, u32);
    const TVec3f bind(const TVec3f&);

    const TVec3f* _10;
    const TVec3f* _14;
    f32 mRadius;  // 0x18
    f32 _1C;
    const TVec3f* mOffsetVec;  // 0x20
    u32 _24;
    int mPlaneNum;              // 0x28
    HitInfo* mPlaneInfos;       // 0x2C
    TVec3f mFixReactionVector;  // 0x30
    HitInfo mGroundInfo;        // 0x3c
    f32 _C8;
    HitInfo mWallInfo;  // 0xcc
    f32 _158;
    HitInfo mRoofInfo;  // 0x15c
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
