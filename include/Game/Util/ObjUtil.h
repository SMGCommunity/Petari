#pragma once

#include "Game/Util/Functor.h"

class NameObj;
class LayoutActor;
class LiveActor;
class JMapInfo;
class ResourceHolder;
class StageSwitchCtrl;

class ResTIMG;

namespace MR {
    void connectToScene(NameObj *, int, int, int, int);
    void connectToSceneCollisionMapObj(LiveActor *);
    void connectToSceneCollisionMapObjMovementCalcAnim(LiveActor *);
    void connectToSceneCollisionMapObjWeakLight(LiveActor *);
    void connectToSceneCollisionMapObjStrongLight(LiveActor *);
    void connectToSceneCollisionEnemy(LiveActor *);
    void connectToSceneCollisionEnemyMovement(NameObj *);
    void connectToSceneCollisionEnemyStrongLight(LiveActor *);
    void connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(LiveActor *);
    void connectToSceneNpc(LiveActor *);
    void connectToSceneNpcMovement(LiveActor *);
    void connectToSceneRide(LiveActor *);
    void connectToSceneEnemy(LiveActor *);
    void connectToSceneEnemyMovement(NameObj *);
    void connectToSceneMapObj(LiveActor *);
    void connectToSceneMapObjMovement(NameObj *);
    void connectToSceneMapObjMovementCalcAnim(NameObj *);
    void connectToSceneMapObjNoMovement(LiveActor *);
    void connectToSceneMapObjNoCalcAnim(LiveActor *);
    void connectToSceneMapObjNoCalcAnimStrongLight(LiveActor *);
    void connectToSceneMapObjDecoration(LiveActor *);
    void connectToSceneMapObjDecorationStrongLight(LiveActor *);
    void connectToSceneMapObjDecorationMovement(NameObj *);
    void connectToSceneMapObjStrongLight(LiveActor *);
    void connectToSceneMapParts(LiveActor *);
    void connectToScenePlanet(LiveActor *);
    void connectToSceneEnvironment(LiveActor *);
    void connectToSceneEnvironmentStrongLight(LiveActor *);
    void connectToClippedMapParts(LiveActor *);
    void connectToSceneEnemyDecoration(LiveActor *);
    void connectToSceneItem(LiveActor *);
    void connectToSceneItemStrongLight(LiveActor *);
    void connectToSceneIndirectEnemy(LiveActor *);
    void connectToSceneIndirectNpc(LiveActor *);
    void connectToSceneIndirectMapObj(LiveActor *);
    void connectToSceneIndirectMapObjStrongLight(LiveActor *);
    void connectToSceneAreaObj(NameObj *);

    void connectToSceneSun(LiveActor *);

    void connectToSceneNoSilhouettedMapObjStrongLight(LiveActor *);

    void connectToSceneLayout(NameObj *);

    void connectToSceneCamera(NameObj *);

    void connectToSceneAir(LiveActor *);

    void connectToSceneAreaObj(NameObj *);

    void connectToSceneMirrorMapObj(LiveActor *);

    void connectToSceneSky(LiveActor *);

    void connectToSceneNoSilhouettedMapObj(LiveActor *);

    void connectToSceneNoSilhouettedMapObjWeakLightNoMovement(LiveActor *);

    void connectToSceneCrystal(LiveActor *);

    void connectToSceneMirrorMapObjNoMovement(LiveActor *);

    void connectToSceneMirrorMapObjDecoration(LiveActor *);

    void connectToSceneNoShadowedMapObjStrongLight(LiveActor *);
    
    bool isName(const NameObj *, const char *);
    bool isSame(const NameObj *, const NameObj *);

    bool isStageStateScenarioOpeningCamera();

    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
    
    JMapInfo* createCsvParser(const char *, const char *, ...);
    JMapInfo* tryCreateCsvParser(const ResourceHolder *, const char *, ...);
    s32 getCsvDataElementNum(const JMapInfo *);

    void getCsvDataS32(s32 *, const JMapInfo *, const char *, s32);
    void getCsvDataStr(const char **, const JMapInfo *, const char *, s32);
    void getCsvDataBool(bool *, const JMapInfo *, const char *, s32);

