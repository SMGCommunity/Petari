#pragma once

#include "Game/AreaObj/AreaObj.h"
#include "Game/MapObj/DynamicCollisionObj.h"

class AreaPolygon : public DynamicCollisionObj {
public:
    AreaPolygon();
    virtual ~AreaPolygon();

    virtual void init(const JMapInfoIter &);

    void setMtx(MtxPtr, const TVec3f &, float a3);
    void setSurfaceAndSync(long);
    void setSurface(long);
    void invalidate();
    void validate();

    u8 _120[4];
    AreaForm *mForm;    // _124
    MtxPtr _128;
    TVec3f _12C;
    f32 _138;
};

class CollisionArea : public AreaObj {
public:
    CollisionArea(int, const char *);
    virtual ~CollisionArea();

    virtual void init(const JMapInfoIter &);
    virtual void movement();
    
    bool hitCheck(const TVec3f &, float, TVec3f *, TVec3f *);

    u32 _3C;
    f32 _40;
    TVec3f _44;
    s32 _50;
    s32 _54;
    bool _58;
    u8 _59[3];
    s32 _5C;
    s32 _60;
    AreaPolygon *mPolygon;  // _64
    bool mIsValid;          // _68
    u8 _69[3];
};