#pragma once

#include <revolution/types.h>

class J3DAnmColor;
class J3DAnmTevRegKey;
class J3DAnmTexPattern;
class J3DAnmTextureSRTKey;
class J3DMaterial;
class J3DTexture;
class JUTNameTab;

class J3DMaterialTable {
public:
    J3DMaterialTable();

    virtual ~J3DMaterialTable();

    s32 removeMatColorAnimator(J3DAnmColor*);
    s32 removeTexNoAnimator(J3DAnmTexPattern*);
    s32 removeTexMtxAnimator(J3DAnmTextureSRTKey*);
    s32 removeTevRegAnimator(J3DAnmTevRegKey*);
    s32 createTexMtxForAnimator(J3DAnmTextureSRTKey*);
    s32 entryMatColorAnimator(J3DAnmColor*);
    s32 entryTexNoAnimator(J3DAnmTexPattern*);
    s32 entryTexMtxAnimator(J3DAnmTextureSRTKey*);
    s32 entryTevRegAnimator(J3DAnmTevRegKey*);

    inline u16 getMaterialCount() { return mMaterialCount; }

    u16 mMaterialCount;             // 0x4
    u16 mUniqueMaterialCount;       // 0x6
    J3DMaterial** mMaterials;       // 0x8
    JUTNameTab* mNameTable;         // 0xC
    J3DMaterial* mUniqueMaterials;  // 0x10
    J3DTexture* mTexture;           // 0x14
    JUTNameTab* mTextureNameTable;  // 0x18
    u16 _1C;
    u8 _1E;
    u8 _1F;
};