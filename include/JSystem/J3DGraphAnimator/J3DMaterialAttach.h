#pragma once

#include <revolution.h>

class JUTNameTab;
class J3DMaterial;
class J3DTexture;

class J3DMaterialTable {
public:
    J3DMaterialTable();

    virtual ~J3DMaterialTable();

    u16 _4;
    u16 _6;
    J3DMaterial** _8;
    JUTNameTab* _C;
    J3DMaterial* _10;
    J3DTexture* _14;
    JUTNameTab* _18;
    u16 _1C;
    u8 _1E;
    u8 _1F;
};