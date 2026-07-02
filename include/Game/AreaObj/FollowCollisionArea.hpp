#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry.hpp>

class AreaPolygon;

class FollowCollisionArea : public NameObj {
public:
    FollowCollisionArea(const TVec3f&, MtxPtr, f32);

    virtual ~FollowCollisionArea();
    virtual void movement();

    bool hitCheck(const TVec3f&, f32, TVec3f*, TVec3f*);

    /* 0x0C */ u32 _C;
    /* 0x10 */ f32 _10;
    /* 0x14 */ u32 _14;
    /* 0x18 */ u32 _18;
    /* 0x1C */ u32 _1C;
    /* 0x20 */ s32 _20;
    /* 0x24 */ s32 _24;
    /* 0x28 */ bool _28;
    /* 0x2C */ u32 _2C;
    /* 0x30 */ AreaPolygon* _30;
    /* 0x34 */ MtxPtr _34;
    /* 0x38 */ TVec3f _38;
    /* 0x44 */ f32 _44;
};