    ResourceHolder* createAndAddResourceHolder(const char *);

    void listenNameObjStageSwitchOnAppear(const NameObj *, const StageSwitchCtrl *, const MR::FunctorBase &);
    void listenNameObjStageSwitchOnOffAppear(const NameObj *, const StageSwitchCtrl *, const MR::FunctorBase &, const MR::FunctorBase &);

    void listenNameObjStageSwitchOnOffA(const NameObj *, const StageSwitchCtrl *, const MR::FunctorBase &, const MR::FunctorBase &);

    void listenNameObjStageSwitchOnB(const NameObj *, const StageSwitchCtrl *, const MR::FunctorBase &);

    void requestMovementOn(LiveActor *);

    void requestMovementOn(LayoutActor *);

    void registerPreDrawFunction(const MR::FunctorBase &, int);

    bool isJudgedToClipFrustum(const TVec3f &, f32);
    bool isJudgedToClipFrustum100m(const TVec3f &, f32);
    bool isJudgedToClipFrustum300m(const TVec3f &, f32);

    bool tryRumblePad(const void *, const char *, s32);
    bool tryRumblePadVeryStrongLong(const void *, s32);
    bool tryRumblePadWeak(const void *, s32);
    bool tryRumblePadVeryWeak(const void *, s32);
    bool tryRumblePadMiddle(const void *, s32);
    bool tryRumblePadStrong(const void *, s32);
    bool tryRumblePadVeryStrong(const void *, s32);

    bool tryRumbleDefaultHit(const void *, s32);

    void declareCoin(const NameObj *, s32);
    void getDeclareRemnantCoinCount(const NameObj *);
    void hopCoin(const NameObj *, const TVec3f &, const TVec3f &);
    void appearCoinFix(const NameObj *, const TVec3f &, s32);

    void appearCoinPop(const NameObj *, const TVec3f &, s32);
    void appearCoinPopToDirection(const NameObj *, const TVec3f &, const TVec3f &, s32);

    void appearCoinToVelocity(const NameObj *, const TVec3f &, const TVec3f &, s32);

    void declareStarPiece(const NameObj *, s32);
    s32 getDeclareRemnantStarPieceCount(const NameObj *);
    s32 getDeclareRemnantStarPieceCountNotExist(const NameObj *);
    void declareStarPieceReceiver(const NameObj *, s32);
    void clearGotCountStarPieceReceiver(const NameObj *);
    void hopStarPiece(const NameObj *, const TVec3f &, const TVec3f &);
    bool appearStarPiece(const NameObj *, const TVec3f &, s32, f32, f32, bool);
    void appearStarPieceToDirection(const NameObj *, const TVec3f &, const TVec3f &, s32, f32, f32, bool);

    void declarePowerStar(const NameObj *);
    void declarePowerStar(const NameObj *, s32);
    void declarePowerStarCoin100();
    void appearPowerStarContinueCurrentDemo(const NameObj *, const TVec3f &);
    void appearPowerStarWithoutDemo(const NameObj *);
    void requestAppearPowerStar(const NameObj *);
    void requestAppearPowerStar(const NameObj *, const TVec3f &);
    void requestAppearPowerStar(const NameObj *, s32, const TVec3f &);
    void requestAppearPowerStar(const NameObj *, const LiveActor *, f32);

    void shakeCameraWeak();
    void shakeCameraNormal();
    void shakeCameraStrong();
    void shakeCameraVeryStrong();
    void shakeCameraInfinity(NameObj *, f32, f32);

    void stopShakingCamera(NameObj *);

    void findNamePos(const char *, TVec3f *, TVec3f *);

    bool isInWater(const TVec3f &);
    bool isInDeath(const TVec3f &);
    bool isInDarkMatter(const TVec3f &);

    void stopScene(s32);
    void stopSceneForDefaultHit(s32);

    const ResTIMG* loadTexFromArc(const char *, const char *);

    void joinToNameObjGroup(NameObj *, const char *);

    bool isEndPowerStarAppearDemo(const NameObj *);

    bool tryRegisterNamePosLinkObj(const NameObj *, const JMapInfoIter &);

    bool tryFindLinkNamePos(const NameObj *, const char *, MtxPtr);
};
