#pragma once

#include "JSystem/J3DGraphBase/J3DStruct.hpp"
#include "JSystem/J3DGraphBase/J3DTevs.hpp"
#include "JSystem/J3DGraphBase/J3DTexture.hpp"
#include <revolution.h>

struct J3DGXColorS10 : public GXColorS10 {
    J3DGXColorS10() {}

    J3DGXColorS10(J3DGXColorS10& other) { __memcpy(this, &other, sizeof(J3DGXColorS10)); }

    J3DGXColorS10(const GXColorS10& color) : GXColorS10(color) {}

    J3DGXColorS10& operator=(const GXColorS10& color) {
        // Fakematch? Instruction order is wrong with __memcpy or GXColorS10::operator=
        // Might be real as this matches on debug as well.
        ((u32*)this)[0] = ((u32*)&color)[0];
        ((u32*)this)[1] = ((u32*)&color)[1];
        return *this;
    }
};

struct J3DIndTexOrder : public J3DIndTexOrderInfo {
    J3DIndTexOrder() { J3DIndTexOrderInfo::operator=(j3dDefaultIndTexOrderNull); }
    J3DIndTexOrder& operator=(J3DIndTexOrder const& other) {
#if DEBUG
        J3DIndTexOrderInfo::operator=(other);
#else
        // Fakematch: Instruction order is wrong with __memcpy or J3DIndTexCoordScaleInfo::operator=
        *(u32*)this = *(u32*)&other;
#endif
        return *this;
    }
    J3DIndTexOrder(J3DIndTexOrderInfo const& info) { J3DIndTexOrderInfo::operator=(info); }
    u8 getMap() const { return (GXTexMapID)mMap; }
    u8 getCoord() const { return (GXTexCoordID)mCoord; }
};  // Size: 0x04

struct J3DIndTexCoordScale : public J3DIndTexCoordScaleInfo {
    J3DIndTexCoordScale() { J3DIndTexCoordScaleInfo::operator=(j3dDefaultIndTexCoordScaleInfo); }
    J3DIndTexCoordScale(const J3DIndTexCoordScaleInfo& info) { J3DIndTexCoordScaleInfo::operator=(info); }
    J3DIndTexCoordScale(const J3DIndTexCoordScale& other) { __memcpy(this, &other, sizeof(J3DIndTexCoordScale)); }
    ~J3DIndTexCoordScale() {}
    u8 getScaleS() { return mScaleS; }
    u8 getScaleT() { return mScaleT; }

    J3DIndTexCoordScale& operator=(const J3DIndTexCoordScale& other) {
#if DEBUG
        J3DIndTexCoordScaleInfo::operator=(other);
#else
        // Fakematch: Instruction order is wrong with __memcpy or J3DIndTexCoordScaleInfo::operator=
        *(u32*)this = *(u32*)&other;
#endif
        return *this;
    }
};  // Size: 0x4

struct J3DBlend : public J3DBlendInfo {
    J3DBlend() { J3DBlendInfo::operator=(j3dDefaultBlendInfo); }
    J3DBlend(J3DBlendInfo const& info) { J3DBlendInfo::operator=(info); }

    void setType(u8 i_type) { mType = i_type; }
    void setSrcFactor(u8 i_factor) { mSrcFactor = i_factor; }
    void setDstFactor(u8 i_factor) { mDstFactor = i_factor; }

    GXBlendMode getBlendMode() const { return (GXBlendMode)mType; }
    GXBlendFactor getSrcFactor() const { return (GXBlendFactor)mSrcFactor; }
    GXBlendFactor getDstFactor() const { return (GXBlendFactor)mDstFactor; }
    GXLogicOp getLogicOp() const { return (GXLogicOp)mOp; }

    void load(u8 ditherEnable) const {
        J3DGDSetBlendMode((GXBlendMode)mType, (GXBlendFactor)mSrcFactor, (GXBlendFactor)mDstFactor, (GXLogicOp)mOp, ditherEnable);
    }

    void setBlendInfo(const J3DBlendInfo& i_blendInfo) { *static_cast< J3DBlendInfo* >(this) = i_blendInfo; }
};

struct J3DFog : public J3DFogInfo {
    J3DFog() { *(J3DFogInfo*)this = j3dDefaultFogInfo; }
    ~J3DFog() {}
    J3DFog* getFogInfo() { return this; }
    void setFogInfo(J3DFogInfo info) { *(J3DFogInfo*)this = info; }
    void setFogInfo(const J3DFogInfo* info) { *(J3DFogInfo*)this = *info; }

    void load() const {
        J3DGDSetFog(GXFogType(mType), mStartZ, mEndZ, mNearZ, mFarZ, mColor);
        J3DGDSetFogRangeAdj(mAdjEnable, mCenter, (GXFogAdjTable*)&mFogAdjTable);
    }
};

inline u16 calcAlphaCmpID(u8 comp0, u8 op, u8 comp1) {
    return (comp0 << 5) + (op << 3) + (comp1);
}

struct J3DAlphaComp {
    J3DAlphaComp() NO_INLINE : mID(j3dDefaultAlphaCmpID), mRef0(0), mRef1(0) {}
    J3DAlphaComp(u16 id) : mID(id), mRef0(0), mRef1(0) {}

    explicit J3DAlphaComp(const J3DAlphaCompInfo& info) {
        mID = calcAlphaCmpID(info.mComp0, info.mOp, info.mComp1);
        mRef0 = info.mRef0;
        mRef1 = info.mRef1;
    }

    J3DAlphaComp& operator=(u16 id) {
        mID = id;
        return *this;
    }

    J3DAlphaComp& operator=(const J3DAlphaComp& rhs) {
        mID = rhs.mID;
        mRef0 = rhs.mRef0;
        mRef1 = rhs.mRef1;
        return *this;
    }

    J3DAlphaComp& operator=(J3DAlphaComp& rhs) {
        mID = rhs.mID;
        mRef0 = rhs.mRef0;
        mRef1 = rhs.mRef1;
        return *this;
    }

    void setAlphaCompInfo(const J3DAlphaCompInfo& info) {
        mRef0 = info.mRef0;
        mRef1 = info.mRef1;
        mID = calcAlphaCmpID(info.mComp0, info.mOp, info.mComp1);
    }

    u8 getComp0() const { return *(&j3dAlphaCmpTable[mID * 3] + 0); }
    u8 getOp() const { return *(&j3dAlphaCmpTable[mID * 3] + 1); }
    u8 getComp1() const { return *(&j3dAlphaCmpTable[mID * 3] + 2); }
    u8 getRef0() const { return mRef0; }
    u8 getRef1() const { return mRef1; }

    void load() const { J3DGDSetAlphaCompare((GXCompare)getComp0(), mRef0, (GXAlphaOp)getOp(), (GXCompare)getComp1(), mRef1); }

    /* 0x00 */ u16 mID;
    /* 0x02 */ u8 mRef0;
    /* 0x03 */ u8 mRef1;
};  // Size: 0x4

