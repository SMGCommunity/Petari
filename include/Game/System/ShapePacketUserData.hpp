#pragma once

#include <revolution.h>

class J3DMaterial;
class J3DShapePacket;
class J3DModel;

class ShapePacketUserData {
public:
    ShapePacketUserData();

    void init(J3DMaterial* pMaterial);
    void callDL() const;
    void loadTexMtx(J3DMaterial* pMaterial, int slot, u16 index) const;

    struct TexMtxInfo {
        TexMtxInfo() : mType(), mPostMtx(GX_PTIDENTITY) {
        }

        /* 0x00 */ s32 mType;
        /* 0x04 */ u32 mPostMtx;
    };

    /* 0x00 */ TexMtxInfo mTexMtxInfo[8];
    /* 0x40 */ u32 mTexGenNum;
    /* 0x44 */ u32 mDisplayListSize;
    /* 0x48 */ u8* mDisplayList;
};

namespace MR {
    ShapePacketUserData* getJ3DShapePacketUserData(const J3DShapePacket* pShapePacket);
    void initJ3DShapePacketUserData(J3DModel* pModel);
};  // namespace MR
