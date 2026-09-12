#pragma once

#include "Inline.hpp"

#include "JSystem/J3DGraphBase/J3DGD.hpp"
#include "JSystem/J3DGraphBase/J3DStruct.hpp"
#include <revolution/gx.h>
#include "JSystem/J3DGraphBase/J3DSys.hpp"

extern const J3DLightInfo j3dDefaultLightInfo;
extern const J3DTexCoordInfo j3dDefaultTexCoordInfo[8];

extern const J3DTexMtxInfo j3dDefaultTexMtxInfo;
extern const J3DIndTexMtxInfo j3dDefaultIndTexMtxInfo;

extern const J3DTevStageInfo j3dDefaultTevStageInfo;
extern const J3DIndTevStageInfo j3dDefaultIndTevStageInfo;
extern const J3DFogInfo j3dDefaultFogInfo;
extern const J3DNBTScaleInfo j3dDefaultNBTScaleInfo;

extern const GXColor j3dDefaultColInfo;
extern const GXColor j3dDefaultAmbInfo;
extern const GXColorS10 j3dDefaultTevColor;
extern const GXColor j3dDefaultTevKColor;
extern u8 j3dAlphaCmpTable[768];
extern const u8 j3dDefaultNumChans;

extern const J3DIndTexCoordScaleInfo j3dDefaultIndTexCoordScaleInfo;

extern const J3DIndTexOrderInfo j3dDefaultIndTexOrderNull;

extern const J3DTevSwapModeInfo j3dDefaultTevSwapMode;
extern const J3DTevSwapModeTableInfo j3dDefaultTevSwapModeTable;
extern const J3DBlendInfo j3dDefaultBlendInfo;
extern const J3DColorChanInfo j3dDefaultColorChanInfo;

extern const u8 j3dDefaultTevSwapTableID;
extern const u16 j3dDefaultAlphaCmpID;
extern const u16 j3dDefaultZModeID;

class J3DLightObj {
public:
    J3DLightObj() {
        mInfo = j3dDefaultLightInfo;
    }
    void load(u32) const;

    J3DLightInfo* getLightInfo() {
        return &mInfo;
    }
    J3DLightObj& operator=(J3DLightObj const& other) {
        mInfo = other.mInfo;
        return *this;
    }

    /* 0x00 */ J3DLightInfo mInfo;
    /* 0x34 */ GXLightObj mLightObj;
};  // Size = 0x74

struct J3DTevStage {
    J3DTevStage() {
        setTevStageInfo(j3dDefaultTevStageInfo);
        setTevSwapModeInfo(j3dDefaultTevSwapMode);
    }

    void setTevColorOp(u8 param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5) {
        mTevColorOp = mTevColorOp & ~(0x01 << 2) | param_1 << 2;
        if (param_1 <= 1) {
            mTevColorOp = mTevColorOp & ~(0x03 << 4) | param_3 << 4;
            mTevColorOp = mTevColorOp & ~0x03 | param_2;
        } else {
            mTevColorOp = mTevColorOp & ~(0x03 << 4) | (param_1 >> 1 & 3) << 4;
            mTevColorOp = mTevColorOp & ~0x03 | 3;
        }
        mTevColorOp = mTevColorOp & ~(0x01 << 3) | param_4 << 3;
        mTevColorOp = mTevColorOp & ~(0x03 << 6) | param_5 << 6;
    }

    void setTevColorAB(u8 a, u8 b) {
        mTevColorAB = a << 4 | b;
    }
    void setTevColorCD(u8 c, u8 d) {
        mTevColorCD = c << 4 | d;
    }
    void setAlphaA(u8 a) {
        mTevAlphaAB = mTevAlphaAB & ~(0x07 << 5) | a << 5;
    }
    void setAlphaB(u8 b) {
        mTevAlphaAB = mTevAlphaAB & ~(0x07 << 2) | b << 2;
    }
    void setAlphaC(u8 c) {
        mTevAlphaAB = mTevAlphaAB & ~0x03 | c >> 1;
        mTevSwapModeInfo = mTevSwapModeInfo & ~(0x01 << 7) | c << 7;
    }
    void setAlphaD(u8 d) {
        mTevSwapModeInfo = mTevSwapModeInfo & ~(0x07 << 4) | d << 4;
    }
    void setAlphaABCD(u8 a, u8 b, u8 c, u8 d) {
        setAlphaA(a);
        setAlphaB(b);
        setAlphaC(c);
        setAlphaD(d);
    }

