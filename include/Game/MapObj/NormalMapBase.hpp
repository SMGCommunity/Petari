#pragma once

#include "Game/Animation/AnmPlayer.h"
#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

class NormalMapBase : public LiveActor {
public:
    NormalMapBase(const char *);

    virtual ~NormalMapBase();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void control();
    virtual void drawSetting(MtxPtr) const;
    virtual void drawMaterialSetting(J3DMaterial *) const;
    virtual void setTevForObject() const;
    virtual void setTevForObject_Material() const;
    virtual void setTevForDebug() const;

    void setup(const char *);
    void initNormalMap();
    void setupLighting();

    void updateBtkMtx();
    void loadTexMtxBtk(J3DMaterial *) const;
    void updateLightMtx(MtxPtr) const;
    bool isNormalMapMaterial(const char *) const;
    void standardDraw(J3DModel *) const;
    void shapeAnalyzeDraw(J3DModel *) const;

    void loadDirectLightTex(_GXTexMapID) const;

    JUTTexture* _90;
    JUTTexture* mNormalTex;     // 0x94
    u8 _98[0xDC-0x98];
    void* mImagePtr;            // 0xDC
    u32 _E0;
    s32 nGradTexMode;           // 0xE4
    u32 _E8;
    u32 _EC;
    u8 _F0[0x124-0xF0];
    u8 mBackLightMode;          // 0x124
    Color10 mTevColor0;         // 0x128
    Color10 mTevColor1;         // 0x130
    u8 mLightAMode;             // 0x13C
    u8 mLightBMode;             // 0x13D
    u16 mHardLightColorMask;    // 0x13E
    u16 mHardLightAlphaMask;    // 0x140
    u8 mAmbColor;               // 0x142
    u8 mMatColor;               // 0x143
    u8 mAmbAlpha;               // 0x144
    u8 mMatAlpha;               // 0x145
    u16 mUseModelLightReg;      // 0x146
    u16 mUseModelTevReg;        // 0x148
    u16 _14A;
    BtkPlayer* mBtkPlayer;      // 0x14C
    u8 _150;
    u8 _151[0x184-0x151];
    MtxPtr _184[0x10];             // 0x184

    JUTTexture* _4B8;
    u32 _4BC;
    u32 _4C0;
    u8 _4C4;
    u8 _4C5;
};