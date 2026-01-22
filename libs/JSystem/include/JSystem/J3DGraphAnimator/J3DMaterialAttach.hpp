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

    u16 getMaterialNum() const { return mMaterialNum; }
    J3DTexture* getTexture() const { return mTexture; }
    JUTNameTab* getTextureName() const { return mTextureName; }

    J3DMaterial* getMaterialNodePointer(u16 idx) const { return mMaterialNodePointer[idx]; }

    /* 0x04 */ u16 mMaterialNum;
    /* 0x06 */ u16 mUniqueMatNum;
    /* 0x08 */ J3DMaterial** mMaterialNodePointer;
    /* 0x0C */ JUTNameTab* mMaterialName;
    /* 0x10 */ J3DMaterial* field_0x10;
    /* 0x14 */ J3DTexture* mTexture;
    /* 0x18 */ JUTNameTab* mTextureName;
    /* 0x1C */ u16 field_0x1c;
};
