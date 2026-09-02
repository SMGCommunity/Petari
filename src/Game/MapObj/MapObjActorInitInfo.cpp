#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/StringUtil.hpp"

MapObjActorInitInfo::MapObjActorInitInfo()
    : mSetDefaultPosition(), mConnectToScene(), mInitBinder(), mHasEffect(), mHasSensors(), mHasShadows(), mCalcGravity(), mUseProjectMapMtx(),
      mInitFur(), mHasRailMover(), mHasRotator(), mHasRailRotator(), _C(), _D(), mUseRailPosture(), mUseBaseMtxFollowTarget(), mIsAffectedByScale(),
      mUseMirrorReflection(), mModelName(), mHioNode(), _1C(), mSensorRadius(), mSensorPosition(gZeroVec), mHasHitSensorCB(), mBinderRadius(),
      mBinderCenterY(), mEffectName(), mSound(), mSoundPos(), _48(), mNerve(), mGroupClipping(), mClippingRadius(), mFarClipping(), _5C(-1),
      mColorChangeArg(-1), mTextureChangeArg(-1), mIsAppearRiddleSE(), mSeesaw1AxisRotatorName(), mSeesaw1AxisRotatorRadius(), _74(), mShadowName(),
      mShadowLength(-1.0f), _80(), mDummyChangeTexture(), _88(-1), _8C(), mNoUseLOD() {
}

void MapObjActorInitInfo::setupHioNode(const char* pName) {
    mHioNode = pName;
}

void MapObjActorInitInfo::setupDefaultPos() {
    mSetDefaultPosition = true;
}

void MapObjActorInitInfo::setupModelName(const char* pName) {
    mModelName = pName;
}

void MapObjActorInitInfo::setupConnectToScene() {
    mConnectToScene = true;
}

void MapObjActorInitInfo::setupBinder(f32 radius, f32 centerY) {
    mInitBinder = true;
    mBinderRadius = radius;
    mBinderCenterY = centerY;
}

void MapObjActorInitInfo::setupEffect(const char* pName = nullptr) {
    mHasEffect = true;
    mEffectName = pName;
}

void MapObjActorInitInfo::setupSound(s32 id) {
    mSound = id;
}

void MapObjActorInitInfo::setupSoundPos(TVec3f* pPos) {
    mSoundPos = pPos;
}

void MapObjActorInitInfo::setupNoAppearRiddleSE() {
    mIsAppearRiddleSE = false;
}

void MapObjActorInitInfo::setupHitSensor() {
    mHasSensors = true;
}

void MapObjActorInitInfo::setupHitSensorCallBack() {
    mHasSensors = true;
    mHasHitSensorCB = true;
}

void MapObjActorInitInfo::setupHitSensorParam(u16 a1, f32 radius, const TVec3f& rPosition) {
    _1C = a1;
    mSensorRadius = radius;
    mSensorPosition.set(rPosition);
}

void MapObjActorInitInfo::setupNerve(const Nerve* pNerve) {
    mNerve = pNerve;
}

void MapObjActorInitInfo::setupShadow(const char* pName = nullptr) {
    mHasShadows = true;
    mShadowName = pName;
}

void MapObjActorInitInfo::setupGroupClipping(s32 id) {
    mGroupClipping = id;
}

void MapObjActorInitInfo::setupClippingRadius(f32 radius) {
    mClippingRadius = radius;
}

void MapObjActorInitInfo::setupFarClipping(f32 distance = -1.0f) {
    mFarClipping = distance;
}

void MapObjActorInitInfo::setupProjmapMtx(bool b1) {
    mUseProjectMapMtx = true;
    _48 = b1;
}

void MapObjActorInitInfo::setupRailMover() {
    mHasRailMover = true;
}

void MapObjActorInitInfo::setupRotator() {
    mHasRotator = true;
}

void MapObjActorInitInfo::setupRailRotator() {
    mHasRailRotator = true;
}

void MapObjActorInitInfo::setupSeesaw1AxisRotator(const char* pName, f32 radius) {
    _C = true;
    mSeesaw1AxisRotatorName = pName;
    mSeesaw1AxisRotatorRadius = radius;
}

void MapObjActorInitInfo::setupRailPosture() {
    mUseRailPosture = true;
}

void MapObjActorInitInfo::setupBaseMtxFollowTarget() {
    mUseBaseMtxFollowTarget = true;
}

void MapObjActorInitInfo::setupAffectedScale() {
    mIsAffectedByScale = true;
}

void MapObjActorInitInfo::setupSeAppear() {
    mIsAppearRiddleSE = true;
}

void MapObjActorInitInfo::setupMirrorReflection(bool b1) {
    _74 = b1;
    mUseMirrorReflection = true;
}

void MapObjActorInitInfo::setupPrepareChangeDummyTexture(const char* pName) {
    mDummyChangeTexture = pName;
}

void MapObjActorInitInfo::setupNoUseLodCtrl() {
    mNoUseLOD = true;
}

