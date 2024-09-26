#pragma once

#include <revolution.h>

class JUTNameTab;
class J3DMaterial;
class J3DTexture;

class J3DMaterialTable {
public:
    J3DMaterialTable();

    virtual ~J3DMaterialTable();

    inline u16 getMaterialCount() {
        return mMaterialCount;
    }

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