    void setTevAlphaOp(u8 param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5) {
        mTevAlphaOp = mTevAlphaOp & ~(0x01 << 2) | param_1 << 2;
        if (param_1 <= 1) {
            mTevAlphaOp = mTevAlphaOp & ~0x03 | param_2;
            mTevAlphaOp = mTevAlphaOp & ~(0x03 << 4) | param_3 << 4;
        } else {
            mTevAlphaOp = mTevAlphaOp & ~(0x03 << 4) | (param_1 >> 1 & 3) << 4;
            mTevAlphaOp = mTevAlphaOp & ~0x03 | 3;
        }
        mTevAlphaOp = mTevAlphaOp & ~(0x01 << 3) | param_4 << 3;
        mTevAlphaOp = mTevAlphaOp & ~(0x03 << 6) | param_5 << 6;
    }

    void setTevStageInfo(const J3DTevStageInfo& info) {
        setTevColorOp(info.mTevAlphaOp, info.mTevAlphaAB, info.mTevSwapModeInfo, info.field_0x8, info.field_0x9);
        setTevColorAB(info.mTevColorOp, info.mTevColorAB);
        setTevColorCD(info.mTevColorCD, info.field_0x4);
        setAlphaABCD(info.field_0xa, info.field_0xb, info.field_0xc, info.field_0xd);
        setTevAlphaOp(info.field_0xe, info.field_0xf, info.field_0x10, info.field_0x11, info.field_0x12);
    }

    J3DTevStage(J3DTevStageInfo const& param_0) {
        setTevStageInfo(param_0);
        setTevSwapModeInfo(j3dDefaultTevSwapMode);
    }

    void setTevSwapModeInfo(J3DTevSwapModeInfo const& param_0) {
        setTexSel(param_0.mTexSel);
        setRasSel(param_0.mRasSel);
    }

    void setStageNo(u32 param_0) {
        field_0x0 = 0xC0 + param_0 * 2;
        field_0x4 = 0xC1 + param_0 * 2;
    }

    void setRasSel(u8 ras_sel) {
        mTevSwapModeInfo = (mTevSwapModeInfo & ~3) | ras_sel;
    }
    void setTexSel(u8 tex_sel) {
        mTevSwapModeInfo = (mTevSwapModeInfo & ~0xc) | (tex_sel << 2);
    }

    void load(u32 param_1) const {
        J3DGDWriteBPCmd(*(u32*)&field_0x0);
        J3DGDWriteBPCmd(*(u32*)&field_0x4);
    }

    J3DTevStage& operator=(const J3DTevStage& other) {
        mTevColorOp = other.mTevColorOp;
        mTevColorAB = other.mTevColorAB;
        mTevColorCD = other.mTevColorCD;
        mTevAlphaOp = other.mTevAlphaOp;
        mTevAlphaAB = other.mTevAlphaAB;
        mTevSwapModeInfo = other.mTevSwapModeInfo;
        return *this;
    }

    J3DTevStage& operator=(J3DTevStage& other) {
        mTevColorOp = other.mTevColorOp;
        mTevColorAB = other.mTevColorAB;
        mTevColorCD = other.mTevColorCD;
        mTevAlphaOp = other.mTevAlphaOp;
        mTevAlphaAB = other.mTevAlphaAB;
        mTevSwapModeInfo = other.mTevSwapModeInfo;
        return *this;
    }

    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 mTevColorOp;
    /* 0x2 */ u8 mTevColorAB;
    /* 0x3 */ u8 mTevColorCD;
    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 mTevAlphaOp;
    /* 0x6 */ u8 mTevAlphaAB;
    /* 0x7 */ u8 mTevSwapModeInfo;
};

