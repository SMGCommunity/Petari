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

    void clear();
    int removeMatColorAnimator(J3DAnmColor*);
    int removeTexNoAnimator(J3DAnmTexPattern*);
    int removeTexMtxAnimator(J3DAnmTextureSRTKey*);
    int removeTevRegAnimator(J3DAnmTevRegKey*);
    int createTexMtxForAnimator(J3DAnmTextureSRTKey*);
    int entryMatColorAnimator(J3DAnmColor*);
    int entryTexNoAnimator(J3DAnmTexPattern*);
    int entryTexMtxAnimator(J3DAnmTextureSRTKey*);
    int entryTevRegAnimator(J3DAnmTevRegKey*);

    u16 getMaterialNum() const { return mMaterialNum; }
    J3DTexture* getTexture() const { return mTexture; }
    JUTNameTab* getTextureName() const { return mTextureName; }
    JUTNameTab* getMaterialName() const { return mMaterialName; }
    bool isLocked() const { return field_0x1c == 1; }
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