inline u16 calcColorChanID(u16 enable, u8 matSrc, u8 lightMask, u8 diffuseFn, u8 attnFn, u8 ambSrc) {
    u32 reg = 0;
    reg = (reg & ~0x0002) | enable << 1;
    reg = (reg & ~0x0001) | matSrc;
    reg = (reg & ~0x0040) | ambSrc << 6;
    reg = (reg & ~0x0004) | bool(lightMask & 0x01) << 2;
    reg = (reg & ~0x0008) | bool(lightMask & 0x02) << 3;
    reg = (reg & ~0x0010) | bool(lightMask & 0x04) << 4;
    reg = (reg & ~0x0020) | bool(lightMask & 0x08) << 5;
    reg = (reg & ~0x0800) | bool(lightMask & 0x10) << 11;
    reg = (reg & ~0x1000) | bool(lightMask & 0x20) << 12;
    reg = (reg & ~0x2000) | bool(lightMask & 0x40) << 13;
    reg = (reg & ~0x4000) | bool(lightMask & 0x80) << 14;
    reg = (reg & ~0x0180) | (attnFn == GX_AF_SPEC ? 0 : diffuseFn) << 7;
    reg = (reg & ~0x0200) | (attnFn != GX_AF_NONE) << 9;
    reg = (reg & ~0x0400) | (attnFn != GX_AF_SPEC) << 10;
    return reg;
}

static inline u32 setChanCtrlMacro(u8 enable, GXColorSrc ambSrc, GXColorSrc matSrc, u32 lightMask, GXDiffuseFn diffuseFn, GXAttnFn attnFn) {
    return matSrc << 0 | enable << 1 | (lightMask & 0x0F) << 2 | ambSrc << 6 | ((attnFn == GX_AF_SPEC) ? GX_DF_NONE : diffuseFn) << 7 |
           (attnFn != GX_AF_NONE) << 9 | (attnFn != GX_AF_SPEC) << 10 | (lightMask >> 4 & 0x0F) << 11;
}

class J3DColorChan {
public:
    J3DColorChan() NO_INLINE { setColorChanInfo(j3dDefaultColorChanInfo); }
    J3DColorChan(J3DColorChanInfo const& info) {
        u32 ambSrc = info.mAmbSrc == 0xFF ? 0 : info.mAmbSrc;
        mColorChanID = calcColorChanID(info.mEnable, info.mMatSrc, info.mLightMask, info.mDiffuseFn, info.mAttnFn, ambSrc);
    }

    inline void setColorChanInfo(J3DColorChanInfo const& info) {
        // !@bug: It compares info.mAmbSrc (an 8 bit integer) with 0xFFFF instead of 0xFF.
        // This inline is only called by the default constructor J3DColorChan().
        // The J3DColorChan(const J3DColorChanInfo&) constructor does not call this inline, and instead duplicates the
        // same logic but without the bug.
        // See J3DMaterialFactory::newColorChan - both the bugged and correct behavior are present there, as it calls
        // both constructors.
        u32 ambSrc = info.mAmbSrc == 0xFFFF ? 0 : info.mAmbSrc;
        mColorChanID = calcColorChanID(info.mEnable, info.mMatSrc, info.mLightMask, info.mDiffuseFn, info.mAttnFn, ambSrc);
    }
    u8 getLightMask() const { return ((mColorChanID >> 2) & 0xf) | ((mColorChanID >> 11) & 0xf) << 4; }
    void setLightMask(u8 param_1) {
        mColorChanID = (mColorChanID & ~0x3c) | ((param_1 & 0xf) << 2);
        mColorChanID = (mColorChanID & ~0x7800) | ((param_1 & 0xf0) << 7);
    }

    u8 getEnable() const { return (u32)(mColorChanID & 0x2) >> 1; }
    u8 getAmbSrc() const { return (GXColorSrc)((u32)(mColorChanID & (1 << 6)) >> 6); }
    u8 getMatSrc() const { return (GXColorSrc)(mColorChanID & 1); }
    u8 getDiffuseFn() const { return ((u32)(mColorChanID & (3 << 7)) >> 7); }
    u8 getAttnFn() const {
#ifndef DECOMPCTX
        u8 AttnArr[] = {2, 0, 2, 1};
#endif
        return AttnArr[(u32)(mColorChanID & (3 << 9)) >> 9];
    }
    J3DColorChan& operator=(const J3DColorChan& other) {
        mColorChanID = other.mColorChanID;
        return *this;
    }

    void load() const {
        J3DGDWrite_u32(setChanCtrlMacro(getEnable(), (GXColorSrc)getAmbSrc(), (GXColorSrc)getMatSrc(), getLightMask(), (GXDiffuseFn)getDiffuseFn(),
                                        (GXAttnFn)getAttnFn()));
    }

    /* 0x0 */ u16 mColorChanID;
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
    virtual void load() {}
    virtual void reset(J3DColorBlock*) {}
    virtual void patch() {}
    virtual void patchMatColor() {}
    virtual void patchLight() {}
    virtual void diff(u32) {}
    virtual void diffMatColor() {}
    virtual void diffLight() {}
    virtual s32 countDLSize() { return 0; }
    virtual u32 getType() = 0;
    virtual void setMatColor(u32, J3DGXColor const*) {}
    virtual void setMatColor(u32, J3DGXColor) {}
    virtual J3DGXColor* getMatColor(u32) { return NULL; }
    virtual void setAmbColor(u32, J3DGXColor const*) {}
    virtual void setAmbColor(u32, J3DGXColor) {}
    virtual J3DGXColor* getAmbColor(u32) { return NULL; }
    virtual void setColorChanNum(u8) {}
    virtual void setColorChanNum(u8 const*) {}
    virtual u8 getColorChanNum() const { return 0; }
    virtual void setColorChan(u32, J3DColorChan const&) {}
    virtual void setColorChan(u32, J3DColorChan const*) {}
    virtual J3DColorChan* getColorChan(u32) { return NULL; }
    virtual void setLight(u32, J3DLightObj*) {}
    virtual J3DLightObj* getLight(u32) { return NULL; }
    virtual void setCullMode(u8 const*) {}
    virtual void setCullMode(u8) {}
    virtual u8 getCullMode() const { return 2; }
    virtual u32 getMatColorOffset() const { return 0; }
    virtual u32 getColorChanOffset() const { return 0; }
    virtual void setMatColorOffset(u32) {}
    virtual void setColorChanOffset(u32) {}
    virtual ~J3DColorBlock() {}
};

class J3DColorBlockLightOff : public J3DColorBlock {
public:
    J3DColorBlockLightOff() NO_INLINE { initialize(); }
    void initialize();

