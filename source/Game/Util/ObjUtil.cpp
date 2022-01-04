#include "Game/Util.h"
#include "Game/NameObj/NameObjExecuteHolder.h"

namespace MR {
    void connectToScene(LiveActor *pActor, int a2, int a3, int a4, int a5) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), a2, a3, a4, a5);
    }

    void connectToScene(NameObj *pObj, int a2, int a3, int a4, int a5) {
        MR::registerNameObjToExecuteHolder(pObj, a2, a3, a4, a5);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneCollisionMapObj(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, 8, -1);
    }

    void connectToSceneCollisionMapObjMovementCalcAnim(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, -1, -1);
    }

    void connectToSceneCollisionMapObjWeakLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, 9, -1);
    }

    void connectToSceneCollisionMapObjStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1E, 2, 0xA, -1);
    }

    void connectToSceneCollisionEnemy(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1F, 3, 0x12, -1);
    }

    void connectToSceneCollisionEnemyMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x1F, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneCollisionEnemyStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1F, 3, 0xA, -1);
    }

    void connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1F, 3, 0xC, -1);
    }

    void connectToSceneNpc(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x28, 6, 0x10, -1);
    }

    void connectToSceneNpcMovement(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x28, -1, -1, -1);
    }

    void connectToSceneRide(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x29, 7, 0x11, -1);
    }

    void connectToSceneEnemy(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2A, 8, 0x12, -1);
    }

    void connectToSceneEnemyMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x2A, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObj(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 8, -1);
    }

    void connectToSceneMapObjMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x22, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObjMovementCalcAnim(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x22, 5, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObjNoMovement(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), -1, 5, 8, -1);
    }

    void connectToSceneMapObjNoCalcAnim(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, -1, 8, -1);
    }

    void connectToSceneMapObjNoCalcAnimStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, -1, 0xA, -1);
    }

    void connectToSceneMapObjDecoration(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x23, 0xB, 8, -1);
    }

    void connectToSceneMapObjDecorationStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x23, 0xB, 0xA, -1);
    }

    void connectToSceneMapObjDecorationMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x23, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneMapObjStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 0xA, -1);
    }

    void connectToSceneMapParts(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1C, 0, 8, -1);
    }

    void connectToScenePlanet(LiveActor *pActor) {
        if (MR::isExistIndirectTexture(pActor)) {
            MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1D, 1, 0x1D, -1);
        }
        else {
            MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1D, 1, 4, -1);
        }
    }

    void connectToSceneEnvironment(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x21, 4, 6, -1);
    }

    void connectToSceneEnvironmentStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x21, 4, 7, -1);
    }

    void connectToClippedMapParts(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x1C, 0, 0, -1);
    }

    void connectToSceneEnemyDecoration(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2B, 0xB, 0x13, -1);
    }

    void connectToSceneEnemyDecorationMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x2B, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneEnemyDecorationMovementCalcAnim(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0x2B, 0x11, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneItem(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2C, 0x10, 0xD, -1);
    }

    void connectToSceneItemStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2C, 0x10, 0xF, -1);
    }

    void connectToSceneIndirectEnemy(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x2A, 8, 0x1C, -1);
    }

    void connectToSceneIndirectNpc(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x28, 6, 0x1B, -1);
    }

    void connectToSceneIndirectMapObj(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 0x19, -1);
    }

    void connectToSceneIndirectMapObjStrongLight(LiveActor *pActor) {
        MR::registerNameObjToExecuteHolder(reinterpret_cast<NameObj*>(pActor), 0x22, 5, 0x1A, -1);
    }

    void connectToSceneScreenEffectMovement(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 3, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneAreaObj(NameObj *pObj) {
        MR::registerNameObjToExecuteHolder(pObj, 0xD, -1, -1, -1);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }
};