void makeTexCoordTable();
void makeAlphaCmpTable();
void makeZModeTable();
void makeTevSwapTable();

extern const J3DNBTScaleInfo j3dDefaultNBTScaleInfo;

struct J3DNBTScale : public J3DNBTScaleInfo {
    J3DNBTScale() {
        mbHasScale = j3dDefaultNBTScaleInfo.mbHasScale;
        mScale.x = j3dDefaultNBTScaleInfo.mScale.x;
        mScale.y = j3dDefaultNBTScaleInfo.mScale.y;
        mScale.z = j3dDefaultNBTScaleInfo.mScale.z;
    }

    J3DNBTScale(J3DNBTScaleInfo const& info) {
        mbHasScale = info.mbHasScale;
        mScale.x = info.mScale.x;
        mScale.y = info.mScale.y;
        mScale.z = info.mScale.z;
    }

    Vec* getScale() {
        return &mScale;
    }
};

extern const J3DTevOrderInfo j3dDefaultTevOrderInfoNull;

struct J3DTevOrder : public J3DTevOrderInfo {
    J3DTevOrder() {
        *reinterpret_cast< u32* >(this) = *reinterpret_cast< const u32* >(&j3dDefaultTevOrderInfoNull);
    }
    J3DTevOrder(const J3DTevOrderInfo& info) {
        *reinterpret_cast< u32* >(this) = *reinterpret_cast< const u32* >(&info);
    }
    J3DTevOrder& operator=(const J3DTevOrder& other);
    J3DTevOrderInfo& getTevOrderInfo() {
        return *this;
    }

    u8 getTexMap() {
        return mTexMap;
    }
};

extern u8 j3dTevSwapTableTable[1024];
extern u8 const j3dDefaultTevSwapTableID;

inline u8 calcTevSwapTableID(u8 param_0, u8 param_1, u8 param_2, u8 param_3) {
    return 0x40 * (u8)param_0 + 0x10 * (u8)param_1 + 4 * (u8)param_2 + param_3;
}

struct J3DTevSwapModeTable {
    J3DTevSwapModeTable() {
        mIdx = j3dDefaultTevSwapTableID;
    }
    J3DTevSwapModeTable(J3DTevSwapModeTableInfo const& info) {
        mIdx = calcTevSwapTableID(info.field_0x0, info.field_0x1, info.field_0x2, info.field_0x3);
    }

    J3DTevSwapModeTable& operator=(const J3DTevSwapModeTable& rhs) {
        mIdx = rhs.mIdx;
        return *this;
    }

    J3DTevSwapModeTable& operator=(J3DTevSwapModeTable& other) {
        mIdx = other.mIdx;
        return *this;
    }

    u8 getR() const {
        return *(&j3dTevSwapTableTable[mIdx * 4] + 0);
    }
    u8 getG() const {
        return *(&j3dTevSwapTableTable[mIdx * 4] + 1);
    }
    u8 getB() const {
        return *(&j3dTevSwapTableTable[mIdx * 4] + 2);
    }
    u8 getA() const {
        return *(&j3dTevSwapTableTable[mIdx * 4] + 3);
    }

    /* 0x0 */ u8 mIdx;
};  // Size: 0x1

