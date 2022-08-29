#pragma once

#include "revolution.h"

class J3DShape;
class J3DJoint;
class J3DColorBlockLightOn;
class J3DTexGenBlockPatched;
class J3DTevBlockPatched;
class J3DPEBlock;
class J3DMaterialAnm;
class J3DDisplayListObj;

class J3DAlphaComp {
public:
    J3DAlphaComp();

    u16 _0;
    u8 _2;
    u8 _3;

    static u16 sUnk;
};

class J3DCurrentMtx {
public:
    void setCurrentTexMtx(u8, u8, u8, u8, u8, u8, u8, u8);

    u32 _0;
    u32 _4;
};

class J3DMaterial {
public:
    virtual void calc(const MtxPtr);
    virtual void calcDiffTexMtx(const MtxPtr);
    virtual void makeDisplayList();
    virtual void makeSharedDisplayList();
    virtual void load();
    virtual void loadSharedDL();
    virtual void patch();
    virtual void diff(u32);
    virtual void reset();
    virtual void change();

    J3DMaterial* _4;
    J3DShape* mShape;                   // _8
    J3DJoint* mJoint;                   // _C
    u32 _10;
    u16 _14;
    u8 _16;
    u8 _17;
    u32 _18;
    u8 _1C;
    u8 _1D;
    u8 _1E;
    u8 _1F;
    u32 _20;
    J3DColorBlockLightOn* _24;
    J3DTexGenBlockPatched* _28;
    J3DTevBlockPatched* _2C;
    u32 _30;
    J3DPEBlock* _34;
    J3DMaterial* _38;
    J3DMaterialAnm* _3C;
    J3DCurrentMtx mMtx;                 // _40
    J3DDisplayListObj* mDisplayListObj; // _48
};