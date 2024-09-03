#pragma once

#include <revolution.h>
#include "JSystem/J3DGraphBase/J3DMatPacket.hpp"
#include "JSystem/J3DGraphBase/J3DMatBlock.hpp"
#include "JSystem/J3DGraphBase/J3DStruct.hpp"

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
    J3DShape* mShape;                   // 0x8
    J3DJoint* mJoint;                   // 0xC
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
    J3DColorBlockLightOn* mColorBlock;      // 0x24
    J3DTexGenBlockPatched* mTexGenBlock;    // 0x28
    J3DTevBlockPatched* mTevBlock;          // 0x2C
    u32 mIndBlock;                          // 0x30
    J3DPEBlock* mPEBlock;                   // 0x34
    J3DMaterial* mOrigMaterial;             // 0x38
    J3DMaterialAnm* mMaterialAnm;           // 0x3C
    J3DCurrentMtx mMtx;                     // 0x40
    J3DDisplayListObj* mDisplayListObj;     // 0x48
};

class J3DPEBlock {
public:
    //inline J3DPEBlock() { }

    virtual void reset(J3DPEBlock *);
    virtual void load() = 0;
    virtual void patch();
    virtual void diff();
    virtual void diffFog();
    virtual void diffBlend();
    virtual u32 countDLSize();
    virtual u32 getType() = 0;
    virtual void setFog(J3DFog);
    virtual void setFog(J3DFog *);
    virtual J3DFogInfo& getFog();
    virtual void setAlphaComp(const J3DAlphaComp *);
    virtual void setAlphaComp(const J3DAlphaComp &);
    virtual J3DAlphaComp* getAlphaComp();
    virtual void setBlend(const J3DBlend *);
    virtual void setBlend(const J3DBlend &);
    virtual J3DBlend* getBlend();
    virtual void setZMode(const J3DZMode *);
    virtual void setZMode(const J3DZMode &);
    virtual J3DZMode* getZMode();
    virtual void setZCompLoc(const u8 *);
    virtual void setZCompLoc(u8);
    virtual u8* getZCompLoc() const;
    virtual void setDither(const u8 *);
    virtual void setDither(u8);
    virtual u8 getDither() const;
    virtual u32 getFogOffset() const;
    virtual void setFogOffset(u32);
    virtual ~J3DPEBlock();
};

class J3DPEBlockFull : public J3DPEBlock {
public:
    //inline J3DPEBlock() { }

    virtual void reset(J3DPEBlock *);
    virtual void load();
    virtual void patch();
    virtual void diff();
    virtual void diffFog();
    virtual void diffBlend();
    virtual u32 countDLSize();
    virtual u32 getType();
    virtual void setFog(J3DFog);
    virtual void setFog(J3DFog *);
    virtual J3DFogInfo& getFog();
    virtual void setAlphaComp(const J3DAlphaComp *);
    virtual void setAlphaComp(const J3DAlphaComp &);
    virtual J3DAlphaComp* getAlphaComp();
    virtual void setBlend(const J3DBlend *);
    virtual void setBlend(const J3DBlend &);
    virtual J3DBlend* getBlend();
    virtual void setZMode(const J3DZMode *);
    virtual void setZMode(const J3DZMode &);
    virtual J3DZMode* getZMode();
    virtual void setZCompLoc(const u8 *);
    virtual void setZCompLoc(u8);
    virtual u8* getZCompLoc() const;
    virtual void setDither(const u8 *);
    virtual void setDither(u8);
    virtual u8 getDither() const;
    virtual u32 getFogOffset() const;
    virtual void setFogOffset(u32);
    virtual ~J3DPEBlockFull();

    J3DFogInfo mFogInfo;            // 0x4
    J3DAlphaComp mAlphaComp;        // 0x30
    J3DBlend mBlend;                // 0x34
    J3DZMode mZMode;                // 0x38
    const u8 mDither;               // 0x3B
    u32 mFogOffset;                 // 0x3C
};