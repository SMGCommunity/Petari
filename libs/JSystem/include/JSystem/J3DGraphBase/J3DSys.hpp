#pragma once

#include <revolution.h>

enum J3DSysDrawBuf {
    /* 0x0 */ J3DSysDrawBuf_Opa,
    /* 0x1 */ J3DSysDrawBuf_Xlu,

    /* 0x2 */ J3DSysDrawBuf_MAX
};

struct J3DTexCoordScaleInfo {
    /* 0x0 */ u16 field_0x00;
    /* 0x2 */ u16 field_0x02;
    /* 0x4 */ u16 field_0x04;
    /* 0x6 */ u16 field_0x06;
};

enum J3DError {
    kJ3DError_Success = 0,
    kJ3DError_Alloc = 4,
};

class J3DMtxCalc;
class J3DModel;
class J3DMatPacket;
class J3DShapePacket;
class J3DShape;
class J3DDrawBuffer;
class J3DTexture;

enum J3DSysFlag {
    J3DSysFlag_SkinPosCpu = 0x00000004,
    J3DSysFlag_SkinNrmCpu = 0x00000008,
    J3DSysFlag_PostTexMtx = 0x40000000,
};

class J3DSys {
public:
    /* 0x000 */ Mtx mViewMtx;
    /* 0x030 */ J3DMtxCalc* mCurrentMtxCalc;
    /* 0x034 */ u32 mFlags;
    /* 0x038 */ J3DModel* mModel;
    /* 0x03C */ J3DMatPacket* mMatPacket;
    /* 0x040 */ J3DShapePacket* mShapePacket;
    /* 0x044 */ J3DShape* mShape;
    /* 0x048 */ J3DDrawBuffer* mDrawBuffer[J3DSysDrawBuf_MAX];
    /* 0x050 */ u32 mDrawMode;
    /* 0x054 */ u32 mMaterialMode;
    /* 0x058 */ J3DTexture* mTexture;
    /* 0x05C */ u8 unk_0x5c[0x60 - 0x5C];
    /* 0x060 */ u32 mTexCacheRegionNum;
    /* 0x064 */ GXTexRegion mTexCacheRegion[8];
    /* 0x0E4 */ u8 unk_0xe4[0x104 - 0xE4];
    /* 0x104 */ Mtx* mModelDrawMtx;
    /* 0x108 */ Mtx33* mModelNrmMtx;
    /* 0x10C */ void* mVtxPos;
    /* 0x110 */ void* mVtxNrm;
    /* 0x114 */ GXColor* mVtxCol;
    /* 0x118 */ Vec* mNBTScale;

    J3DSys();

    void loadPosMtxIndx(int, u16) const;
    void loadNrmMtxIndx(int, u16) const;
    void setTexCacheRegion(GXTexCacheSize);
    void drawInit();
    void reinitGX();
    void reinitGenMode();
    void reinitLighting();
    void reinitTransform();
    void reinitTexture();
    void reinitTevStages();
    void reinitIndStages();
    void reinitPixelProc();

    MtxPtr getViewMtx() { return mViewMtx; }

    void onFlag(u32 flag) { mFlags |= flag; }
    void offFlag(u32 flag) { mFlags &= ~flag; }
    bool checkFlag(u32 flag) { return mFlags & flag ? true : false; }

    J3DMatPacket* getMatPacket() { return mMatPacket; }
    void setMaterialMode(u32 mode) { mMaterialMode = mode; }

    void* getVtxPos() { return mVtxPos; }
    void setVtxPos(void* pVtxPos) { mVtxPos = pVtxPos; }

    void* getVtxNrm() { return mVtxNrm; }
    void setVtxNrm(void* pVtxNrm) { mVtxNrm = pVtxNrm; }

    void* getVtxCol() { return mVtxCol; }
    void setVtxCol(GXColor* pVtxCol) { mVtxCol = pVtxCol; }

    void setModelDrawMtx(Mtx* pMtxArr) {
        mModelDrawMtx = pMtxArr;
        GXSetArray(GX_POS_MTX_ARRAY, mModelDrawMtx, sizeof(*mModelDrawMtx));
    }

    void setModelNrmMtx(Mtx33* pMtxArr) {
        mModelNrmMtx = pMtxArr;
        GXSetArray(GX_NRM_MTX_ARRAY, mModelNrmMtx, sizeof(*mModelNrmMtx));
    }

    void setNBTScale(Vec* scale) { mNBTScale = scale; }
    Vec* getNBTScale() { return mNBTScale; }

    void setMatPacket(J3DMatPacket* pPacket) { mMatPacket = pPacket; }

    void setShapePacket(J3DShapePacket* pPacket) { mShapePacket = pPacket; }

    J3DModel* getModel() { return mModel; }
    void setModel(J3DModel* pModel) { mModel = pModel; }

    J3DShapePacket* getShapePacket() { return mShapePacket; }
    Mtx& getModelDrawMtx(u16 no) { return mModelDrawMtx[no]; }
    void setTexture(J3DTexture* pTex) { mTexture = pTex; }
    J3DTexture* getTexture() { return mTexture; }

    J3DDrawBuffer* getDrawBuffer(int type) { return mDrawBuffer[type]; }

    static Mtx mCurrentMtx;
    static Vec mCurrentS;
    static Vec mParentS;
    static J3DTexCoordScaleInfo sTexCoordScaleTable[8];
};

extern u32 j3dDefaultViewNo;
extern J3DSys j3dSys;
