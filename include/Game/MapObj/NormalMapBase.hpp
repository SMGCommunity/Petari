#pragma once

#include "Game/Animation/AnmPlayer.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include "revolution/gx/GXEnum.h"

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

    void loadDirectLightTex(_GXTexMapID) const;

    void indirectCapture() const;

    void drawSettingForDebug(MtxPtr) const;
    void drawSettingForCapture(MtxPtr) const;

    void loadDiffuseGradTex(_GXTexMapID) const;
    void loadNormalTex(const JUTTexture*, _GXTexMapID) const;

    u32 _8C;
    JUTTexture* _90;
    JUTTexture* mNormalTex;  // 0x94
    u8 _98[0xDC - 0x98];
    void* mImagePtr;  // 0xDC
    u32 _E0;
    s32 nGradTexMode;  // 0xE4
    u32 _E8;
    s32 _EC;
    u32 _F0;
    Mtx _F4;
    u8 mBackLightMode;  // 0x124
    u8 _125;
    u8 _126;
    u8 _127;
    Color10 mTevColor0;  // 0x128
    Color10 mTevColor1;  // 0x130
    u16 _138;
    u16 _13A;
    u8 mLightAMode;           // 0x13C
    u8 mLightBMode;           // 0x13D
    u16 mHardLightColorMask;  // 0x13E
    u16 mHardLightAlphaMask;  // 0x140
    u8 mAmbColor;             // 0x142
    u8 mMatColor;             // 0x143
    u8 mAmbAlpha;             // 0x144
    u8 mMatAlpha;             // 0x145
    u16 mUseModelLightReg;    // 0x146
    u16 mUseModelTevReg;      // 0x148
    u8 mLightingHighLevel;    // 0x14A
    u8 mLightingLowLevel;     // 0x14B
    BtkPlayer* mBtkPlayer;    // 0x14C
    u8 _150;
    u8 _151[0x184 - 0x151];
    Mtx _184[0x10];  // 0x184
    u8 _484[0x34];
    JUTTexture* _4B8;
    int mHeight;  // 0x4BC
    int mWidth;   // 0x4C0
    u8 _4C4;
    u8 _4C5;
};
