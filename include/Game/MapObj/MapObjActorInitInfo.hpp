#pragma once

#include <JSystem/JGeometry/TVec.hpp>

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
    void setupSeesaw1AxisRotator(const char*, f32);
    void setupRailPosture();
    void setupBaseMtxFollowTarget();
    void setupAffectedScale();
    void setupSeAppear();
    void setupMirrorReflection(bool);
    void setupPrepareChangeDummyTexture(const char*);
    void setupNoUseLodCtrl();

    /* 0x00 */ bool mSetDefaultPosition;
    /* 0x01 */ bool mConnectToScene;
    /* 0x02 */ bool mInitBinder;
    /* 0x03 */ bool mHasEffect;
    /* 0x04 */ bool mHasSensors;
    /* 0x05 */ bool mHasShadows;
    /* 0x06 */ bool mCalcGravity;
    /* 0x07 */ bool mUseProjectMapMtx;
    /* 0x08 */ bool mInitFur;
    /* 0x09 */ bool mHasRailMover;
    /* 0x0A */ bool mHasRotator;
    /* 0x0B */ bool mHasRailRotator;
    /* 0x0C */ bool _C;
    /* 0x0D */ bool _D;
    /* 0x0E */ bool mUseRailPosture;
    /* 0x0F */ bool mUseBaseMtxFollowTarget;
    /* 0x10 */ bool mIsAffectedByScale;
    /* 0x11 */ bool mUseMirrorReflection;
    /* 0x12 */ bool _12;
    /* 0x13 */ bool _13;
    /* 0x14 */ const char* mModelName;
    /* 0x18 */ const char* mHioNode;
    /* 0x1C */ u16 _1C;
    /* 0x20 */ f32 mSensorRadius;
    /* 0x24 */ TVec3f mSensorPosition;
    /* 0x30 */ bool mHasHitSensorCB;
    /* 0x31 */ u8 _31;
    /* 0x32 */ u8 _32;
    /* 0x33 */ u8 _33;
    /* 0x34 */ f32 mBinderRadius;
    /* 0x38 */ f32 mBinderCenterY;
    /* 0x3C */ const char* mEffectName;
    /* 0x40 */ s32 mSound;
    /* 0x44 */ TVec3f* mSoundPos;
    /* 0x48 */ bool _48;
    /* 0x4C */ const Nerve* mNerve;
    /* 0x50 */ s32 mGroupClipping;
    /* 0x54 */ f32 mClippingRadius;
    /* 0x58 */ f32 mFarClipping;
    /* 0x5C */ s32 _5C;
    /* 0x60 */ s32 mColorChangeArg;    // Obj_arg0
    /* 0x64 */ s32 mTextureChangeArg;  // Obj_arg1
    /* 0x68 */ bool mIsAppearRiddleSE;
    /* 0x6C */ const char* mSeesaw1AxisRotatorName;
    /* 0x70 */ f32 mSeesaw1AxisRotatorRadius;
    /* 0x74 */ bool _74;
    /* 0x78 */ const char* mShadowName;
    /* 0x7C */ f32 mShadowLength;  // Obj_arg2
    /* 0x80 */ const char* _80;
    /* 0x84 */ const char* mDummyChangeTexture;
    /* 0x88 */ u32 _88;
    /* 0x8C */ bool _8C;
    /* 0x8D */ bool mNoUseLOD;
};
