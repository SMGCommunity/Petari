#pragma once

#include "Game/AreaObj/AreaObj.hpp"
#include "Game/MapObj/DynamicCollisionObj.hpp"

class AreaPolygon : public DynamicCollisionObj {
public:
    AreaPolygon();
    virtual ~AreaPolygon();

    virtual void init(const JMapInfoIter&);

    void setMtx(MtxPtr, const TVec3f&, f32 a3);
    void setSurfaceAndSync(s32);
    void setSurface(s32);
    void invalidate();
    void validate();

    u8 _120[4];
    AreaForm* mForm;  // 0x124
    MtxPtr _128;
    TVec3f _12C;
    f32 _138;
};

class CollisionArea : public AreaObj {
public:
    CollisionArea(int, const char*);
    virtual ~CollisionArea();

    virtual void init(const JMapInfoIter&);
    virtual void movement();

    bool hitCheck(const TVec3f&, f32, TVec3f*, TVec3f*);

    u32 _3C;
    f32 _40;
    TVec3f _44;
    s32 _50;
    s32 _54;
    bool _58;
    u8 _59[3];
    s32 _5C;
    s32 _60;
    AreaPolygon* mPolygon;  // 0x64
    bool mIsValid;          // 0x68
    u8 _69[3];
};