    virtual s32 countDLSize();
    virtual void load();
    virtual void reset(J3DColorBlock*);
    virtual void patch();
    virtual void patchMatColor();
    virtual void patchLight();
    virtual void diff(u32);
    virtual void diffMatColor();
    virtual void diffColorChan();
    virtual u32 getType() { return 'CLOF'; }
    virtual void setMatColor(u32 idx, J3DGXColor const* color) { mMatColor[idx] = *color; }
    virtual void setMatColor(u32 idx, J3DGXColor color) { mMatColor[idx] = color; }
    virtual J3DGXColor* getMatColor(u32 idx) { return &mMatColor[idx]; }
    virtual void setColorChanNum(u8 num) { mColorChanNum = num; }
    virtual void setColorChanNum(u8 const* num) { mColorChanNum = *num; }
    virtual u8 getColorChanNum() const { return mColorChanNum; }
    virtual void setColorChan(u32 idx, J3DColorChan const& chan) { mColorChan[idx] = chan; }
    virtual void setColorChan(u32 idx, J3DColorChan const* chan) { mColorChan[idx] = *chan; }
    virtual J3DColorChan* getColorChan(u32 idx) { return &mColorChan[idx]; }
    virtual void setCullMode(u8 const* mode) { mCullMode = *mode; }
    virtual void setCullMode(u8 mode) { mCullMode = mode; }
    virtual u8 getCullMode() const { return mCullMode; }
    virtual u32 getMatColorOffset() const { return mMatColorOffset; }
    virtual u32 getColorChanOffset() const { return mColorChanOffset; }
    virtual void setMatColorOffset(u32 offset) { mMatColorOffset = offset; }
    virtual void setColorChanOffset(u32 offset) { mColorChanOffset = offset; }
    virtual ~J3DColorBlockLightOff() {}

    /* 0x04 */ J3DGXColor mMatColor[2];
    /* 0x0C */ u8 mColorChanNum;
    /* 0x0E */ J3DColorChan mColorChan[4];
    /* 0x16 */ u8 mCullMode;
    /* 0x18 */ u32 mMatColorOffset;
    /* 0x1C */ u32 mColorChanOffset;
};  // Size: 0x20

class J3DColorBlockLightOn : public J3DColorBlock {
public:
    J3DColorBlockLightOn() { initialize(); }
    void initialize();

    virtual void load();
    virtual void reset(J3DColorBlock*);
    virtual void patch();
    virtual void patchMatColor();
    virtual void patchLight();
    virtual void diff(u32);
    virtual void diffMatColor();
    virtual void diffColorChan();
    virtual s32 countDLSize();
    virtual u32 getType() { return 'CLON'; }
    virtual void setMatColor(u32 idx, J3DGXColor const* color) { mMatColor[idx] = *color; }
    virtual void setMatColor(u32 idx, J3DGXColor color) { mMatColor[idx] = color; }
    virtual J3DGXColor* getMatColor(u32 idx) { return &mMatColor[idx]; }
    virtual void setAmbColor(u32 idx, J3DGXColor const* color) { mAmbColor[idx] = *color; }
    virtual void setAmbColor(u32 idx, J3DGXColor color) { mAmbColor[idx] = color; }
    virtual J3DGXColor* getAmbColor(u32 idx) { return &mAmbColor[idx]; }
    virtual void setColorChanNum(u8 num) { mColorChanNum = num; }
    virtual void setColorChanNum(u8 const* num) { mColorChanNum = *num; }
    virtual u8 getColorChanNum() const { return mColorChanNum; }
    virtual void setColorChan(u32 idx, J3DColorChan const& chan) { mColorChan[idx] = chan; }
    virtual void setColorChan(u32 idx, J3DColorChan const* chan) { mColorChan[idx] = *chan; }
    virtual J3DColorChan* getColorChan(u32 idx) { return &mColorChan[idx]; }
    virtual void setLight(u32 idx, J3DLightObj* light) { mLight[idx] = light; }
    virtual J3DLightObj* getLight(u32 idx) { return mLight[idx]; }
    virtual void setCullMode(u8 const* mode) { mCullMode = *mode; }
    virtual void setCullMode(u8 mode) { mCullMode = mode; }
    virtual u8 getCullMode() const { return mCullMode; }
    virtual u32 getMatColorOffset() const { return mMatColorOffset; }
    virtual u32 getColorChanOffset() const { return mColorChanOffset; }
    virtual void setMatColorOffset(u32 offset) { mMatColorOffset = offset; }
    virtual void setColorChanOffset(u32 offset) { mColorChanOffset = offset; }
    virtual ~J3DColorBlockLightOn() {}

    /* 0x04 */ J3DGXColor mMatColor[2];
    /* 0x0C */ J3DGXColor mAmbColor[2];
    /* 0x14 */ u8 mColorChanNum;
    /* 0x16 */ J3DColorChan mColorChan[4];
    /* 0x20 */ J3DLightObj* mLight[8];
    /* 0x40 */ u8 mCullMode;
    /* 0x44 */ u32 mMatColorOffset;
    /* 0x48 */ u32 mColorChanOffset;
};  // Size: 0x4C

class J3DColorBlockAmbientOn : public J3DColorBlockLightOff {
public:
    J3DColorBlockAmbientOn() { initialize(); }
    void initialize();

    virtual void load();
    virtual void reset(J3DColorBlock*);
    virtual s32 countDLSize();
    virtual u32 getType() { return 'CLAB'; }
    virtual void setAmbColor(u32 idx, J3DGXColor const* color) { mAmbColor[idx] = *color; }
    virtual void setAmbColor(u32 idx, J3DGXColor color) { mAmbColor[idx] = color; }
    virtual J3DGXColor* getAmbColor(u32 idx) { return &mAmbColor[idx]; }
    virtual ~J3DColorBlockAmbientOn() {}

    /* 0x20 */ J3DGXColor mAmbColor[2];
};  // Size: 0x28

class J3DTexGenBlock {
public:
    virtual void reset(J3DTexGenBlock*) {}
    virtual void calc(f32 const (*)[4]) = 0;
    virtual void calcWithoutViewMtx(f32 const (*)[4]) = 0;
    virtual void calcPostTexMtx(f32 const (*)[4]) = 0;
    virtual void calcPostTexMtxWithoutViewMtx(f32 const (*)[4]) = 0;
    virtual void load() = 0;
    virtual void patch() = 0;
    virtual void diff(u32) = 0;
    virtual void diffTexMtx() = 0;
    virtual void diffTexGen() = 0;
    virtual s32 countDLSize() { return 0; }
    virtual u32 getType() = 0;
    virtual void setTexGenNum(u32 const*) {}
    virtual void setTexGenNum(u32) {}
    virtual u32 getTexGenNum() const { return 0; }
    virtual void setTexCoord(u32, J3DTexCoord const*) {}
    virtual J3DTexCoord* getTexCoord(u32) { return NULL; }
    virtual void setTexMtx(u32, J3DTexMtx*) {}
    virtual J3DTexMtx* getTexMtx(u32) { return NULL; }
    virtual void setNBTScale(J3DNBTScale const* scale) {}
    virtual void setNBTScale(J3DNBTScale) {}
    virtual J3DNBTScale* getNBTScale() { return NULL; }
    virtual u32 getTexMtxOffset() const { return 0; }
    virtual void setTexMtxOffset(u32) {}
    virtual ~J3DTexGenBlock() {}
};

class J3DTexGenBlockPatched : public J3DTexGenBlock {
public:
    J3DTexGenBlockPatched() NO_INLINE { initialize(); }
    void initialize();

