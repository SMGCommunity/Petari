#pragma once

#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphAnimator/J3DMtxCalc.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class XtransformInfo {
public:
    XtransformInfo();

    XtransformInfo& operator=(const XtransformInfo&);

    TVec3f _0;
    TVec3f _C;
    f32 _18;
    f32 _1C;
    f32 _20;
    f32 _24;
};

class XjointTransform {
public:
    XjointTransform();

    u32 _0;
    s16 _4;
    f32 _8;
    f32 _C;
    f32 _10;
    TVec3f _14;
    f32 _20;
    f32 _24;
    f32 _28;
    f32 _2C;
    f32 _30;
    f32 _34;
    TVec3f _38;
    f32 _44;
    f32 _48;
    f32 _4C;
    u16 _50;
    u16 _52;
    u16 _54;
    u16 _56;
    f32 _58;
    f32 _5C;
    f32 _60;
    MtxPtr _64;
    u32 _68;
    MtxPtr _6C;
};

class XjointInfo {
public:
    XjointInfo();

    XtransformInfo _0;
    XtransformInfo _28;
    TVec3f _50;
    f32 _5C;
    f32 _60;
};

class XanimeTrack {
public:
    void init();

    u32 _0;
    f32 mWeight;  // 0x4
    f32 _8;
    u8 _C;
};

class XanimeCore : public J3DMtxCalc {
public:
    XanimeCore(u32, u32, u8);
    XanimeCore(u32, XanimeCore*);

    virtual ~XanimeCore();
    virtual void setWeight(u8, f32);
    virtual void init(const Vec&, const Mtx&);
    virtual void calc();

    void shareJointTransform(const XanimeCore*);
    void doFreeze();
    void setBck(u32, J3DAnmTransform*);
    void freezeCopy(J3DModelData*, XanimeCore*, u32, u32);

    void enableJointTransform(J3DModelData*);

    void initMember(u32);

    XjointTransform* getJointTransform(u32);

    u8 _4;
    u8 mTrackCount;  // 0x5
    u8 _6;
    u32 mJointCount;  // 0x8
    u16 _C;
    XjointInfo* mJointList;           // 0x10
    XjointTransform* mTransformList;  // 0x14  // this is an array
    XanimeTrack* mTrackList;          // 0x18
    f32 _1C;
    f32 _20;
    f32 _24;
    u8 _28;
    u8 _29;
};
