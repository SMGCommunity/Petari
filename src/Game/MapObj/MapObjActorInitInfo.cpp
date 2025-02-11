#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"


// scheduling issues with the paired single set
MapObjActorInitInfo::MapObjActorInitInfo() {
    mSetDefaultPosition = 0;
    mConnectToScene = 0;
    mInitBinder = 0;
    mHasEffect = 0;
    mHasSensors = 0;
    mHasShadows = 0; 
    mCalcGravity = 0;
    mProjectMapMtx = 0;
    mInitFur = 0;
    mHasRailMover = 0;
    mHasRotator = 0;
    mHasRailRotator = 0; 
    _C = 0;
    _D = 0; 
    mUsesRailPosture = 0;
    mDoesBaseMtxFollowTarget = 0;
    mIsAffectedByScale = 0;
    mUseMirrorReflection = 0;
    mModelName = 0;
    mHioNode = 0; 
    _1C = 0;
    mSensorSize = 0.0f;
    mSensorOffset.zero();
    mHitSensorCB = 0;
    mBinderRadius = 0.0f;
    mBinderCenterY = 0.0f;
    mEffectName = 0;
    mSound = 0;
    mSoundPos = 0;
    _48 = 0;
    mNerve = 0;
    mGroupClipping = 0;
    mClippingRadius = 0.0f;
    mFarClipping = 0.0f;
    _5C = -1;
    mColorChangeArg = -1;
    mTextureChangeArg = -1;
    mNoAppearRiddleSE = 0;
    _6C = 0;
    _70 = 0.0f;
    _74 = 0;
    mShadowName = 0;
    mShadowLength = -1.0f;
    _80 = 0;
    mDummyChangeTexture = 0;
    _88 = -1;
    _8C = 0;
    mNoUseLOD = 0;
}

void MapObjActorInitInfo::setupHioNode(const char *pName) {
    mHioNode = pName;
}

void MapObjActorInitInfo::setupDefaultPos() {
    mSetDefaultPosition = true;
}

void MapObjActorInitInfo::setupModelName(const char *pName) {
    mModelName = pName;
}

void MapObjActorInitInfo::setupConnectToScene() {
    mConnectToScene = true;
}

void MapObjActorInitInfo::setupBinder(f32 binderRadius, f32 binderCenterY) {
    mBinderRadius = binderRadius;
    mInitBinder = true;
    mBinderCenterY = binderCenterY;
}

void MapObjActorInitInfo::setupEffect(const char *pName) {
    mEffectName = pName;
    mHasEffect = true;
}

void MapObjActorInitInfo::setupSound(s32 id) {
    mSound = id;
}

void MapObjActorInitInfo::setupSoundPos(TVec3f *pPos) {
    mSoundPos = pPos;
}

void MapObjActorInitInfo::setupNoAppearRiddleSE() {
    mNoAppearRiddleSE = false;
}

void MapObjActorInitInfo::setupHitSensor() {
    mHasSensors = true;
}
