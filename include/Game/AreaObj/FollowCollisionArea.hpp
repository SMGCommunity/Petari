#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry.hpp>

class AreaPolygon;

class FollowCollisionArea : public NameObj {
public:
    FollowCollisionArea(const TVec3f&, MtxPtr, f32);

    virtual ~FollowCollisionArea();
    virtual void movement();

    u32 _C;
    f32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    s32 _20;
    s32 _24;
    u8 _28;
    u32 _2C;
    AreaPolygon* _30;
    MtxPtr _34;
    TVec3f _38;
    f32 _44;
};
