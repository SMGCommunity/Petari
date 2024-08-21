#pragma once

#include <revolution.h>
#include <JSystem/J3DGraphBase/J3DStruct.hpp>

struct J3DBlendInfo {
    u8 mType;           // 0x0
    u8 mSrcFactor;      // 0x1
    u8 mDstFactor;      // 0x2
    u8 mOp;             // 0x3
};

class J3DBlend : public J3DBlendInfo { };

struct J3DColorChanInfo {
    u8 _0;
    u8 _1;
    u8 _2;
    u8 _3;
    u8 _4;
    u8 _5;
    u8 _6;
    u8 _7;
};

struct J3DColorChan {
    u16 mColorChanID;       // 0x0
};

struct J3DZModeInfo {
    u8 _0;
    u8 _1;
    u8 _2;
    u8 _3;
};

class J3DZMode { 
public:
    u16 _0;
};

class J3DColorBlock {
public:
    virtual void load();
    virtual void reset(J3DColorBlock *);
    virtual void patch();
    virtual void patchMatColor();
    virtual void patchLight();
    virtual void diff(u32);
    virtual void diffMatColor();
    virtual void diffLight();
    virtual s32 countDLSize();
    virtual u32 getType() = 0;
    virtual void setMatColor(u32, J3DGXColor const *);
    virtual void setMatColor(u32, J3DGXColor);
    virtual _GXColor* getMatColor(u32);
    virtual void setAmbColor(u32, J3DGXColor const *);
    virtual void setAmbColor(u32, J3DGXColor);
    virtual bool getAmbColor(u32);
    virtual void setColorChanNum(u8);
    virtual void setColorChanNum(u8 const *);
    virtual u8 getColorChanNum() const;
    virtual void setColorChan(u32, J3DColorChan const &);
    virtual void setColorChan(u32, J3DColorChan const *);
    virtual J3DColorChan* getColorChan(u32);
    virtual void setLight(u32, J3DLightObj *);
    virtual bool getLight(u32);
    virtual void setCullMode(u8 const*);
    virtual void setCullMode(u8);
    virtual s32 getCullMode() const;
    virtual bool getMatColorOffset() const;
    virtual bool getColorChanOffset() const;
    virtual void setMatColorOffset(u32);
    virtual void setColorChanOffset(u32);
    virtual ~J3DColorBlock() {}
};

class J3DColorBlockLightOn : public J3DColorBlock {
public:
    virtual void load();
    virtual void reset(J3DColorBlock *);
    virtual void patch();
    virtual void patchMatColor();
    virtual void patchLight();
    virtual void diff(u32);
    virtual void diffMatColor();
    virtual void diffLight();
    virtual s32 countDLSize();
    virtual u32 getType();
    virtual void setMatColor(u32, J3DGXColor const *);
    virtual void setMatColor(u32, J3DGXColor);
    virtual _GXColor* getMatColor(u32);
    virtual void setAmbColor(u32, J3DGXColor const *);
    virtual void setAmbColor(u32, J3DGXColor);
    virtual bool getAmbColor(u32);
    virtual void setColorChanNum(u8);
    virtual void setColorChanNum(u8 const *);
    virtual u8 getColorChanNum() const;
    virtual void setColorChan(u32, J3DColorChan const &);
    virtual void setColorChan(u32, J3DColorChan const *);
    virtual J3DColorChan* getColorChan(u32);
    virtual void setLight(u32, J3DLightObj *);
    virtual bool getLight(u32);
    virtual void setCullMode(u8 const*);
    virtual void setCullMode(u8);
    virtual s32 getCullMode() const;
    virtual bool getMatColorOffset() const;
    virtual bool getColorChanOffset() const;
    virtual void setMatColorOffset(u32);
    virtual void setColorChanOffset(u32);
    virtual ~J3DColorBlockLightOn() {}

    J3DGXColor mMatColor[2];
    J3DGXColor mAmbColor[2];
    u8 mColorChanNum;
    J3DColorChan mColorChan[4];
    J3DLightObj* mLightObjs[8];
    u8 mCullMode;
    u32 mMatColorOffs;
    u32 mColorChanOffs;
};
