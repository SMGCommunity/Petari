#pragma once

#include "JSystem/J3DGraphBase/J3DStruct.hpp"

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