    virtual void reset(J3DTexGenBlock*);
    virtual void calc(f32 const (*)[4]);
    virtual void calcWithoutViewMtx(f32 const (*)[4]);
    virtual void calcPostTexMtx(f32 const (*)[4]);
    virtual void calcPostTexMtxWithoutViewMtx(f32 const (*)[4]);
    virtual void load() {}
    virtual void patch();
    virtual void diff(u32);
    virtual void diffTexMtx();
    virtual void diffTexGen();
    virtual s32 countDLSize();
    virtual u32 getType() { return 'TGPT'; }
    virtual void setTexGenNum(u32 const* num) { mTexGenNum = *num; }
    virtual void setTexGenNum(u32 num) { mTexGenNum = num; }
    virtual u32 getTexGenNum() const { return mTexGenNum; }
    virtual void setTexCoord(u32 idx, J3DTexCoord const* coord) { mTexCoord[idx] = *coord; }
    virtual J3DTexCoord* getTexCoord(u32 idx) { return &mTexCoord[idx]; }
    virtual void setTexMtx(u32 idx, J3DTexMtx* mtx) { mTexMtx[idx] = mtx; }
    virtual J3DTexMtx* getTexMtx(u32 idx) { return mTexMtx[idx]; }
    virtual u32 getTexMtxOffset() const { return mTexMtxOffset; }
    virtual void setTexMtxOffset(u32 offset) { mTexMtxOffset = offset; }
    virtual ~J3DTexGenBlockPatched() {}

    /* 0x04 */ u32 mTexGenNum;
    /* 0x08 */ J3DTexCoord mTexCoord[8];
    /* 0x38 */ J3DTexMtx* mTexMtx[8];
    /* 0x58 */ u32 mTexMtxOffset;
};  // Size: 0x5C

class J3DTexGenBlock4 : public J3DTexGenBlockPatched {
public:
    J3DTexGenBlock4() : mNBTScale() { initialize(); }
    void initialize();

    virtual void reset(J3DTexGenBlock*);
    virtual void load();
    virtual void patch();
    virtual s32 countDLSize();
    virtual u32 getType() { return 'TGB4'; }
    virtual void setNBTScale(J3DNBTScale const* scale) { mNBTScale = *scale; }
    virtual void setNBTScale(J3DNBTScale scale) { mNBTScale = scale; }
    virtual J3DNBTScale* getNBTScale() { return &mNBTScale; }
    virtual ~J3DTexGenBlock4() {}

    /* 0x5C */ J3DNBTScale mNBTScale;
};  // Size: 0x6C

class J3DTexGenBlockBasic : public J3DTexGenBlockPatched {
public:
    J3DTexGenBlockBasic() : mNBTScale() { initialize(); }
    void initialize();

    virtual void reset(J3DTexGenBlock*);
    virtual void load();
    virtual void patch();
    virtual s32 countDLSize();
    virtual u32 getType() { return 'TGBC'; }
    virtual void setNBTScale(J3DNBTScale const* scale) { mNBTScale = *scale; }
    virtual void setNBTScale(J3DNBTScale scale) { mNBTScale = scale; }
    virtual J3DNBTScale* getNBTScale() { return &mNBTScale; }
    virtual ~J3DTexGenBlockBasic() {}

    /* 0x5C */ J3DNBTScale mNBTScale;
};  // Size: 0x6C

class J3DTevBlock {
public:
    virtual void reset(J3DTevBlock*) {}
    virtual void load() {}
    virtual void diff(u32);
    virtual void diffTexNo() {}
    virtual void diffTevReg() {}
    virtual void diffTexCoordScale() {}
    virtual void diffTevStage() {}
    virtual void diffTevStageIndirect() {}
    virtual void patch() {}
    virtual void patchTexNo() {}
    virtual void patchTevReg() {}
    virtual void patchTexNoAndTexCoordScale() {}
    virtual void ptrToIndex() = 0;
    virtual void indexToPtr() = 0;
    virtual u32 getType() = 0;
    virtual s32 countDLSize() { return 0; }
    virtual void setTexNo(u32, u16 const*) {}
    virtual void setTexNo(u32, u16) {}
    virtual u16 getTexNo(u32) const { return 0xffff; }
    virtual void setTevOrder(u32, J3DTevOrder const*) {}
    virtual void setTevOrder(u32, J3DTevOrder) {}
    virtual J3DTevOrder* getTevOrder(u32) { return NULL; }
    virtual void setTevColor(u32, J3DGXColorS10 const*) {}
    virtual void setTevColor(u32, J3DGXColorS10) {}
    virtual J3DGXColorS10* getTevColor(u32) { return NULL; }
    virtual void setTevKColor(u32, J3DGXColor const*) {}
    virtual void setTevKColor(u32, J3DGXColor) {}
    virtual J3DGXColor* getTevKColor(u32) { return NULL; }
    virtual void setTevKColorSel(u32, u8 const*) {}
    virtual void setTevKColorSel(u32, u8) {}
    virtual u8 getTevKColorSel(u32) { return false; }
    virtual void setTevKAlphaSel(u32, u8 const*) {}
    virtual void setTevKAlphaSel(u32, u8) {}
    virtual u8 getTevKAlphaSel(u32) { return false; }
    virtual void setTevStageNum(u8 const*) {}
    virtual void setTevStageNum(u8) {}
    virtual u8 getTevStageNum() const { return 1; }
    virtual void setTevStage(u32, J3DTevStage const*) {}
    virtual void setTevStage(u32, J3DTevStage) {}
    virtual J3DTevStage* getTevStage(u32) { return NULL; }
    virtual void setTevSwapModeInfo(u32, J3DTevSwapModeInfo const*) {}
    virtual void setTevSwapModeInfo(u32, J3DTevSwapModeInfo) {}
    virtual void setTevSwapModeTable(u32, J3DTevSwapModeTable const*) {}
    virtual void setTevSwapModeTable(u32, J3DTevSwapModeTable) {}
    virtual J3DTevSwapModeTable* getTevSwapModeTable(u32) { return NULL; }
    virtual void setIndTevStage(u32, J3DIndTevStage const*) {}
    virtual void setIndTevStage(u32, J3DIndTevStage) {}
    virtual J3DIndTevStage* getIndTevStage(u32) { return NULL; }
    virtual u32 getTexNoOffset() const { return 0; }
    virtual u32 getTevRegOffset() const { return 0; }
    virtual void setTexNoOffset(u32 offset) { mTexNoOffset = offset; }
    virtual void setTevRegOffset(u32) {}
    virtual ~J3DTevBlock() {}

protected:
    void indexToPtr_private(u32);

    /* 0x4 */ u32 mTexNoOffset;
};

class J3DTevBlock1 : public J3DTevBlock {
public:
    J3DTevBlock1() { initialize(); }
    void initialize();

