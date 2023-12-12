#pragma once

#include "JSystem/JGeometry.h"

class Nerve;

class MapObjActorInitInfo {
public:
    MapObjActorInitInfo();

    void setupHioNode(const char *);
    void setupDefaultPos();
    void setupModelName(const char *);
    void setupConnectToScene();
    void setupBinder(f32, f32);
    void setupEffect(const char *);
    void setupSound(s32);
    void setupSoundPos(TVec3f *);
    void setupNoAppearRiddleSE();
    void setupHitSensor();
    void setupHitSensorCallBack();
    void setupHitSensorParam(u16, f32, const TVec3f &);
    void setupNerve(const Nerve *);
    void setupShadow(const char *);
    void setupGroupClipping(s32);
    void setupClippingRadius(f32);
    void setupFarClipping(f32);
    void setupProjmapMtx(bool);
    void setupRailMover();
    void setupRotator();
    void setupRailRotator();
    void setupSEesaw1AxisRotator(const char *, f32);
    void setupRailPosture();
    void setupBaseMtxFollowTarget();
    void setupAffectedScale();
    void setupSeAppear();
    void setupMirrorReflection(bool);
    void setupPrepareChangeDummyTexture(const char *);
    void setupNouseLodCtrl();

    bool mSetDefaultPosition;           // _0
    bool mConnectToScene;               // _1
    bool mInitBinder;                   // _2
    bool mHasEffect;                    // _3
    bool mHasSensors;                   // _4
    bool mHasShadows;                   // _5
    bool mCalcGravity;                  // _6
    bool mProjectMapMtx;                // _7
    bool mInitFur;                      // _8
    bool mHasRailMover;                 // _9
    bool mHasRotator;                   // _A
    bool mHasRailRotator;               // _B
    u8 _C;
    u8 _D;
    bool mUsesRailPosture;              // _E
    bool mDoesBaseMtxFollowTarget;      // _F
    bool mIsAffectedByScale;            // _10
    bool mUseMirrorReflection;          // _11
    u8 _12;
    u8 _13;
    const char* mModelName;             // _14
    const char* mHioNode;               // _18
    u16 _1C;
    u8 _1E;
    u8 _1F;
    f32 mSensorSize;                    // _20
    TVec3f mSensorOffset;               // _24
    bool mHitSensorCB;                  // _30
    u8 _31;
    u8 _32;
    u8 _33;
    f32 mBinderRadius;                  // _34
    f32 mBinderCenterY;                 // _38
    const char* mEffectName;            // _3C
    s32 mSound;                         // _40
    TVec3f* mSoundPos;                  // _44
    u8 _48;
    const Nerve* mNerve;                // _4C
    s32 mGroupClipping;                 // _50
    f32 mClippingRadius;                // _54
    f32 mFarClipping;                   // _58
    s32 _5C;
    s32 mColorChangeArg;                // _60
    s32 mTextureChangeArg;              // _64
    bool mNoAppearRiddleSE;             // _68
    u32 _6C;
    f32 _70;
    bool _74;
    const char* mShadowName;            // _78
    f32 mShadowLength;                  // _7C
    u32 _80;
    const char* mDummyChangeTexture;    // _84
    u32 _88;
    bool _8C;
    bool mNoUseLOD;                     // _8D
};
