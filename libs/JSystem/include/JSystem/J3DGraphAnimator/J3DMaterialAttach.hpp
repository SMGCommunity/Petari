#pragma once

#include <revolution.h>

class JUTNameTab;
class J3DMaterial;
class J3DTexture;

class J3DMaterialTable {
public:
    J3DMaterialTable();

    virtual ~J3DMaterialTable();

    u16 mMaterialCount;             // _4
    u16 mUniqueMaterialCount;       // _6 
    J3DMaterial** mMaterials;       // _8
    JUTNameTab* mNameTable;         // _C
    J3DMaterial* mUniqueMaterials;  // _10
    J3DTexture* mTexture;           // _14
    JUTNameTab* mTextureNameTable;  // _18
    u16 _1C;
    u8 _1E;
    u8 _1F;
};