struct J3DIndTevStage {
    J3DIndTevStage() : mInfo(0) {
        setIndTevStageInfo(j3dDefaultIndTevStageInfo);
    }
    J3DIndTevStage(J3DIndTevStageInfo const& info) : mInfo(0) {
        setIndTevStageInfo(info);
    }
    void setIndTevStageInfo(J3DIndTevStageInfo const& info) {
        setIndStage(info.mIndStage);
        setIndFormat(info.mIndFormat);
        setBiasSel(info.mBiasSel);
        setMtxSel(info.mMtxSel);
        setWrapS(info.mWrapS);
        setWrapT(info.mWrapT);
        setPrev(info.mPrev);
        setLod(info.mLod);
        setAlphaSel(info.mAlphaSel);
    }
    void setIndStage(u8 indStage) {
        mInfo = (mInfo & ~3) | indStage;
    }
    void setIndFormat(u8 indFormat) {
        mInfo = (mInfo & ~0xc) | (indFormat << 2);
    }
    void setBiasSel(u8 biasSel) {
        mInfo = (mInfo & ~0x70) | (biasSel << 4);
    }
    void setMtxSel(u8 mtxSel) {
        mInfo = (mInfo & ~0x1e00) | (mtxSel << 9);
    }
    void setWrapS(u8 wrapS) {
        mInfo = (mInfo & ~0xe000) | (wrapS << 13);
    }
    void setWrapT(u8 wrapT) {
        mInfo = (mInfo & ~0x70000) | (wrapT << 16);
    }
    void setPrev(u8 prev) {
        mInfo = (mInfo & ~0x100000) | (prev << 20);
    }
    void setLod(u8 lod) {
        mInfo = (mInfo & ~0x80000) | (lod << 19);
    }
    void setAlphaSel(u8 alphaSel) {
        mInfo = (mInfo & ~0x180) | (alphaSel << 7);
    }

    void load(u32 param_1) const {
        J3DGDWriteBPCmd(mInfo | (param_1 + 0x10) * 0x1000000);
    }

    J3DIndTevStage& operator=(const J3DIndTevStage& other) {
        mInfo = other.mInfo;
        return *this;
    }

    J3DIndTevStage& operator=(J3DIndTevStage& other) {
        mInfo = other.mInfo;
        return *this;
    }

    /* 0x0 */ u32 mInfo;
};

struct J3DNBTScale;
struct J3DTexCoord;
void loadNBTScale(J3DNBTScale& param_0);

void loadTexCoordGens(u32 param_0, J3DTexCoord* param_1);
void loadTexNo(u32 param_0, u16 const& param_1);
void patchTexNo_PtrToIdx(u32 texID, u16 const& idx);
bool isTexNoReg(void* param_0);
u16 getTexNoReg(void* param_0);

extern J3DTexCoordInfo const j3dDefaultTexCoordInfo[8];

struct J3DTexCoord : public J3DTexCoordInfo {



    J3DTexCoord() {
        J3DTexCoordInfo::operator=(j3dDefaultTexCoordInfo[0]);
        mTexMtxReg = mTexGenMtx;
    }
    J3DTexCoord(const J3DTexCoordInfo& info) {
        J3DTexCoordInfo::operator=(info);
        mTexMtxReg = mTexGenMtx;
    }

    void setTexCoordInfo(const J3DTexCoordInfo& info) {
        __memcpy(this, &info, sizeof(J3DTexCoordInfo));
    }

    u8 getTexGenType() const {
        return mTexGenType;
    }
    u8 getTexGenSrc() const {
        return mTexGenSrc;
    }
    u8 getTexGenMtx() const {
        return mTexGenMtx;
    }
    u32 getTexMtxReg() const {
        return mTexMtxReg & 0xff;
    }
    void setTexGenMtx(u8 param_1) {
        mTexGenMtx = param_1;
    }
    void setTexMtxReg(u16 reg) {
        mTexMtxReg = reg;
    }
    J3DTexCoord& operator=(const J3DTexCoord& other) {
        __memcpy(this, &other, sizeof(J3DTexCoordInfo));
        return *this;
    }

    void resetTexMtxReg() {
        mTexMtxReg = mTexGenMtx;
    }

    /* 0x4 */ u16 mTexMtxReg;
};  // Size: 0x6

struct J3DBlend : public J3DBlendInfo {
    J3DBlend() {
        *reinterpret_cast< u32* >(this) = *reinterpret_cast< const u32* >(&j3dDefaultBlendInfo);
    }
    J3DBlend(J3DBlendInfo const& info) {
        *reinterpret_cast< u32* >(this) = *reinterpret_cast< const u32* >(&info);
    }

    void setType(u8 i_type) {
        mType = i_type;
    }
    void setSrcFactor(u8 i_factor) {
        mSrcFactor = i_factor;
    }
    void setDstFactor(u8 i_factor) {
        mDstFactor = i_factor;
    }

