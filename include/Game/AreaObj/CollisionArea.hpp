#pragma once

#include "Game/AreaObj/AreaObj.hpp"
#include "Game/MapObj/DynamicCollisionObj.hpp"

class AreaPolygon : public DynamicCollisionObj {
public:
    AreaPolygon();

    virtual ~AreaPolygon();

    virtual void init(const JMapInfoIter& rIter);

    void setMtx(MtxPtr pMatrix, const TVec3f& rSize, f32 offset);
    void setSurfaceAndSync(s32 surface);
    void setSurface(s32 surface);
    void invalidate();
    void validate();

    /* 0x120 */ u8 _120[4];
    /* 0x124 */ AreaForm* mForm;
    /* 0x128 */ MtxPtr _128;
    /* 0x12C */ TVec3f _12C;
    /* 0x138 */ f32 _138;
};

class CollisionArea : public AreaObj {
public:
    CollisionArea(int formType, const char* pName);

    virtual ~CollisionArea();

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();

    bool hitCheck(const TVec3f& rPos, f32 radius, TVec3f* pPoint, TVec3f* pNormal);

    /* 0x3C */ u32 _3C;
    /* 0x40 */ f32 _40;
    /* 0x44 */ TVec3f _44;
    /* 0x50 */ s32 _50;
    /* 0x54 */ s32 _54;
    /* 0x58 */ bool _58;
    /* 0x59 */ u8 _59[3];
    /* 0x5C */ s32 _5C;
    /* 0x60 */ s32 _60;
    /* 0x64 */ AreaPolygon* mPolygon;
    /* 0x68 */ bool mIsValid;
    /* 0x69 */ u8 _69[3];
};