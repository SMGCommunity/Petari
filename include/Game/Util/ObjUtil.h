#pragma once

#include "Game/Util/Functor.h"

class NameObj;
class LayoutActor;
class LiveActor;
class JMapInfo;
class ResourceHolder;
class StageSwitchCtrl;

namespace MR {
    void connectToScene(NameObj *, int, int, int, int);
    void connectToSceneCollisionMapObj(LiveActor *);
    void connectToSceneCollisionMapObjMovementCalcAnim(LiveActor *);
    void connectToSceneCollisionMapObjWeakLight(LiveActor *);
    void connectToSceneCollisionMapObjStrongLight(LiveActor *);
    void connectToSceneCollisionEnemy(LiveActor *);
    void connectToSceneCollisionEnemyMovement(NameObj *);
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

    void connectToSceneCamera(NameObj *);

    void connectToSceneAreaObj(NameObj *);

    void connectToSceneMirrorMapObj(LiveActor *);

    void connectToSceneSky(LiveActor *);
    
    bool isName(const NameObj *, const char *);
    bool isSame(const NameObj *, const NameObj *);

    bool isStageStateScenarioOpeningCamera();

    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
    
    const JMapInfo* createCsvParser(const char *, const char *, ...);
    JMapInfo* tryCreateCsvParser(const ResourceHolder *, const char *, ...);
    s32 getCsvDataElementNum(const JMapInfo *);

    void getCsvDataS32(s32 *, const JMapInfo *, const char *, s32);
    void getCsvDataStr(const char **, const JMapInfo *, const char *, s32);
    void getCsvDataBool(bool *, const JMapInfo *, const char *, s32);

    ResourceHolder* createAndAddResourceHolder(const char *);

    void listenNameObjStageSwitchOnOffAppear(const NameObj *, const StageSwitchCtrl *, const MR::FunctorBase &, const MR::FunctorBase &);

    void requestMovementOn(LiveActor *);

    void requestMovementOn(LayoutActor *);

    void registerPreDrawFunction(const MR::FunctorBase &, int);

    bool isJudgedToClipFrustum100m(const TVec3f &, f32);
    bool isJudgedToClipFrustum300m(const TVec3f &, f32);

    bool tryRumblePadMiddle(const void *, s32);

    void declarePowerStar(const NameObj *);
    void declarePowerStar(const NameObj *, s32);

    void appearPowerStarWithoutDemo(const NameObj *);
    void requestAppearPowerStar(const NameObj *);
    void requestAppearPowerStar(const NameObj *, const TVec3f &);
    void requestAppearPowerStar(const NameObj *, s32, const TVec3f &);
    void requestAppearPowerStar(const NameObj *, const LiveActor *, f32);
};
