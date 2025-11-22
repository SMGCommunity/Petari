#pragma once

#include "JSystem/JGeometry.hpp"

class Nerve;

class MapObjActorInitInfo {
public:
    MapObjActorInitInfo();

    void setupHioNode(const char*);
    void setupDefaultPos();
    void setupModelName(const char*);
    void setupConnectToScene();
    void setupBinder(f32, f32);
    void setupEffect(const char*);
    void setupSound(s32);
    void setupSoundPos(TVec3f*);
    void setupNoAppearRiddleSE();
    void setupHitSensor();
    void setupHitSensorCallBack();
    void setupHitSensorParam(u16, f32, const TVec3f&);
    void setupNerve(const Nerve*);
    void setupShadow(const char*);
    void setupGroupClipping(s32);
    void setupClippingRadius(f32);
    void setupFarClipping(f32);
    void setupProjmapMtx(bool);
    void setupRailMover();
    void setupRotator();
    void setupRailRotator();
    void setupSEesaw1AxisRotator(const char*, f32);
    void setupRailPosture();
    void setupBaseMtxFollowTarget();
    void setupAffectedScale();
    void setupSeAppear();
    void setupMirrorReflection(bool);
    void setupPrepareChangeDummyTexture(const char*);
    void setupNoUseLodCtrl();

    bool mSetDefaultPosition;  // 0x0
    bool mConnectToScene;      // 0x1
    bool mInitBinder;          // 0x2
    bool mHasEffect;           // 0x3
    bool mHasSensors;          // 0x4
    bool mHasShadows;          // 0x5
    bool mCalcGravity;         // 0x6
    bool mProjectMapMtx;       // 0x7
    bool mInitFur;             // 0x8
    bool mHasRailMover;        // 0x9
    bool mHasRotator;          // 0xA
    bool mHasRailRotator;      // 0xB
    u8 _C;
    u8 _D;
    bool mUsesRailPosture;          // 0xE
    bool mDoesBaseMtxFollowTarget;  // 0xF
    bool mIsAffectedByScale;        // 0x10
    bool mUseMirrorReflection;      // 0x11
    u8 _12;
    u8 _13;
    const char* mModelName;  // 0x14
    const char* mHioNode;    // 0x18
    u16 _1C;
    u8 _1E;
    u8 _1F;
    f32 mSensorSize;       // 0x20
    TVec3f mSensorOffset;  // 0x24
    bool mHitSensorCB;     // 0x30
    u8 _31;
    u8 _32;
    u8 _33;
    f32 mBinderRadius;        // 0x34
    f32 mBinderCenterY;       // 0x38
    const char* mEffectName;  // 0x3C
    s32 mSound;               // 0x40
    TVec3f* mSoundPos;        // 0x44
    u8 _48;
    const Nerve* mNerve;  // 0x4C
    s32 mGroupClipping;   // 0x50
    f32 mClippingRadius;  // 0x54
    f32 mFarClipping;     // 0x58
    s32 _5C;
    s32 mColorChangeArg;     // 0x60
    s32 mTextureChangeArg;   // 0x64
    bool mNoAppearRiddleSE;  // 0x68
    u32 _6C;
    f32 _70;
    bool _74;
    const char* mShadowName;  // 0x78
    f32 mShadowLength;        // 0x7C
    u32 _80;
    const char* mDummyChangeTexture;  // 0x84
    u32 _88;
    bool _8C;
    bool mNoUseLOD;  // 0x8D
};