    virtual void reset(J3DTevBlock*);
    virtual void load();
    virtual void diffTexNo();
    virtual void diffTevReg();
    virtual void diffTexCoordScale();
    virtual void diffTevStage();
    virtual void diffTevStageIndirect();
    virtual void patch();
    virtual void patchTexNo();
    virtual void patchTevReg();
    virtual void patchTexNoAndTexCoordScale();
    virtual void ptrToIndex() {}
    virtual void indexToPtr() { indexToPtr_private(mTexNoOffset); }
    virtual u32 getType() { return 'TVB1'; }
    virtual s32 countDLSize();
    virtual void setTexNo(u32 idx, u16 const* no) { mTexNo[idx] = *no; }
    virtual void setTexNo(u32 idx, u16 no) { mTexNo[idx] = no; }
    virtual u16 getTexNo(u32 idx) const { return mTexNo[idx]; }
    virtual void setTevOrder(u32 idx, J3DTevOrder const* order) { mTevOrder[idx] = *order; }
    virtual void setTevOrder(u32 idx, J3DTevOrder order) { mTevOrder[idx] = order; }
    virtual J3DTevOrder* getTevOrder(u32 idx) { return &mTevOrder[idx]; }
    virtual void setTevStageNum(u8 const* num) {}
    virtual void setTevStageNum(u8 num) {}
    virtual u8 getTevStageNum() const { return 1; }
    virtual void setTevStage(u32 idx, J3DTevStage const* stage) { mTevStage[idx] = *stage; }
    virtual void setTevStage(u32 idx, J3DTevStage stage) { mTevStage[idx] = stage; }
    virtual J3DTevStage* getTevStage(u32 idx) { return &mTevStage[idx]; }
    virtual void setIndTevStage(u32 idx, J3DIndTevStage const* stage) { mIndTevStage[idx] = *stage; }
    virtual void setIndTevStage(u32 idx, J3DIndTevStage stage) { mIndTevStage[idx] = stage; }
    virtual J3DIndTevStage* getIndTevStage(u32 idx) { return &mIndTevStage[idx]; }
    virtual u32 getTexNoOffset() const { return mTexNoOffset; }
    virtual ~J3DTevBlock1() {}

    /* 0x08 */ u16 mTexNo[1];
    /* 0x0A */ J3DTevOrder mTevOrder[1];
    /* 0x0E */ J3DTevStage mTevStage[1];
    /* 0x18 */ J3DIndTevStage mIndTevStage[1];
};  // Size: 0x1C

class J3DTevBlock2 : public J3DTevBlock {
public:
    J3DTevBlock2() { initialize(); }
    void initialize();

    virtual void reset(J3DTevBlock*);
    virtual void load();
    virtual void diffTexNo();
    virtual void diffTevReg();
    virtual void diffTexCoordScale();
    virtual void diffTevStage();
    virtual void diffTevStageIndirect();
    virtual void patch();
    virtual void patchTexNo();
    virtual void patchTevReg();
    virtual void patchTexNoAndTexCoordScale();
    virtual void ptrToIndex() {}
    virtual void indexToPtr() { indexToPtr_private(mTexNoOffset); }
    virtual u32 getType() { return 'TVB2'; }
    virtual s32 countDLSize();
    virtual void setTexNo(u32 idx, u16 const* texNo) { mTexNo[idx] = *texNo; }
    virtual void setTexNo(u32 idx, u16 texNo) { mTexNo[idx] = texNo; }
    virtual u16 getTexNo(u32 idx) const { return mTexNo[idx]; }
    virtual void setTevOrder(u32 idx, J3DTevOrder const* order) { mTevOrder[idx] = *order; }
    virtual void setTevOrder(u32 idx, J3DTevOrder order) { mTevOrder[idx] = order; }
    virtual J3DTevOrder* getTevOrder(u32 idx) { return &mTevOrder[idx]; }
    virtual void setTevColor(u32 idx, J3DGXColorS10 const* color) { mTevColor[idx] = *color; }
    virtual void setTevColor(u32 idx, J3DGXColorS10 color) { mTevColor[idx] = color; }
    virtual J3DGXColorS10* getTevColor(u32 idx) { return &mTevColor[idx]; }
    virtual void setTevKColor(u32 idx, J3DGXColor const* color) { mTevKColor[idx] = *color; }
    virtual void setTevKColor(u32 idx, J3DGXColor color) { mTevKColor[idx] = color; }
    virtual J3DGXColor* getTevKColor(u32 idx) { return &mTevKColor[idx]; }
    virtual void setTevKColorSel(u32 idx, u8 const* sel) { mTevKColorSel[idx] = *sel; }
    virtual void setTevKColorSel(u32 idx, u8 sel) { mTevKColorSel[idx] = sel; }
    virtual u8 getTevKColorSel(u32 idx) { return mTevKColorSel[idx]; }
    virtual void setTevKAlphaSel(u32 idx, u8 const* sel) { mTevKAlphaSel[idx] = *sel; }
    virtual void setTevKAlphaSel(u32 idx, u8 sel) { mTevKAlphaSel[idx] = sel; }
    virtual u8 getTevKAlphaSel(u32 idx) { return mTevKAlphaSel[idx]; }
    virtual void setTevStageNum(u8 const* num) { mTevStageNum = *num; }
    virtual void setTevStageNum(u8 num) { mTevStageNum = num; }
    virtual u8 getTevStageNum() const { return mTevStageNum; }
    virtual void setTevStage(u32 idx, J3DTevStage const* stage) { mTevStage[idx] = *stage; }
    virtual void setTevStage(u32 idx, J3DTevStage stage) { mTevStage[idx] = stage; }
    virtual J3DTevStage* getTevStage(u32 idx) { return &mTevStage[idx]; }
    virtual void setTevSwapModeInfo(u32 idx, J3DTevSwapModeInfo const* info) { mTevStage[idx].setTevSwapModeInfo(*info); }
    virtual void setTevSwapModeInfo(u32 idx, J3DTevSwapModeInfo info) { mTevStage[idx].setTevSwapModeInfo(info); }
    virtual void setTevSwapModeTable(u32 idx, J3DTevSwapModeTable const* table) { mTevSwapModeTable[idx] = *table; }
    virtual void setTevSwapModeTable(u32 idx, J3DTevSwapModeTable table) { mTevSwapModeTable[idx] = table; }
    virtual J3DTevSwapModeTable* getTevSwapModeTable(u32 idx) { return &mTevSwapModeTable[idx]; }
    virtual void setIndTevStage(u32 idx, J3DIndTevStage const* stage) { mIndTevStage[idx] = *stage; }
    virtual void setIndTevStage(u32 idx, J3DIndTevStage stage) { mIndTevStage[idx] = stage; }
    virtual J3DIndTevStage* getIndTevStage(u32 idx) { return &mIndTevStage[idx]; }
    virtual u32 getTexNoOffset() const { return mTexNoOffset; }
    virtual u32 getTevRegOffset() const { return mTevRegOffset; }
    virtual void setTevRegOffset(u32 offset) { mTevRegOffset = offset; }
    virtual ~J3DTevBlock2() {}

    /* 0x08 */ u16 mTexNo[2];
    /* 0x0C */ J3DTevOrder mTevOrder[2];
    /* 0x14 */ J3DGXColorS10 mTevColor[4];
    /* 0x34 */ u8 mTevStageNum;
    /* 0x35 */ J3DTevStage mTevStage[2];
    /* 0x45 */ J3DGXColor mTevKColor[4];
    /* 0x55 */ u8 mTevKColorSel[2];
    /* 0x57 */ u8 mTevKAlphaSel[2];
    /* 0x59 */ J3DTevSwapModeTable mTevSwapModeTable[4];
    /* 0x60 */ J3DIndTevStage mIndTevStage[2];
    /* 0x68 */ u32 mTevRegOffset;
};  // Size: 0x6C

/**
 * @ingroup jsystem-j3d
 *
 */
class J3DTevBlock4 : public J3DTevBlock {
public:
    J3DTevBlock4() { initialize(); }
    void initialize();

