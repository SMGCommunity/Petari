#pragma once

#include "JSystem/J3DGraphBase/J3DStruct.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include <stdint.h>

class J3DTexture {
private:
    /* 0x0 */ u16 mNum;
    /* 0x2 */ u16 unk_0x2;
    /* 0x4 */ ResTIMG* mpRes;

public:
    J3DTexture(u16 num, ResTIMG* res) : mNum(num), unk_0x2(0), mpRes(res) {}

    void loadGX(u16, GXTexMapID) const;
    void entryNum(u16);
    void addResTIMG(u16, ResTIMG const*);
    virtual ~J3DTexture() {}

    u16 getNum() const { return mNum; }

    ResTIMG* getResTIMG(u16 index) const { return &mpRes[index]; }

    void setResTIMG(u16 index, const ResTIMG& timg) {
        mpRes[index] = timg;
        mpRes[index].mImageDataOffset = ((mpRes[index].mImageDataOffset + (uintptr_t)&timg - (uintptr_t)(mpRes + index)));
        mpRes[index].mPaletteDataOffset = ((mpRes[index].mPaletteDataOffset + (uintptr_t)&timg - (uintptr_t)(mpRes + index)));
    }
};

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

    void setTexCoordInfo(const J3DTexCoordInfo& info) { __memcpy(this, &info, sizeof(J3DTexCoordInfo)); }

    u8 getTexGenType() const { return mTexGenType; }
    u8 getTexGenSrc() const { return mTexGenSrc; }
    u8 getTexGenMtx() const { return mTexGenMtx; }
    u32 getTexMtxReg() const { return mTexMtxReg & 0xff; }
    void setTexGenMtx(u8 param_1) { mTexGenMtx = param_1; }
    void setTexMtxReg(u16 reg) { mTexMtxReg = reg; }
    J3DTexCoord& operator=(const J3DTexCoord& other) {
        *(u32*)this = *(u32*)&other;
        return *this;
    }

    void resetTexMtxReg() { mTexMtxReg = mTexGenMtx; }

    /* 0x4 */ u16 mTexMtxReg;
};  // Size: 0x6

extern J3DTexMtxInfo const j3dDefaultTexMtxInfo;

class J3DTexMtx {
public:
    J3DTexMtx() { mTexMtxInfo = j3dDefaultTexMtxInfo; }

    J3DTexMtx(const J3DTexMtxInfo& info) { mTexMtxInfo = info; }

    void load(u32) const;
    void calc(const Mtx);
    void calcTexMtx(const Mtx);
    void calcPostTexMtx(const Mtx);
    void loadTexMtx(u32) const;
    void loadPostTexMtx(u32) const;

    J3DTexMtxInfo& getTexMtxInfo() { return mTexMtxInfo; }
    Mtx& getMtx() { return mMtx; }
    void setEffectMtx(Mtx effectMtx) { mTexMtxInfo.setEffectMtx(effectMtx); }

private:
    /* 0x00 */ J3DTexMtxInfo mTexMtxInfo;
    /* 0x64 */ Mtx mMtx;
};  // Size: 0x94
