#ifndef OBJUTIL_H
#define OBJUTIL_H

#include "Actor/LiveActor/LiveActor.h"
#include "Actor/NameObj/NameObj.h"
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

    void requestMovementOn(NameObj *);
    void requestMovementOff(NameObj *);
    void notifyRequestNameObjMovementOnOff();

    ResourceHolder* createAndAddResourceHolder(const char *);
    void* loadResourceFromArc(const char *, const char *);
};

#endif // OBJUTIL_H