    virtual void reset(J3DTevBlock*);
    virtual void load();
    virtual void diffTexNo();
    virtual void diffTevReg();
    virtual void diffTexCoordScale();
    virtual void diffTevStage();
    virtual void diffTevStageIndirect();
    virtual void patch();
    virtual void patchTexNo();
    virtual void patchTevReg();
    virtual void patchTexNoAndTexCoordScale();
    virtual void ptrToIndex() {}
    virtual void indexToPtr() { indexToPtr_private(mTexNoOffset); }
    virtual u32 getType() { return 'TVB4'; }
    virtual s32 countDLSize();
    virtual void setTexNo(u32 idx, u16 const* texNo) { mTexNo[idx] = *texNo; }
    virtual void setTexNo(u32 idx, u16 texNo) { mTexNo[idx] = texNo; }
    virtual u16 getTexNo(u32 idx) const { return mTexNo[idx]; }
    virtual void setTevOrder(u32 idx, J3DTevOrder const* order) { mTevOrder[idx] = *order; }
    virtual void setTevOrder(u32 idx, J3DTevOrder order) { mTevOrder[idx] = order; }
    virtual J3DTevOrder* getTevOrder(u32 idx) { return &mTevOrder[idx]; }
    virtual void setTevColor(u32 idx, J3DGXColorS10 const* color) { mTevColor[idx] = *color; }
    virtual void setTevColor(u32 idx, J3DGXColorS10 color) { mTevColor[idx] = color; }
    virtual J3DGXColorS10* getTevColor(u32 idx) { return &mTevColor[idx]; }
    virtual void setTevKColor(u32 idx, J3DGXColor const* color) { mTevKColor[idx] = *color; }
    virtual void setTevKColor(u32 idx, J3DGXColor color) { mTevKColor[idx] = color; }
    virtual J3DGXColor* getTevKColor(u32 idx) { return &mTevKColor[idx]; }
    virtual void setTevKColorSel(u32 idx, u8 const* sel) { mTevKColorSel[idx] = *sel; }
    virtual void setTevKColorSel(u32 idx, u8 sel) { mTevKColorSel[idx] = sel; }
    virtual u8 getTevKColorSel(u32 idx) { return mTevKColorSel[idx]; }
    virtual void setTevKAlphaSel(u32 idx, u8 const* sel) { mTevKAlphaSel[idx] = *sel; }
    virtual void setTevKAlphaSel(u32 idx, u8 sel) { mTevKAlphaSel[idx] = sel; }
    virtual u8 getTevKAlphaSel(u32 idx) { return mTevKAlphaSel[idx]; }
    virtual void setTevStageNum(u8 const* num) { mTevStageNum = *num; }
    virtual void setTevStageNum(u8 num) { mTevStageNum = num; }
    virtual u8 getTevStageNum() const { return mTevStageNum; }
    virtual void setTevStage(u32 idx, J3DTevStage const* stage) { mTevStage[idx] = *stage; }
    virtual void setTevStage(u32 idx, J3DTevStage stage) { mTevStage[idx] = stage; }
    virtual J3DTevStage* getTevStage(u32 idx) { return &mTevStage[idx]; }
    virtual void setTevSwapModeInfo(u32 idx, J3DTevSwapModeInfo const* info) { mTevStage[idx].setTevSwapModeInfo(*info); }
    virtual void setTevSwapModeInfo(u32 idx, J3DTevSwapModeInfo info) { mTevStage[idx].setTevSwapModeInfo(info); }
    virtual void setTevSwapModeTable(u32 idx, J3DTevSwapModeTable const* table) { mTevSwapModeTable[idx] = *table; }
    virtual void setTevSwapModeTable(u32 idx, J3DTevSwapModeTable table) { mTevSwapModeTable[idx] = table; }
    virtual J3DTevSwapModeTable* getTevSwapModeTable(u32 idx) { return &mTevSwapModeTable[idx]; }
    virtual void setIndTevStage(u32 idx, J3DIndTevStage const* stage) { mIndTevStage[idx] = *stage; }
    virtual void setIndTevStage(u32 idx, J3DIndTevStage stage) { mIndTevStage[idx] = stage; }
    virtual J3DIndTevStage* getIndTevStage(u32 idx) { return &mIndTevStage[idx]; }
    virtual u32 getTexNoOffset() const { return mTexNoOffset; }
    virtual u32 getTevRegOffset() const { return mTevRegOffset; }
    virtual void setTevRegOffset(u32 offset) { mTevRegOffset = offset; }
    virtual ~J3DTevBlock4() {}

    /* 0x08 */ u16 mTexNo[4];
    /* 0x10 */ J3DTevOrder mTevOrder[4];
    /* 0x20 */ u8 mTevStageNum;
    /* 0x21 */ J3DTevStage mTevStage[4];
    /* 0x42 */ J3DGXColorS10 mTevColor[4];
    /* 0x62 */ J3DGXColor mTevKColor[4];
    /* 0x72 */ u8 mTevKColorSel[4];
    /* 0x76 */ u8 mTevKAlphaSel[4];
    /* 0x7A */ J3DTevSwapModeTable mTevSwapModeTable[4];
    /* 0x80 */ J3DIndTevStage mIndTevStage[4];
    /* 0x90 */ u32 mTevRegOffset;
};  // Size: 0x94

class J3DTevBlock16 : public J3DTevBlock {
public:
    J3DTevBlock16() { initialize(); }
    void initialize();

