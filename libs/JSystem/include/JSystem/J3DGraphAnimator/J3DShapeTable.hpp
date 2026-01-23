#pragma once

#include "JSystem/J3DGraphBase/J3DShape.hpp"
#include <revolution/types.h>

class JUTNameTab;

class J3DShapeTable {
public:
    J3DShapeTable() {
        mShapeNum = 0;
        mShapeNodePointer = NULL;
        mShapeName = NULL;
    }

    void hide();
    void show();
    void initShapeNodes(J3DDrawMtxData*, J3DVertexData*);
    void sortVcdVatCmd();

    virtual ~J3DShapeTable() {}

    u16 getShapeNum() const { return mShapeNum; }
    J3DShape* getShapeNodePointer(u16 idx) const { return mShapeNodePointer[idx]; }

private:
    friend class J3DModelLoader;

    /* 0x4 */ u16 mShapeNum;
    /* 0x8 */ J3DShape** mShapeNodePointer;
    /* 0xC */ JUTNameTab* mShapeName;
};  // Size: 0x10
