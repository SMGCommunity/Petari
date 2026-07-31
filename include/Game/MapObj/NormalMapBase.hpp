#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Color.hpp"

class BtkPlayer;
class J3DMaterial;
class J3DModel;
class JUTTexture;

class NormalMapBase : public LiveActor {
public:
    NormalMapBase(const char*);

    virtual ~NormalMapBase();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void control();
    virtual void drawSetting(MtxPtr) const;
    virtual void drawMaterialSetting(J3DMaterial*) const;
    virtual void setTevForObject() const;
    virtual void setTevForObject_Material() const;
    virtual void setTevForDebug() const;

    void setup(const char*);
    void initNormalMap();
    void setupLighting();
    void setupTexture();

    void updateBtkMtx();
    void loadTexMtxBtk(J3DMaterial*) const;
    void updateLightMtx(MtxPtr) const;
    bool isNormalMapMaterial(const char*) const;
    void standardDraw(J3DModel*) const;
    void shapeAnalyzeDraw(J3DModel*) const;

    void loadDirectLightTex(GXTexMapID) const;

    void indirectCapture() const;

    void drawSettingForDebug(MtxPtr) const;
    void drawSettingForCapture(MtxPtr) const;

    void loadDiffuseGradTex(GXTexMapID) const;
    void loadNormalTex(const JUTTexture*, GXTexMapID) const;

    /* 0x08C */ u32 _8C;
    /* 0x090 */ JUTTexture* _90;
    /* 0x094 */ JUTTexture* mNormalTex;
    /* 0x098 */ u8 _98[0xDC - 0x98];
    /* 0x0DC */ void* mImagePtr;
    /* 0x0E0 */ u32 _E0;
    /* 0x0E4 */ s32 nGradTexMode;
    /* 0x0E8 */ u32 _E8;
    /* 0x0EC */ s32 _EC;
    /* 0x0F0 */ u32 _F0;
    /* 0x0F4 */ Mtx _F4;
    /* 0x124 */ u8 mBackLightMode;
    /* 0x125 */ u8 _125;
    /* 0x126 */ u8 _126;
    /* 0x127 */ u8 _127;
    /* 0x128 */ Color10 mTevColor0;
    /* 0x130 */ Color10 mTevColor1;
    /* 0x138 */ u16 _138;
    /* 0x13A */ u16 _13A;
    /* 0x13C */ u8 mLightAMode;
    /* 0x13D */ u8 mLightBMode;
    /* 0x13E */ u16 mHardLightColorMask;
    /* 0x140 */ u16 mHardLightAlphaMask;
    /* 0x142 */ u8 mAmbColor;
    /* 0x143 */ u8 mMatColor;
    /* 0x144 */ u8 mAmbAlpha;
    /* 0x145 */ u8 mMatAlpha;
    /* 0x146 */ u16 mUseModelLightReg;
    /* 0x148 */ u16 mUseModelTevReg;
    /* 0x14A */ u8 mLightingHighLevel;
    /* 0x14B */ u8 mLightingLowLevel;
    /* 0x14C */ BtkPlayer* mBtkPlayer;
    /* 0x150 */ u8 _150;
    /* 0x151 */ u8 _151[0x184 - 0x151];
    /* 0x184 */ Mtx _184[0x10];
    /* 0x484 */ u8 _484[0x34];
    /* 0x4B8 */ JUTTexture* _4B8;
    /* 0x4BC */ int mHeight;
    /* 0x4C0 */ int mWidth;
    /* 0x4C4 */ u8 _4C4;
    /* 0x4C5 */ u8 _4C5;
};