    GXBlendMode getBlendMode() const {
        return (GXBlendMode)mType;
    }
    GXBlendFactor getSrcFactor() const {
        return (GXBlendFactor)mSrcFactor;
    }
    GXBlendFactor getDstFactor() const {
        return (GXBlendFactor)mDstFactor;
    }
    GXLogicOp getLogicOp() const {
        return (GXLogicOp)mOp;
    }

    void load(u8 ditherEnable) const {
        J3DGDSetBlendMode((GXBlendMode)mType, (GXBlendFactor)mSrcFactor, (GXBlendFactor)mDstFactor, (GXLogicOp)mOp, ditherEnable);
    }

    void setBlendInfo(const J3DBlendInfo& i_blendInfo) {
        *static_cast< J3DBlendInfo* >(this) = i_blendInfo;
    }
};

struct J3DFog : public J3DFogInfo {
    inline J3DFog() {
        *(J3DFogInfo*)this = j3dDefaultFogInfo;
    }
    ~J3DFog() {
    }
    J3DFog* getFogInfo() {
        return this;
    }
    void setFogInfo(J3DFogInfo info) {
        *(J3DFogInfo*)this = info;
    }
    void setFogInfo(const J3DFogInfo* info) {
        *(J3DFogInfo*)this = *info;
    }

    void load() const {
        J3DGDSetFog(GXFogType(mType), mStartZ, mEndZ, mNearZ, mFarZ, mColor);
        J3DGDSetFogRangeAdj(mAdjEnable, mCenter, (GXFogAdjTable*)&mFogAdjTable);
    }
};

inline u16 calcAlphaCmpID(u8 comp0, u8 op, u8 comp1) {
    return (comp0 << 5) + (op << 3) + (comp1);
}

struct J3DAlphaComp {
    J3DAlphaComp() : mID(j3dDefaultAlphaCmpID), mRef0(0), mRef1(0) {
    }
    J3DAlphaComp(u16 id) : mID(id), mRef0(0), mRef1(0) {
    }

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

    u8 getComp0() const {
        return *(&j3dAlphaCmpTable[mID * 3] + 0);
    }
    u8 getOp() const {
        return *(&j3dAlphaCmpTable[mID * 3] + 1);
    }
    u8 getComp1() const {
        return *(&j3dAlphaCmpTable[mID * 3] + 2);
    }
    u8 getRef0() const {
        return mRef0;
    }
    u8 getRef1() const {
        return mRef1;
    }

    void load() const {
        J3DGDSetAlphaCompare((GXCompare)getComp0(), mRef0, (GXAlphaOp)getOp(), (GXCompare)getComp1(), mRef1);
    }

    /* 0x00 */ u16 mID;
    /* 0x02 */ u8 mRef0;
    /* 0x03 */ u8 mRef1;
};  // Size: 0x4

static inline u32 setChanCtrlMacro(u8 enable, GXColorSrc ambSrc, GXColorSrc matSrc, u32 lightMask, GXDiffuseFn diffuseFn, GXAttnFn attnFn) {
    return matSrc << 0 | enable << 1 | (lightMask & 0x0F) << 2 | ambSrc << 6 | ((attnFn == GX_AF_SPEC) ? GX_DF_NONE : diffuseFn) << 7 |
           (attnFn != GX_AF_NONE) << 9 | (attnFn != GX_AF_SPEC) << 10 | (lightMask >> 4 & 0x0F) << 11;
}

class J3DColorChan {
public:
    J3DColorChan() {
        setColorChanInfo(j3dDefaultColorChanInfo);
    }
    J3DColorChan(J3DColorChanInfo const& info) {
        u8 ambSrc;
        if (info.mAmbSrc == 0xFF) {
            ambSrc = 0;
        } else {
            ambSrc = info.mAmbSrc;
        }
        mColorChanID = calcColorChanID(info.mEnable, info.mMatSrc, info.mLightMask, info.mDiffuseFn, info.mAttnFn, ambSrc);
    }

