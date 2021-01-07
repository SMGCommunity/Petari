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
    void connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(LiveActor *actor);
    void connectToSceneNpc(LiveActor *);
    void connectToSceneNpcMovement(LiveActor *);
    void connectToSceneRide(LiveActor *);
    void connectToSceneEnemy(LiveActor *);

    void connectToSceneMapObjMovement(NameObj *);

    void connectToSceneMapObjDecoration(LiveActor *);

    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
    void notifyRequestNameObjMovementOnOff();

    ResourceHolder* createAndAddResourceHolder(const char *);
    void* loadResourceFromArc(const char *, const char *);

    void appearCoinPopToDirection(const NameObj *, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, s32);
    void appearStarPieceToDirection(const NameObj *, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, s32, f32, f32, bool);
};