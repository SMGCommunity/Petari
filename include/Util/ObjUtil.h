#pragma once

#include "LiveActor/LiveActor.h"
#include "NameObj/NameObj.h"
#include "System/Resource/ResourceHolder.h"

namespace MR
{
    void connectToScene(LiveActor *, s32, s32, s32, s32);
    void connectToScene(NameObj *, s32, s32, s32, s32);
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
    void connectToSceneEnemyDecoration(LiveActor *);
    void connectToSceneEnemyDecorationMovement(NameObj *);
    void connectToSceneEnemyDecorationMovementCalcAnim(NameObj *);
    void connectToSceneItem(LiveActor *);
    void connectToSceneItemStrongLight(LiveActor *);
    void connectToSceneIndirectEnemy(LiveActor *);
    void connectToSceneIndirectNpc(LiveActor *);
    void connectToSceneIndirectMapObj(LiveActor *);
    void connectToSceneIndirectMapObjStrongLight(LiveActor *);
    void connectToSceneScreenEffectMovement(NameObj *);
    void connectToSceneAreaObj(NameObj *);
    void connectToScene3DModelFor2D(LiveActor *);
    void connectToSceneLayout(NameObj *);
    void connectToSceneLayoutMovementCalcAnim(NameObj *);
    void connectToSceneLayoutDecoration(NameObj *);
    void connectToSceneTalkLayout(NameObj *);
    void connectToSceneTalkLayoutNoMovement(NameObj *);
    void connectToSceneWipeLayout(NameObj *);
    void connectToSceneLayoutOnPause(NameObj *);
    void connectToSceneLayoutOnPauseNoMovement(NameObj *);
    void connectToSceneLayoutOnPauseMovementCalcAnim(NameObj *);
    void connectToSceneLayoutMovement(NameObj *);
    void connectToSceneMovie(NameObj *);
    void connectToSceneMirrorMapObj(LiveActor *);
    void connectToSceneMirrorMapObjDecoration(LiveActor *);
    void connectToSceneMirrorMapObjNoMovement(LiveActor *);
    void connectToSceneCamera(NameObj *);
    void connectToSceneNoShadowedMapObj(LiveActor *);
    void connectToSceneNoShadowedMapObjStrongLight(LiveActor *);
    void connectToSceneNoSilhouettedMapObj(LiveActor *);
    void connectToSceneNoSilhouettedMapObjStringLight(LiveActor *);
    void connectToSceneNoSilhouettedMapObjNoMovement(LiveActor *);
    void connectToSceneSky(LiveActor *);
    void connectToSceneAir(LiveActor *);
    void connectToSceneSun(LiveActor *);
    void connectToSceneCrystal(LiveActor *);
    void connectToSceneNormalMapObj(LiveActor *);

    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
    void notifyRequestNameObjMovementOnOff();

    ResourceHolder* createAndAddResourceHolder(const char *);
    void* loadResourceFromArc(const char *, const char *);

    void appearCoinPopToDirection(const NameObj *, const JGeometry::TVec3f &, const JGeometry::TVec3f &, s32);
    void appearStarPieceToDirection(const NameObj *, const JGeometry::TVec3f &, const JGeometry::TVec3f &, s32, f32, f32, bool);
};