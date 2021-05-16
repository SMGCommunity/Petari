#include "Util/ObjUtil.h"
#include "NameObj/NameObjExecuteHolder.h"

namespace MR
{
    void connectToScene(LiveActor *pActor, s32 movementType, s32 calcAnimType, s32 drawBufType, s32 drawType)
    {
        MR::registerNameObjToExecuteHolder(pActor, movementType, calcAnimType, drawBufType, drawType);
    }

    void connectToScene(NameObj *pActor, s32 movementType, s32 calcAnimType, s32 drawBufType, s32 drawType)
    {
        MR::registerNameObjToExecuteHolder(pActor, movementType, calcAnimType, drawBufType, drawType);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);
    }

    void connectToSceneCollisionMapObj(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x1E, 2, 8, -1);
    }

    void connectToSceneCollisionMapObjMovementCalcAnim(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x1E, 2, -1, -1);
    }

    void connectToSceneCollisionMapObjWeakLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x1E, 2, 9, -1);
    }

    void connectToSceneCollisionMapObjStrongLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x1E, 2, 0xA, -1);
    }

    void connectToSceneCollisionEnemy(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x1F, 3, 0x12, -1);
    }

    void connectToSceneCollisionEnemyMovement(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x1F, -1, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);
    }

    void connectToSceneCollisionEnemyStrongLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x1F, 3, 0xA, -1);
    }

    void connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x1F, 3, 0xC, -1);
    }

    void connectToSceneNpc(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x28, 6, 0x10, -1);
    }

    void connectToSceneNpcMovement(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x28, -1, -1, -1);
    }

    void connectToSceneRide(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x29, 7, 0x11, -1);
    }

    void connectToSceneEnemy(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x2A, 8, 0x12, -1);
    }
};