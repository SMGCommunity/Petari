#pragma once

#include "JSystem/J3DGraphBase/J3DMatBlock.hpp"
#include "JSystem/J3DGraphBase/J3DPacket.hpp"
#include "JSystem/J3DGraphBase/J3DShape.hpp"
#include "JSystem/J3DGraphBase/J3DStruct.hpp"
#include <revolution.h>

class J3DShape;
class J3DJoint;
class J3DColorBlockLightOn;
class J3DTexGenBlockPatched;
class J3DTevBlockPatched;
class J3DPEBlock;
class J3DMaterialAnm;
class J3DDisplayListObj;

class J3DMaterial {
public:
    virtual void calc(f32 const (*)[4]);
    virtual void calcDiffTexMtx(f32 const (*)[4]);
    virtual void makeDisplayList();
    virtual void makeSharedDisplayList();
    virtual void load();
    virtual void loadSharedDL();
    virtual void patch();
    virtual void diff(u32);
    virtual void reset();
    virtual void change();

    static J3DColorBlock* createColorBlock(u32);
    static J3DTexGenBlock* createTexGenBlock(u32);
    static J3DTevBlock* createTevBlock(int);
    static J3DIndBlock* createIndBlock(int);
    static J3DPEBlock* createPEBlock(u32, u32);
    static u32 calcSizeColorBlock(u32);
    static u32 calcSizeTexGenBlock(u32);
    static u32 calcSizeTevBlock(int);
    static u32 calcSizeIndBlock(int);
    static u32 calcSizePEBlock(u32, u32);
    void initialize();
    u32 countDLSize();
    void makeDisplayList_private(J3DDisplayListObj*) NO_INLINE;
    void setCurrentMtx();
    void calcCurrentMtx();
    void copy(J3DMaterial*) NO_INLINE;
    s32 newSharedDisplayList(u32);
    s32 newSingleSharedDisplayList(u32);

    inline J3DMaterialAnm* getMaterialAnm() {
        if ((uintptr_t)mMaterialAnm < 0xC0000000) {
            return mMaterialAnm;
        } else {
            return NULL;
        }
    }

    bool isDrawModeOpaTexEdge() { return (mMaterialMode & 3) == 0; }

    u16 getIndex() { return mIndex; }

    J3DTexMtx* getTexMtx(u32 idx) { return mTexGenBlock->getTexMtx(idx); }
    void setTexMtx(u32 idx, J3DTexMtx* mtx) { mTexGenBlock->setTexMtx(idx, mtx); }

    J3DNBTScale* getNBTScale() { return mTexGenBlock->getNBTScale(); }

    J3DTexCoord* getTexCoord(u32 idx) { return mTexGenBlock->getTexCoord(idx); }

    u32 getTexGenNum() const { return mTexGenBlock->getTexGenNum(); }
    u8 getTevStageNum() const { return mTevBlock->getTevStageNum(); }

    J3DMaterial* getNext() { return mNext; }
    J3DShape* getShape() { return mShape; }
    J3DTevBlock* getTevBlock() { return mTevBlock; }
    J3DColorBlock* getColorBlock() { return mColorBlock; }
    J3DTexGenBlock* getTexGenBlock() { return mTexGenBlock; }
    J3DDisplayListObj* getSharedDisplayListObj() { return mSharedDLObj; }
    J3DIndBlock* getIndBlock() { return mIndBlock; }
    J3DJoint* getJoint() { return mJoint; }
    J3DPEBlock* getPEBlock() { return mPEBlock; }

    u16 getTexNo(u32 idx) { return mTevBlock->getTexNo(idx); }

    void setNext(J3DMaterial* pMaterial) { mNext = pMaterial; }

    void addShape(J3DShape* pShape) { mShape = pShape; }

    void setJoint(J3DJoint* pJoint) { mJoint = pJoint; }

    /* 0x04 */ J3DMaterial* mNext;
    /* 0x08 */ J3DShape* mShape;
    /* 0x0C */ J3DJoint* mJoint;
    /* 0x10 */ u32 mMaterialMode;
    /* 0x14 */ u16 mIndex;
    /* 0x18 */ u32 mInvalid;
    /* 0x1C */ u32 field_0x1c;
    /* 0x20 */ u32 mDiffFlag;
    /* 0x24 */ J3DColorBlock* mColorBlock;
    /* 0x28 */ J3DTexGenBlock* mTexGenBlock;
    /* 0x2C */ J3DTevBlock* mTevBlock;
    /* 0x30 */ J3DIndBlock* mIndBlock;
    /* 0x34 */ J3DPEBlock* mPEBlock;
    /* 0x38 */ J3DMaterial* mpOrigMaterial;
    /* 0x3C */ J3DMaterialAnm* mMaterialAnm;
    /* 0x40 */ J3DCurrentMtx mCurrentMtx;
    /* 0x48 */ J3DDisplayListObj* mSharedDLObj;
};

class J3DPatchedMaterial : public J3DMaterial {
public:
    J3DPatchedMaterial() { initialize(); }
    void initialize();

    virtual void makeDisplayList();
    virtual void makeSharedDisplayList();
    virtual void load();
    virtual void loadSharedDL();
    virtual void reset();
    virtual void change();
};

class J3DLockedMaterial : public J3DMaterial {
public:
    J3DLockedMaterial() { initialize(); }
    void initialize();

    virtual void calc(f32 const (*)[4]);
    virtual void makeDisplayList();
    virtual void makeSharedDisplayList();
    virtual void load();
    virtual void loadSharedDL();
    virtual void patch();
    virtual void diff(u32);
    virtual void reset();
    virtual void change();
};
