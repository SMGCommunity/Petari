#include "Util/ObjUtil.h"
#include "Util/LiveActorUtil.h"
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

    void connectToSceneEnemyMovement(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x2A, -1, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);
    }

    void connectToSceneMapObj(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 5, 8, -1);
    }

    void connectToSceneMapObjMovement(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, -1, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);
    }

    void connectToSceneMapObjMovementCalcAnim(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 5, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);
    }

    void connectToSceneMapObjNoMovement(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, -1, 5, 8, -1);
    }

    void connectToSceneMapObjNoCalcAnim(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, -1, 8, -1);
    }

    void connectToSceneMapObjNoCalcAnimStrongLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, -1, 0xA, -1);
    }

    void connectToSceneMapObjDecoration(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x23, 0xB, 8, -1);
    }

    void connectToSceneMapObjDecorationStrongLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x23, 0xB, 0xA, -1);
    }

    void connectToSceneMapObjDecorationMovement(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x23, -1, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);
    }

    void connectToSceneMapObjStrongLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 5, 0xA, -1);
    }

    void connectToSceneMapParts(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x1C, 0, 8, -1);
    }

    void connectToScenePlanet(LiveActor *pActor)
    {
        if (MR::isExistIndirectTexture(pActor))
        {
            MR::registerNameObjToExecuteHolder(pActor, 0x1D, 1, 0x1D, -1);
        }
        else
        {
            MR::registerNameObjToExecuteHolder(pActor, 0x1D, 1, 4, -1);
        }
    } 

    void connectToSceneEnvironment(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x21, 4, 6, -1);
    }

    void connectToSceneEnvironmentStrongLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x21, 4, 7, -1);
    }

    void connectToClippedMapParts(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x1C, 0, 0, -1);
    }

    void connectToSceneEnemyDecoration(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x2B, 0xB, 0x13, -1);
    }

    void connectToSceneEnemyDecorationMovement(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x2B, -1, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);
    }

    void connectToSceneEnemyDecorationMovementCalcAnim(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x2B, 0xB, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);
    }

    void connectToSceneItem(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x2C, 0x10, 0xD, -1);
    }

    void connectToSceneIndirectEnemy(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x2A, 8, 0x1C, -1);
    }

    void connectToSceneIndirectNpc(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x28, 6, 0x1B, -1);
    }

    void connectToSceneIndirectMapObjStrongLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 5, 0x1A, -1);
    }

    void connectToSceneScreenEffectMovement(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 3, -1, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);        
    }

    void connectToSceneAreaObj(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 3, -1, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);           
    }

    void connectToScene3DModelFor2D(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0xE, 0xD, 0x24, -1);
    }

    void connectToSceneLayout(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0xE, 0xD, -1, 0x3C);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);               
    }

    void connectToSceneLayout(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0xE, 0xD, -1, 0x3C);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);               
    }

    void connectToSceneLayoutMovementCalcAnim(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0xE, 0xD, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);            
    }

    void connectToSceneLayoutDecoration(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0xF, 0xE, -1, 0x3D);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);             
    }

    void connectToSceneTalkLayout(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0xE, 0xD, -1, 0x43);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);            
    }

    void connectToSceneTalkLayoutNoMovement(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, -1, 0xD, -1, 0x43);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);            
    }

    void connectToSceneWipeLayout(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x11, 0xD, -1, 0x42);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);             
    }

    void connectToSceneLayoutOnPause(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x12, 0xD, -1, 0x3E);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);           
    }

    void connectToSceneLayoutOnPauseNoMovement(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, -1, 0xD, -1, 0x3E);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);          
    }
    
    void connectToSceneLayoutOnPauseMovementCalcAnim(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x12, 0xD, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);              
    }

    void connectToSceneLayoutMovement(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0xE, -1, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);             
    }

    void connectToSceneMovie(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x10, -1, -1, 0x3F);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);              
    }

    void connectToSceneMirrorMapObj(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 0xC, 0x27, -1);
    }

    void connectToSceneMirrorMapObjDecoration(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x23, 0xC, 0x27, -1);
    }

    void connectToSceneMirrorMapObjNoMovement(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, -1, 0xC, 0x27, -1);
    }

    void connectToSceneCamera(NameObj *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 2, -1, -1, -1);
        MR::connectToSceneTemporarily(pActor);
        MR::connectToDrawTemporarily(pActor);
    }

    void connectToSceneNoShadowedMapObj(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 5, 0xB, -1);
    }

    void connectToSceneNoShadowedMapObjStrongLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 5, 0xC, -1);
    }

    void connectToSceneNoSilhouettedMapObj(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 5, 0xD, -1);
    }

    void connectToSceneNoSilhouettedMapObjStrongLight(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 5, 0xF, -1);
    }

    void connectToSceneNoSilhouettedMapObjWeakLightNoMovement(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, -1, 5, 0xE, -1);
    }

    void connectToSceneSky(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x24, 5, 1, -1);
    }

    void connectToSceneAir(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x24, 5, 2, -1);
    }

    void connectToSceneSun(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x24, 5, 3, -1);
    }

    void connectToSceneCrystal(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 5, 0x20, -1);
    }

    void connectToSceneNormalMapObj(LiveActor *pActor)
    {
        MR::registerNameObjToExecuteHolder(pActor, 0x22, 5, -1, 0x18);
    }

    void requestMovementOn(NameObj *pActor)
    {
        NameObjFunction::requestMovementOn(pActor);
    }

    void requestMovementOff(NameObj *pActor)
    {
        NameObjFunction::requestMovementOff(pActor);
    }
};