    virtual void reset(J3DTevBlock*);
    virtual void load();
    virtual void diffTexNo();
    virtual void diffTevReg();
    virtual void diffTexCoordScale();
    virtual void diffTevStage();
    virtual void diffTevStageIndirect();
    virtual void patch();
    virtual void patchTexNo();
    virtual void patchTevReg();
    virtual void patchTexNoAndTexCoordScale();
    virtual void ptrToIndex();
    virtual void indexToPtr() { indexToPtr_private(mTexNoOffset); }
    virtual u32 getType() { return 'TV16'; }
    virtual s32 countDLSize();
    virtual void setTexNo(u32 idx, u16 const* texNo) { mTexNo[idx] = *texNo; }
    virtual void setTexNo(u32 idx, u16 texNo) { mTexNo[idx] = texNo; }
    virtual u16 getTexNo(u32 idx) const { return mTexNo[idx]; }
    virtual void setTevOrder(u32 idx, J3DTevOrder const* order) { mTevOrder[idx] = *order; }
    virtual void setTevOrder(u32 idx, J3DTevOrder order) { mTevOrder[idx] = order; }
    virtual J3DTevOrder* getTevOrder(u32 idx) { return &mTevOrder[idx]; }
    virtual void setTevColor(u32 idx, J3DGXColorS10 const* color) { mTevColor[idx] = *color; }
    virtual void setTevColor(u32 idx, J3DGXColorS10 color) { mTevColor[idx] = color; }
    virtual J3DGXColorS10* getTevColor(u32 idx) { return &mTevColor[idx]; }
    virtual void setTevKColor(u32 idx, J3DGXColor const* color) { mTevKColor[idx] = *color; }
    virtual void setTevKColor(u32 idx, J3DGXColor color) { mTevKColor[idx] = color; }
    virtual J3DGXColor* getTevKColor(u32 idx) { return &mTevKColor[idx]; }
    virtual void setTevKColorSel(u32 idx, u8 const* sel) { mTevKColorSel[idx] = *sel; }
    virtual void setTevKColorSel(u32 idx, u8 sel) { mTevKColorSel[idx] = sel; }
    virtual u8 getTevKColorSel(u32 idx) { return mTevKColorSel[idx]; }
    virtual void setTevKAlphaSel(u32 idx, u8 const* sel) { mTevKAlphaSel[idx] = *sel; }
    virtual void setTevKAlphaSel(u32 idx, u8 sel) { mTevKAlphaSel[idx] = sel; }
    virtual u8 getTevKAlphaSel(u32 idx) { return mTevKAlphaSel[idx]; }
    virtual void setTevStageNum(u8 const* num) { mTevStageNum = *num; }
    virtual void setTevStageNum(u8 num) { mTevStageNum = num; }
    virtual u8 getTevStageNum() const { return mTevStageNum; }
    virtual void setTevStage(u32 idx, J3DTevStage const* stage) { mTevStage[idx] = *stage; }
    virtual void setTevStage(u32 idx, J3DTevStage stage) { mTevStage[idx] = stage; }
    virtual J3DTevStage* getTevStage(u32 idx) { return &mTevStage[idx]; }
    virtual void setTevSwapModeInfo(u32 idx, J3DTevSwapModeInfo const* info) { mTevStage[idx].setTevSwapModeInfo(*info); }
    virtual void setTevSwapModeInfo(u32 idx, J3DTevSwapModeInfo info) { mTevStage[idx].setTevSwapModeInfo(info); }
    virtual void setTevSwapModeTable(u32 idx, J3DTevSwapModeTable const* table) { mTevSwapModeTable[idx] = *table; }
    virtual void setTevSwapModeTable(u32 idx, J3DTevSwapModeTable table) { mTevSwapModeTable[idx] = table; }
    virtual J3DTevSwapModeTable* getTevSwapModeTable(u32 idx) { return &mTevSwapModeTable[idx]; }
    virtual void setIndTevStage(u32 idx, J3DIndTevStage const* stage) { mIndTevStage[idx] = *stage; }
    virtual void setIndTevStage(u32 idx, J3DIndTevStage stage) { mIndTevStage[idx] = stage; }
    virtual J3DIndTevStage* getIndTevStage(u32 idx) { return &mIndTevStage[idx]; }
    virtual u32 getTexNoOffset() const { return mTexNoOffset; }
    virtual u32 getTevRegOffset() const { return mTevRegOffset; }
    virtual void setTevRegOffset(u32 offset) { mTevRegOffset = offset; }
    virtual ~J3DTevBlock16() {}

    /* 0x008 */ u16 mTexNo[8];
    /* 0x018 */ J3DTevOrder mTevOrder[16];
    /* 0x058 */ u8 mTevStageNum;
    /* 0x059 */ J3DTevStage mTevStage[16];
    /* 0x0DA */ J3DGXColorS10 mTevColor[4];
    /* 0x0FA */ J3DGXColor mTevKColor[4];
    /* 0x10A */ u8 mTevKColorSel[16];
    /* 0x11A */ u8 mTevKAlphaSel[16];
    /* 0x12A */ J3DTevSwapModeTable mTevSwapModeTable[4];
    /* 0x130 */ J3DIndTevStage mIndTevStage[16];
    /* 0x170 */ u32 mTevRegOffset;
};  // Size: 0x174

struct J3DIndTexMtx : public J3DIndTexMtxInfo {
    J3DIndTexMtx() { *(J3DIndTexMtxInfo*)this = j3dDefaultIndTexMtxInfo; }
    J3DIndTexMtx(const J3DIndTexMtxInfo& info) { *(J3DIndTexMtxInfo*)this = info; }
    J3DIndTexMtx(const J3DIndTexMtx& other) { __memcpy(this, &other, sizeof(J3DIndTexMtx)); }
    ~J3DIndTexMtx() {}
    void load(u32 param_1) const { J3DGDSetIndTexMtx((GXIndTexMtxID)(param_1 + GX_ITM_0), (Mtx3P)field_0x0, field_0x18); }
};  // Size: 0x1C

class J3DIndBlock {
public:
    virtual void reset(J3DIndBlock*) {}
    virtual void diff(u32) = 0;
    virtual void load() = 0;
    virtual s32 countDLSize() { return 0; }
    virtual u32 getType() = 0;
    virtual void setIndTexStageNum(u8) {}
    virtual u8 getIndTexStageNum() const { return 0; }
    virtual void setIndTexOrder(u32, J3DIndTexOrder) {}
    virtual void setIndTexOrder(u32, J3DIndTexOrder const*) {}
    virtual J3DIndTexOrder* getIndTexOrder(u32) { return NULL; }
    virtual void setIndTexMtx(u32, J3DIndTexMtx const*) {}
    virtual void setIndTexMtx(u32, J3DIndTexMtx) {}
    virtual J3DIndTexMtx* getIndTexMtx(u32) { return NULL; }
    virtual void setIndTexCoordScale(u32, J3DIndTexCoordScale const*) {}
    virtual void setIndTexCoordScale(u32, J3DIndTexCoordScale) {}
    virtual J3DIndTexCoordScale* getIndTexCoordScale(u32) { return NULL; }
    virtual ~J3DIndBlock() {}
};

class J3DIndBlockFull : public J3DIndBlock {
public:
    J3DIndBlockFull() { initialize(); }
    void initialize();

    virtual s32 countDLSize();
    virtual void reset(J3DIndBlock*);
    virtual void diff(u32);
    virtual void load();
    virtual u32 getType() { return 'IBLF'; }
    virtual void setIndTexStageNum(u8 num) { mIndTexStageNum = num; }
    virtual u8 getIndTexStageNum() const { return mIndTexStageNum; }
    virtual void setIndTexOrder(u32 idx, J3DIndTexOrder const* order) { mIndTexOrder[idx] = *order; }
    virtual void setIndTexOrder(u32 idx, J3DIndTexOrder order) { mIndTexOrder[idx] = order; }
    virtual J3DIndTexOrder* getIndTexOrder(u32 idx) { return &mIndTexOrder[idx]; }
    virtual void setIndTexMtx(u32 idx, J3DIndTexMtx const* mtx) { mIndTexMtx[idx] = *mtx; }
    virtual void setIndTexMtx(u32 idx, J3DIndTexMtx mtx) { mIndTexMtx[idx] = mtx; }
    virtual J3DIndTexMtx* getIndTexMtx(u32 idx) { return &mIndTexMtx[idx]; }
    virtual void setIndTexCoordScale(u32 idx, J3DIndTexCoordScale const* scale) { mIndTexCoordScale[idx] = *scale; }
    virtual void setIndTexCoordScale(u32 idx, J3DIndTexCoordScale scale) { mIndTexCoordScale[idx] = scale; }
    virtual J3DIndTexCoordScale* getIndTexCoordScale(u32 idx) { return &mIndTexCoordScale[idx]; }
    virtual ~J3DIndBlockFull() {}

    /* 0x04 */ u8 mIndTexStageNum;
    /* 0x05 */ J3DIndTexOrder mIndTexOrder[4];
    /* 0x18 */ J3DIndTexMtx mIndTexMtx[3];
    /* 0x6C */ J3DIndTexCoordScale mIndTexCoordScale[4];
};  // Size: 0x7C