void MapObjActorUtil::setupInitInfoTypical(MapObjActorInitInfo* pInitInfo, const char* pName) {
    const char* const cNoAppearRiddleSENameList[] = {"AsteroidMoveA",
                                                     "HeavensDoorAppearStepA",
                                                     "HeavensDoorAppearStepAAfter",
                                                     "HeavensDoorInsideRotatePartsA",
                                                     "HeavensDoorInsideRotatePartsB",
                                                     "HeavensDoorInsideRotatePartsC",
                                                     "PeachCastleTownAfterAttack",
                                                     "PeachCastleTownAfterAttack",
                                                     "PeachCastleTownBeforeAttack",
                                                     "PhantomShipPropellerBig",
                                                     "PhantomShipPropellerSmall",
                                                     "StarPieceCluster",
                                                     "RosettaChair",
                                                     "AstroDomeDemoAstroGalaxy",
                                                     "ChallengeBallMoveGroundB",
                                                     "ChallengeBallVanishingRoadA",
                                                     "DarkHopperRotateStepA",
                                                     "DarkHopperPlanetA",
                                                     "PeachCastleTownGate",
                                                     "KoopaShipE",
                                                     "BattleShipElevatorCover",
                                                     "StrongBlock"};
    if (MR::isExistString(pName, cNoAppearRiddleSENameList, ARRAY_SIZE(cNoAppearRiddleSENameList))) {
        pInitInfo->setupNoAppearRiddleSE();
    }

    if (MR::isEqualString("HeavensDoorInsidePlanet", pName)) {
        pInitInfo->setupProjmapMtx(false);
    }

    const char* const cUfoSandObstaclesList[] = {"UFOSandObstacleA", "UFOSandObstacleB", "UFOSandObstacleC"};
    if (MR::isExistString(pName, cUfoSandObstaclesList, ARRAY_SIZE(cUfoSandObstaclesList))) {
        pInitInfo->mShadowName = "Shadow";
    }

    const char* const cKoopaVs2PartsList[] = {"KoopaVS2PartsJoinedMoveStep", "KoopaVS2PartsSquareMoveStepA", "KoopaVS2PartsSquareMoveStepB",
                                              "KoopaVS2PartsNarrowRoad", "KoopaVS2PartsClipAreaDisplayA"};
    if (MR::isExistString(pName, cKoopaVs2PartsList, ARRAY_SIZE(cKoopaVs2PartsList))) {
        pInitInfo->setupHitSensor();
    }

    if (MR::isEqualString("SandUpDownTowerBreakableWallA", pName) || MR::isEqualString("SandUpDownTowerBreakableWallB", pName)) {
        pInitInfo->_80 = "SandUpDownTowerBreakableWallBreak";
    }

    if (MR::isEqualString("KoopaJrNormalShipA", pName)) {
        pInitInfo->_88 = 34;
    }

    if (MR::isEqualString("KoopaStatue", pName)) {
        pInitInfo->_8C = true;
        pInitInfo->_5C = 2;
    }

    if (MR::isEqualString("DangerSignBoard", pName)) {
        pInitInfo->setupHitSensor();
        pInitInfo->setupHitSensorParam(4, 200.0f, TVec3f(0.0f, 200.0f, 0.0f));
    }
}

void MapObjActorUtil::setupInitInfoColorChangeArg0(MapObjActorInitInfo* pInitInfo, const JMapInfoIter& rIter) {
    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    pInitInfo->mColorChangeArg = arg0;
}

void MapObjActorUtil::setupInitInfoTextureChangeArg1(MapObjActorInitInfo* pInitInfo, const JMapInfoIter& rIter) {
    s32 arg1 = -1;
    MR::getJMapInfoArg1NoInit(rIter, &arg1);
    pInitInfo->mTextureChangeArg = arg1;
}

void MapObjActorUtil::setupInitInfoShadowLengthArg2(MapObjActorInitInfo* pInitInfo, const JMapInfoIter& rIter) {
    f32 arg2 = -1.0f;
    MR::getJMapInfoArg2NoInit(rIter, &arg2);
    pInitInfo->mShadowLength = arg2;
}

void MapObjActorUtil::setupInitInfoSeesaw(MapObjActorInitInfo* pInitInfo, const JMapInfoIter& rIter, const char* pName, f32 radius) {
    s32 arg = -1;
    MR::getMapPartsArgRotateAxis(&arg, rIter);

    if (arg == 0) {
        pInitInfo->setupSeesaw1AxisRotator(pName, radius);
    } else {
        pInitInfo->_D = true;
        pInitInfo->mSeesaw1AxisRotatorName = pName;
        pInitInfo->mSeesaw1AxisRotatorRadius = radius;
    }
}

void MapObjActorUtil::setupInitInfoSimpleMapObj(MapObjActorInitInfo* pInitInfo) {
    pInitInfo->setupHioNode("地形オブジェ");
    pInitInfo->setupDefaultPos();
    pInitInfo->setupConnectToScene();
    pInitInfo->setupEffect();
    pInitInfo->setupSound(4);
    pInitInfo->setupShadow();
    pInitInfo->setupGroupClipping(64);
    pInitInfo->setupSeAppear();
}

void MapObjActorUtil::setupInitInfoPlanet(MapObjActorInitInfo* pInitInfo) {
    pInitInfo->setupHioNode("惑星");
    pInitInfo->setupDefaultPos();
    pInitInfo->setupConnectToScene();
    pInitInfo->setupEffect();
    pInitInfo->setupFarClipping();
    pInitInfo->setupNoAppearRiddleSE();
}