    inline void setColorChanInfo(J3DColorChanInfo const& info) {
        // !@bug: It compares info.mAmbSrc (an 8 bit integer) with 0xFFFF instead of 0xFF.
        // This inline is only called by the default constructor J3DColorChan().
        // The J3DColorChan(const J3DColorChanInfo&) constructor does not call this inline, and instead duplicates the
        // same logic but without the bug.
        // See J3DMaterialFactory::newColorChan - both the bugged and correct behavior are present there, as it calls
        // both constructors.
        u8 ambSrc;
        if (info.mAmbSrc == 0xFFFF) {
            ambSrc = 0;
        } else {
            ambSrc = info.mAmbSrc;
        }
        mColorChanID = calcColorChanID(info.mEnable, info.mMatSrc, info.mLightMask, info.mDiffuseFn, info.mAttnFn, ambSrc);
    }
    u8 getLightMask() const {
        return ((mColorChanID >> 2) & 0xf) | ((mColorChanID >> 11) & 0xf) << 4;
    }
    void setLightMask(u8 param_1) {
        mColorChanID = (mColorChanID & ~0x3c) | ((param_1 & 0xf) << 2);
        mColorChanID = (mColorChanID & ~0x7800) | ((param_1 & 0xf0) << 7);
    }

    u8 getEnable() const {
        return (u32)(mColorChanID & 0x2) >> 1;
    }
    u8 getAmbSrc() const {
        return (GXColorSrc)((u32)(mColorChanID & (1 << 6)) >> 6);
    }
    u8 getMatSrc() const {
        return (GXColorSrc)(mColorChanID & 1);
    }
    u8 getDiffuseFn() const {
        return ((u32)(mColorChanID & (3 << 7)) >> 7);
    }

    inline u8 getAttnFn() const {
        u8 AttnArr[] = {2, 0, 2, 1};
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

inline u16 calcZModeID(u8 param_0, u8 param_1, u8 param_2) {
    return param_1 * 2 + param_0 * 0x10 + param_2;
}

extern u8 j3dZModeTable[96];

struct J3DZMode {
    J3DZMode() : mZModeID(j3dDefaultZModeID) {
    }
    J3DZMode(J3DZModeInfo const& info) : mZModeID(calcZModeID(info.field_0x0, info.field_0x1, info.field_0x2)) {
    }

    J3DZMode& operator=(u16 zModeID) {
        mZModeID = zModeID;
        return *this;
    }
    J3DZMode& operator=(const J3DZMode& other) {
        mZModeID = other.mZModeID;
        return *this;
    }

    void setZModeInfo(const J3DZModeInfo& info) {
        mZModeID = calcZModeID(info.field_0x0, info.field_0x1, info.field_0x2);
    }

    void setCompareEnable(u8 i_compare) {
        mZModeID = calcZModeID(i_compare, j3dZModeTable[mZModeID * 3 + 1], j3dZModeTable[mZModeID * 3 + 2]);
    }

    void setFunc(u8 i_func) {
        mZModeID = calcZModeID(j3dZModeTable[mZModeID * 3], i_func, j3dZModeTable[mZModeID * 3 + 2]);
    }

    void setUpdateEnable(u8 i_enable) {
        mZModeID = calcZModeID(j3dZModeTable[mZModeID * 3], j3dZModeTable[mZModeID * 3 + 1], i_enable);
    }

    void load() const {
        J3DGDSetZMode(getCompareEnable(), GXCompare(getFunc()), getUpdateEnable());
    }

    u8 getCompareEnable() const {
        return *(&j3dZModeTable[mZModeID * 3] + 0);
    }
    u8 getFunc() const {
        return *(&j3dZModeTable[mZModeID * 3] + 1);
    }
    u8 getUpdateEnable() const {
        return *(&j3dZModeTable[mZModeID * 3] + 2);
    }

    /* 0x0 */ u16 mZModeID;
};

inline void loadTexCoordScale(GXTexCoordID coord, const J3DTexCoordScaleInfo& info) {
    J3DGDSetTexCoordScale2(coord, info.field_0x00, info.field_0x04 == 1, 0, info.field_0x02, info.field_0x06 == 1, 0);
}