class J3DIndBlockNull : public J3DIndBlock {
public:
    virtual void diff(u32) {}
    virtual void load() {}
    virtual void reset(J3DIndBlock*) {}
    virtual u32 getType() { return 'IBLN'; }
    virtual ~J3DIndBlockNull() {}
};

class J3DPEBlock {
public:
    virtual void reset(J3DPEBlock*) {}
    virtual void load() = 0;
    virtual void patch() {}
    virtual void diff(u32) {}
    virtual void diffFog() {}
    virtual void diffBlend() {}
    virtual s32 countDLSize() { return 0; }
    virtual u32 getType() = 0;
    virtual void setFog(J3DFog) {}
    virtual void setFog(J3DFog*) {}
    virtual J3DFog* getFog() { return NULL; }
    virtual void setAlphaComp(J3DAlphaComp const*) {}
    virtual void setAlphaComp(J3DAlphaComp const&) {}
    virtual J3DAlphaComp* getAlphaComp() { return NULL; }
    virtual void setBlend(J3DBlend const*) {}
    virtual void setBlend(J3DBlend const&) {}
    virtual J3DBlend* getBlend() { return NULL; }
    virtual void setZMode(J3DZMode const*) {}
    virtual void setZMode(J3DZMode) {}
    virtual J3DZMode* getZMode() { return NULL; }
    virtual void setZCompLoc(u8 const*) {}
    virtual void setZCompLoc(u8) {}
    virtual u8 getZCompLoc() const { return 0; }
    virtual void setDither(u8 const*) {}
    virtual void setDither(u8) {}
    virtual u8 getDither() const { return 0; }
    virtual u32 getFogOffset() const { return 0; }
    virtual void setFogOffset(u32) {}
    virtual ~J3DPEBlock() {}
};

struct J3DPEBlockNull : public J3DPEBlock {
    virtual void load() {}
    virtual u32 getType() { return 'PENL'; }
    virtual ~J3DPEBlockNull() {}
};

class J3DPEBlockOpa : public J3DPEBlock {
public:
    virtual s32 countDLSize();
    virtual void load();
    virtual u32 getType() { return 'PEOP'; }
    virtual ~J3DPEBlockOpa() {}
};

class J3DPEBlockTexEdge : public J3DPEBlock {
public:
    virtual s32 countDLSize();
    virtual void load();
    virtual u32 getType() { return 'PEED'; }
    virtual ~J3DPEBlockTexEdge() {}
};

class J3DPEBlockXlu : public J3DPEBlock {
public:
    virtual s32 countDLSize();
    virtual void load();
    virtual u32 getType() { return 'PEXL'; }
    virtual ~J3DPEBlockXlu() {}
};

class J3DPEBlockFogOff : public J3DPEBlock {
public:
    J3DPEBlockFogOff() { initialize(); }
    void initialize();

    virtual void reset(J3DPEBlock*);
    virtual void load();
    virtual void diff(u32 param_0) {
        if (param_0 & 0x20000000)
            diffBlend();
    };
    virtual void diffBlend();
    virtual s32 countDLSize();
    virtual u32 getType() { return 'PEFG'; }
    virtual void setAlphaComp(J3DAlphaComp const* alphaComp) { mAlphaComp = *alphaComp; }
    virtual void setAlphaComp(J3DAlphaComp const& alphaComp) { mAlphaComp = alphaComp; }
    virtual J3DAlphaComp* getAlphaComp() { return &mAlphaComp; }
    virtual void setBlend(J3DBlend const* blend) { mBlend = *blend; }
    virtual void setBlend(J3DBlend const& blend) { mBlend = blend; }
    virtual J3DBlend* getBlend() { return &mBlend; }
    virtual void setZMode(J3DZMode const* zMode) { mZMode = *zMode; }
    virtual void setZMode(J3DZMode zMode) { mZMode = zMode; }
    virtual J3DZMode* getZMode() { return &mZMode; }
    virtual void setZCompLoc(u8 const* zCompLoc) { mZCompLoc = *zCompLoc; }
    virtual void setZCompLoc(u8 zCompLoc) { mZCompLoc = zCompLoc; }
    virtual u8 getZCompLoc() const { return mZCompLoc; }
    virtual void setDither(u8 const* dither) { mDither = *dither; }
    virtual void setDither(u8 dither) { mDither = dither; }
    virtual u8 getDither() const { return mDither; }
    virtual ~J3DPEBlockFogOff() {}

    /* 0x04 */ J3DAlphaComp mAlphaComp;
    /* 0x08 */ J3DBlend mBlend;
    /* 0x0C */ J3DZMode mZMode;
    /* 0x0E */ u8 mZCompLoc;
    /* 0x0F */ u8 mDither;
};  // Size: 0x10

class J3DPEBlockFull : public J3DPEBlock {
public:
    J3DPEBlockFull() { initialize(); }
    void initialize();

    virtual void reset(J3DPEBlock*);
    virtual void load();
    virtual void patch();
    virtual void diff(u32);
    virtual void diffFog();
    virtual void diffBlend();
    virtual s32 countDLSize();
    virtual u32 getType() { return 'PEFL'; }
    virtual void setFog(J3DFog fog) { mFog.setFogInfo(fog.getFogInfo()); }
    virtual void setFog(J3DFog* pFog) { mFog.setFogInfo(pFog->getFogInfo()); }
    virtual J3DFog* getFog() { return &mFog; }
    virtual void setAlphaComp(J3DAlphaComp const* alphaComp) { mAlphaComp = *alphaComp; }
    virtual void setAlphaComp(J3DAlphaComp const& alphaComp) { mAlphaComp = alphaComp; }
    virtual J3DAlphaComp* getAlphaComp() { return &mAlphaComp; }
    virtual void setBlend(J3DBlend const* blend) { mBlend = *blend; }
    virtual void setBlend(J3DBlend const& blend) { mBlend = blend; }
    virtual J3DBlend* getBlend() { return &mBlend; }
    virtual void setZMode(J3DZMode const* zMode) { mZMode = *zMode; }
    virtual void setZMode(J3DZMode zMode) { mZMode = zMode; }
    virtual J3DZMode* getZMode() { return &mZMode; }
    virtual void setZCompLoc(u8 const* zCompLoc) { mZCompLoc = *zCompLoc; }
    virtual void setZCompLoc(u8 zCompLoc) { mZCompLoc = zCompLoc; }
    virtual u8 getZCompLoc() const { return mZCompLoc; }
    virtual void setDither(u8 const* dither) { mDither = *dither; }
    virtual void setDither(u8 dither) { mDither = dither; }
    virtual u8 getDither() const { return mDither; }
    virtual u32 getFogOffset() const { return mFogOffset; }
    virtual void setFogOffset(u32 fogOffset) { mFogOffset = fogOffset; }
    virtual ~J3DPEBlockFull() {}

    /* 0x04 */ J3DFog mFog;
    /* 0x30 */ J3DAlphaComp mAlphaComp;
    /* 0x34 */ J3DBlend mBlend;
    /* 0x38 */ J3DZMode mZMode;
    /* 0x3A */ u8 mZCompLoc;
    /* 0x3B */ u8 mDither;
    /* 0x3C */ u32 mFogOffset;
};  // Size: 0x40
