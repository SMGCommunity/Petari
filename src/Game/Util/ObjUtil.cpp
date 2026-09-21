#include "Game/Util/ObjUtil.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraShaker.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/LiveActor/ClippingJudge.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Map/NamePosHolder.hpp"
#include "Game/Map/SwitchWatcherHolder.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/MapObj/BenefitItemLifeUp.hpp"
#include "Game/MapObj/BenefitItemOneUp.hpp"
#include "Game/MapObj/CoinHolder.hpp"
#include "Game/MapObj/PowerStar.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/NPC/EventDirector.hpp"
#include "Game/NPC/StageStateKeeper.hpp"
#include "Game/NameObj/MovementOnOffGroupHolder.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjAdaptor.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjFinder.hpp"
#include "Game/NameObj/NameObjListExecutor.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Scene/StopSceneController.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/System/WPadRumble.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/SwitchEventFunctorListener.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TUtil.hpp>
#include <cstdio>
#include <va_list.h>

namespace {
    static const TVec3f sDefaultPlayerPos = TVec3f(0, 0, 0);
};  // namespace

bool MR::isJudgedToClipFrustum(const TVec3f& rParam1, f32 param2) {
    return MR::getClippingJudge()->isJudgedToClipFrustum(rParam1, param2);
}

bool MR::isJudgedToClipFrustum100m(const TVec3f& rParam1, f32 param2) {
    return MR::getClippingJudge()->isJudgedToClipFrustum(rParam1, param2, 6);
}

bool MR::isJudgedToClipFrustum300m(const TVec3f& rParam1, f32 param2) {
    return MR::getClippingJudge()->isJudgedToClipFrustum(rParam1, param2, 4);
}

bool MR::isJudgedToNearClip(const TVec3f& rPos, f32 distance) {
    TVec3f camPos = MR::getCamPos();

    return rPos.distance(camPos) < distance;
}

bool MR::isInWater(const TVec3f& rPos) {
    WaterInfo waterInfo = WaterInfo();

    return MR::getWaterAreaObj(&waterInfo, rPos);
}

bool MR::isInDeath(const TVec3f& rPos) {
    return MR::isInAreaObj("DeathArea", rPos);
}

bool MR::isInDarkMatter(const TVec3f& rPos) {
    return MR::isInAreaObj("DarkMatterCube", rPos) || MR::isInAreaObj("DarkMatterCylinder", rPos);
}

void MR::connectToScene(LiveActor* pActor, int movementType, int calcAnimType, int drawBufferType, int drawType) {
    MR::registerNameObjToExecuteHolder(pActor, movementType, calcAnimType, drawBufferType, drawType);
}

void MR::connectToScene(NameObj* pObj, int movementType, int calcAnimType, int drawBufferType, int drawType) {
    MR::registerNameObjToExecuteHolder(pObj, movementType, calcAnimType, drawBufferType, drawType);
    MR::connectToSceneTemporarily(pObj);
    MR::connectToDrawTemporarily(pObj);
}

void MR::connectToSceneCollisionMapObj(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_CollisionMapObj, MR::CalcAnimType_CollisionMapObj, MR::DrawBufferType_MapObj, MR::DrawType_None);
}

void MR::connectToSceneCollisionMapObjMovementCalcAnim(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_CollisionMapObj, MR::CalcAnimType_CollisionMapObj, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneCollisionMapObjWeakLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_CollisionMapObj, MR::CalcAnimType_CollisionMapObj, MR::DrawBufferType_MapObjWeakLight,
                       MR::DrawType_None);
}

void MR::connectToSceneCollisionMapObjStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_CollisionMapObj, MR::CalcAnimType_CollisionMapObj, MR::DrawBufferType_MapObjStrongLight,
                       MR::DrawType_None);
}

void MR::connectToSceneCollisionEnemy(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_CollisionEnemy, MR::CalcAnimType_CollisionEnemy, MR::DrawBufferType_Enemy, MR::DrawType_None);
}

void MR::connectToSceneCollisionEnemyMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_CollisionEnemy, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneCollisionEnemyStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_CollisionEnemy, MR::CalcAnimType_CollisionEnemy, MR::DrawBufferType_MapObjStrongLight,
                       MR::DrawType_None);
}

void MR::connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_CollisionEnemy, MR::CalcAnimType_CollisionEnemy, MR::DrawBufferType_NoShadowedMapObjStrongLight,
                       MR::DrawType_None);
}

void MR::connectToSceneNpc(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_NPC, MR::CalcAnimType_NPC, MR::DrawBufferType_NPC, MR::DrawType_None);
}

void MR::connectToSceneNpcMovement(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_NPC, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneRide(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Ride, MR::CalcAnimType_Ride, MR::DrawBufferType_Ride, MR::DrawType_None);
}

void MR::connectToSceneEnemy(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Enemy, MR::CalcAnimType_Enemy, MR::DrawBufferType_Enemy, MR::DrawType_None);
}

void MR::connectToSceneEnemyMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_Enemy, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneMapObj(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_MapObj, MR::DrawType_None);
}

void MR::connectToSceneMapObjMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_MapObj, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneMapObjMovementCalcAnim(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneMapObjNoMovement(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_None, MR::CalcAnimType_MapObj, MR::DrawBufferType_MapObj, MR::DrawType_None);
}

void MR::connectToSceneMapObjNoCalcAnim(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_None, MR::DrawBufferType_MapObj, MR::DrawType_None);
}

void MR::connectToSceneMapObjNoCalcAnimStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_None, MR::DrawBufferType_MapObjStrongLight, MR::DrawType_None);
}

void MR::connectToSceneMapObjDecoration(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObjDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_MapObj, MR::DrawType_None);
}

void MR::connectToSceneMapObjDecorationStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObjDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_MapObjStrongLight,
                       MR::DrawType_None);
}

void MR::connectToSceneMapObjDecorationMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_MapObjDecoration, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneMapObjStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_MapObjStrongLight, MR::DrawType_None);
}

void MR::connectToSceneMapParts(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_ClippedMapParts, MR::CalcAnimType_ClippedMapParts, MR::DrawBufferType_MapObj, MR::DrawType_None);
}

void MR::connectToScenePlanet(LiveActor* pActor) {
    if (MR::isExistIndirectTexture(pActor)) {
        MR::connectToScene(pActor, MR::MovementType_Planet, MR::CalcAnimType_Planet, MR::DrawBufferType_IndirectPlanet, MR::DrawType_None);
    } else {
        MR::connectToScene(pActor, MR::MovementType_Planet, MR::CalcAnimType_Planet, MR::DrawBufferType_Planet, MR::DrawType_None);
    }
}

void MR::connectToSceneEnvironment(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Environment, MR::CalcAnimType_Environment, MR::DrawBufferType_Environment, MR::DrawType_None);
}

void MR::connectToSceneEnvironmentStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Environment, MR::CalcAnimType_Environment, MR::DrawBufferType_EnvironmentStrongLight,
                       MR::DrawType_None);
}

void MR::connectToClippedMapParts(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_ClippedMapParts, MR::CalcAnimType_ClippedMapParts, MR::DrawBufferType_ClippedMapParts,
                       MR::DrawType_None);
}

void MR::connectToSceneEnemyDecoration(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_EnemyDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_EnemyDecoration,
                       MR::DrawType_None);
}

void MR::connectToSceneEnemyDecorationMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_EnemyDecoration, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneEnemyDecorationMovementCalcAnim(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_EnemyDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneItem(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Item, MR::CalcAnimType_Item, MR::DrawBufferType_NoSilhouettedMapObj, MR::DrawType_None);
}

void MR::connectToSceneItemStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Item, MR::CalcAnimType_Item, MR::DrawBufferType_NoSilhouettedMapObjStrongLight, MR::DrawType_None);
}

void MR::connectToSceneIndirectEnemy(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Enemy, MR::CalcAnimType_Enemy, MR::DrawBufferType_IndirectEnemy, MR::DrawType_None);
}

void MR::connectToSceneIndirectNpc(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_NPC, MR::CalcAnimType_NPC, MR::DrawBufferType_IndirectNpc, MR::DrawType_None);
}

void MR::connectToSceneIndirectMapObj(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_IndirectMapObj, MR::DrawType_None);
}

void MR::connectToSceneIndirectMapObjStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_IndirectMapObjStrongLight, MR::DrawType_None);
}

void MR::connectToSceneScreenEffectMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_ScreenEffect, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneAreaObj(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_AreaObj, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToScene3DModelFor2D(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Layout, MR::CalcAnimType_Layout, MR::DrawBufferType_Model3DFor2D, MR::DrawType_None);
}

void MR::connectToSceneLayout(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_Layout, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_Layout);
}

void MR::connectToSceneLayoutMovementCalcAnim(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_Layout, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneLayoutDecoration(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_LayoutDecoration, MR::CalcAnimType_LayoutDecoration, MR::DrawBufferType_None,
                       MR::DrawType_LayoutDecoration);
}

void MR::connectToSceneTalkLayout(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_Layout, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_TalkLayout);
}

void MR::connectToSceneTalkLayoutNoMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_None, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_TalkLayout);
}

void MR::connectToSceneWipeLayout(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_WipeLayout, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_WipeLayout);
}

void MR::connectToSceneLayoutOnPause(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_LayoutOnPause, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_LayoutOnPause);
}

void MR::connectToSceneLayoutOnPauseNoMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_None, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_LayoutOnPause);
}

void MR::connectToSceneLayoutOnPauseMovementCalcAnim(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_LayoutOnPause, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneLayoutMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_Layout, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneMovie(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_Movie, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_Movie);
}

void MR::connectToSceneMirrorMapObj(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MirrorMapObj, MR::DrawBufferType_MirrorMapObj, MR::DrawType_None);
}

void MR::connectToSceneMirrorMapObjDecoration(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObjDecoration, MR::CalcAnimType_MirrorMapObj, MR::DrawBufferType_MirrorMapObj, MR::DrawType_None);
}

void MR::connectToSceneMirrorMapObjNoMovement(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_None, MR::CalcAnimType_MirrorMapObj, MR::DrawBufferType_MirrorMapObj, MR::DrawType_None);
}

void MR::connectToSceneCamera(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_Camera, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void MR::connectToSceneNoShadowedMapObj(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoShadowedMapObj, MR::DrawType_None);
}

void MR::connectToSceneNoShadowedMapObjStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoShadowedMapObjStrongLight, MR::DrawType_None);
}

void MR::connectToSceneNoSilhouettedMapObj(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoSilhouettedMapObj, MR::DrawType_None);
}

void MR::connectToSceneNoSilhouettedMapObjStrongLight(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoSilhouettedMapObjStrongLight,
                       MR::DrawType_None);
}

void MR::connectToSceneNoSilhouettedMapObjWeakLightNoMovement(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_None, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoSilhouettedMapObjWeakLight, MR::DrawType_None);
}

void MR::connectToSceneSky(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Sky, MR::CalcAnimType_MapObj, MR::DrawBufferType_Sky, MR::DrawType_None);
}

void MR::connectToSceneAir(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Sky, MR::CalcAnimType_MapObj, MR::DrawBufferType_Air, MR::DrawType_None);
}

void MR::connectToSceneSun(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_Sky, MR::CalcAnimType_MapObj, MR::DrawBufferType_Sun, MR::DrawType_None);
}

void MR::connectToSceneCrystal(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_Crystal, MR::DrawType_None);
}

void MR::connectToSceneNormalMapObj(LiveActor* pActor) {
    MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_None, 0x18);  // ??
}

void MR::requestMovementOn(NameObj* pObj) {
    NameObjFunction::requestMovementOn(pObj);
}

void MR::requestMovementOn(LiveActor* pActor) {
    NameObjFunction::requestMovementOn(pActor);

    if (MR::isExistEffectKeeper(pActor)) {
        MR::Effect::requestMovementOn(pActor->mEffectKeeper);
    }
}

void MR::requestMovementOn(LayoutActor* pActor) {
    NameObjFunction::requestMovementOn(pActor);
}

void MR::requestMovementOff(NameObj* pObj) {
    NameObjFunction::requestMovementOff(pObj);
}

NameObjGroup* MR::joinToNameObjGroup(NameObj* pObj, const char* pGroupName) {
    NameObjGroup* pObjGroup;

    pObjGroup = static_cast< NameObjGroup* >(NameObjFinder::find(pGroupName));
    pObjGroup->registerObj(pObj);

    return pObjGroup;
}

NameObjGroup* MR::joinToMovementOnOffGroup(const char* pName, NameObj* pObj, u32 a3) {
    MovementOnOffGroupHolder* pGroupHolder = MR::getSceneObj< MovementOnOffGroupHolder >(SceneObj_MovementOnOffGroupHolder);

    return pGroupHolder->joinToGroup(pName, pObj, a3);
}

void MR::onMovementOnOffGroup(const char* pGroupName) {
    MR::getSceneObj< MovementOnOffGroupHolder >(SceneObj_MovementOnOffGroupHolder)->onMovementGroup(pGroupName);
}

void MR::registerPreDrawFunction(const MR::FunctorBase& rFunc, int drawType) {
    SingletonHolder< GameSystem >::get()->mSceneController->getNameObjListExecutor()->registerPreDrawFunction(rFunc, drawType);
}

NameObjAdaptor* MR::createDrawAdaptor(const char* pName, const MR::FunctorBase& rFunctor) {
    NameObjAdaptor* pAdaptor;

    pAdaptor = new NameObjAdaptor(pName);
    pAdaptor->connectToDraw(rFunctor);

    return pAdaptor;
}

NameObjAdaptor* MR::createAdaptorAndConnectToDrawBloomModel(const char* pName, const MR::FunctorBase& rFunctor) {
    NameObjAdaptor* pAdaptor;

    pAdaptor = new NameObjAdaptor(pName);
    pAdaptor->connectToDraw(rFunctor);

    MR::connectToScene(pAdaptor, MR::MovementType_None, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_BloomModel);

    return pAdaptor;
}

void MR::listenNameObjStageSwitchOnAppear(const NameObj* pObj, const StageSwitchCtrl* pSwitchCtrl, const MR::FunctorBase& rOnFunctor) {
    SwitchEventFunctorListener* pListener;

    pListener = new SwitchEventFunctorListener();
    pListener->setOnFunctor(rOnFunctor);

    MR::getSwitchWatcherHolder()->joinSwitchEventListenerAppear(pSwitchCtrl, pListener);
}

void MR::listenNameObjStageSwitchOnOffAppear(const NameObj* pObj, const StageSwitchCtrl* pSwitchCtrl, const MR::FunctorBase& rOnFunctor,
                                             const MR::FunctorBase& rOffFunctor) {
    SwitchEventFunctorListener* pListener;

    pListener = new SwitchEventFunctorListener();
    pListener->setOnFunctor(rOnFunctor);
    pListener->setOffFunctor(rOffFunctor);

    MR::getSwitchWatcherHolder()->joinSwitchEventListenerAppear(pSwitchCtrl, pListener);
}

void MR::listenNameObjStageSwitchOnA(const NameObj* pObj, const StageSwitchCtrl* pSwitchCtrl, const MR::FunctorBase& rOnFunctor) {
    SwitchEventFunctorListener* pListener;

    pListener = new SwitchEventFunctorListener();
    pListener->setOnFunctor(rOnFunctor);

    MR::getSwitchWatcherHolder()->joinSwitchEventListenerA(pSwitchCtrl, pListener);
}

void MR::listenNameObjStageSwitchOnOffA(const NameObj* pObj, const StageSwitchCtrl* pSwitchCtrl, const MR::FunctorBase& rOnFunctor,
                                        const MR::FunctorBase& rOffFunctor) {
    SwitchEventFunctorListener* pListener;

    pListener = new SwitchEventFunctorListener();
    pListener->setOnFunctor(rOnFunctor);
    pListener->setOffFunctor(rOffFunctor);

    MR::getSwitchWatcherHolder()->joinSwitchEventListenerA(pSwitchCtrl, pListener);
}

void MR::listenNameObjStageSwitchOnB(const NameObj* pObj, const StageSwitchCtrl* pSwitchCtrl, const MR::FunctorBase& rOnFunctor) {
    SwitchEventFunctorListener* pListener;

    pListener = new SwitchEventFunctorListener();
    pListener->setOnFunctor(rOnFunctor);

    MR::getSwitchWatcherHolder()->joinSwitchEventListenerB(pSwitchCtrl, pListener);
}

void MR::listenNameObjStageSwitchOffB(const NameObj* pObj, const StageSwitchCtrl* pSwitchCtrl, const MR::FunctorBase& rOffFunctor) {
    SwitchEventFunctorListener* pListener;

    pListener = new SwitchEventFunctorListener();
    pListener->setOffFunctor(rOffFunctor);

    MR::getSwitchWatcherHolder()->joinSwitchEventListenerB(pSwitchCtrl, pListener);
}

void MR::listenNameObjStageSwitchOnOffB(const NameObj* pObj, const StageSwitchCtrl* pSwitchCtrl, const MR::FunctorBase& rOnFunctor,
                                        const MR::FunctorBase& rOffFunctor) {
    SwitchEventFunctorListener* pListener;

    pListener = new SwitchEventFunctorListener();
    pListener->setOnFunctor(rOnFunctor);
    pListener->setOffFunctor(rOffFunctor);

    MR::getSwitchWatcherHolder()->joinSwitchEventListenerB(pSwitchCtrl, pListener);
}

ResourceHolder* MR::createAndAddResourceHolder(const char* pResource) {
    return SingletonHolder< ResourceHolderManager >::get()->createAndAdd(pResource, nullptr);
}

void* MR::loadResourceFromArc(const char* pArchive, const char* pFile) {
    ResourceHolder* pResourceHolder = createAndAddResourceHolder(pArchive);

    return pResourceHolder->mFileInfoTable->getRes(pFile);
}

bool MR::isExistResourceInArc(const char* pArchive, const char* pFile) {
    ResourceHolder* pResourceHolder = createAndAddResourceHolder(pArchive);

    return pResourceHolder->mFileInfoTable->isExistRes(pFile);
}

const ResTIMG* MR::loadTexFromArc(const char* pArchive, const char* pFile) {
    return static_cast< const ResTIMG* >(loadResourceFromArc(pArchive, pFile));
}

const ResTIMG* MR::loadTexFromArc(const char* pArchive) {
    char arcName[256];
    snprintf(arcName, sizeof(arcName), "%s.arc", pArchive);

    char texName[256];
    snprintf(texName, sizeof(texName), "%s.bti", pArchive);

    return loadTexFromArc(arcName, texName);
}

namespace {
    bool isExistFileInArcLocal(const ResourceHolder* pHolder, const char* pArchive, va_list pFormat) NO_INLINE {
        char buf[0x100];
        vsnprintf(buf, sizeof(buf), pArchive, pFormat);

        return pHolder->mFileInfoTable->isExistRes(buf) != false;
    }
};  // namespace

bool MR::isExistFileInArc(const ResourceHolder* pHolder, const char* pFormat, ...) {
    va_list list;
    va_start(list, pFormat);

    return ::isExistFileInArcLocal(pHolder, pFormat, list);
}

namespace {
    JMapInfo* tryCreateCsvParserLocal(const ResourceHolder* pHolder, const char* pArchive, va_list pFormat) NO_INLINE {
        char buf[0x100];
        vsnprintf(buf, sizeof(buf), pArchive, pFormat);

        if (!pHolder->mFileInfoTable->isExistRes(buf)) {
            return nullptr;
        }

        JMapInfo* pInfo = new JMapInfo();
        pInfo->attach(pHolder->mFileInfoTable->getRes(buf));

        return pInfo;
    }
};  // namespace

JMapInfo* MR::createCsvParser(const ResourceHolder* pHolder, const char* pFormat, ...) {
    va_list list;
    va_start(list, pFormat);

    return ::tryCreateCsvParserLocal(pHolder, pFormat, list);
}

JMapInfo* MR::createCsvParser(const char* pArchive, const char* pFormat, ...) {
    ResourceHolder* pResourceHolder = SingletonHolder< ResourceHolderManager >::get()->createAndAdd(pArchive, nullptr);

    return MR::createCsvParser(pResourceHolder, pFormat);
}

JMapInfo* MR::tryCreateCsvParser(const LiveActor* pActor, const char* pFormat, ...) {
    va_list list;
    va_start(list, pFormat);

    return ::tryCreateCsvParserLocal(getResourceHolder(pActor), pFormat, list);
}

JMapInfo* MR::tryCreateCsvParser(const ResourceHolder* pHolder, const char* pFormat, ...) {
    va_list list;
    va_start(list, pFormat);

    return ::tryCreateCsvParserLocal(pHolder, pFormat, list);
}

s32 MR::getCsvDataElementNum(const JMapInfo* pMapInfo) {
    if (pMapInfo->mData != nullptr) {
        return pMapInfo->mData->mNumEntries;
    }

    return 0;
}

void MR::getCsvDataStr(const char** pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
    pMapInfo->getValue(idx, pKey, pOut);
}

void MR::getCsvDataStrOrNULL(const char** pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
    getCsvDataStr(pOut, pMapInfo, pKey, idx);

    if (*pOut[0] == 0) {
        *pOut = nullptr;
    }
}

void MR::getCsvDataS32(s32* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
    pMapInfo->getValue< s32 >(idx, pKey, pOut);
}

void MR::getCsvDataU8(u8* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) NO_INLINE {
    s32 val = 0;
    pMapInfo->getValue< s32 >(idx, pKey, &val);
    *pOut = val;
}

void MR::getCsvDataF32(f32* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
    pMapInfo->getValue(idx, pKey, pOut);
}

void MR::getCsvDataBool(bool* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
    pMapInfo->getValue(idx, pKey, pOut);
}

void MR::getCsvDataVec(Vec* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
    char key[256];
    snprintf(key, sizeof(key), "%sX", pKey);
    getCsvDataF32(&pOut->x, pMapInfo, key, idx);
    snprintf(key, sizeof(key), "%sY", pKey);
    getCsvDataF32(&pOut->y, pMapInfo, key, idx);
    snprintf(key, sizeof(key), "%sZ", pKey);
    getCsvDataF32(&pOut->z, pMapInfo, key, idx);
}

void MR::getCsvDataColor(GXColor* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
    char key[256];
    snprintf(key, sizeof(key), "%sR", pKey);
    getCsvDataU8(&pOut->r, pMapInfo, key, idx);
    snprintf(key, sizeof(key), "%sG", pKey);
    getCsvDataU8(&pOut->g, pMapInfo, key, idx);
    snprintf(key, sizeof(key), "%sB", pKey);
    getCsvDataU8(&pOut->b, pMapInfo, key, idx);
    snprintf(key, sizeof(key), "%sA", pKey);
    getCsvDataU8(&pOut->a, pMapInfo, key, idx);
}

bool MR::isStageStateScenarioOpeningCamera() {
    return GameSceneFunction::isExecScenarioOpeningCamera();
}

bool MR::isStageStatePowerStarAppeared() {
    return EventFunction::getStageStateKeeper()->mIsPowerStarAppeared;
}

void MR::declarePowerStar(const NameObj* pObj) {
    declareEventPowerStar(pObj, -1, true);
}

void MR::declarePowerStar(const NameObj* pObj, s32 id) {
    declareEventPowerStar(pObj, id, true);
}

void MR::declarePowerStarCoin100() {
    declareEventPowerStar("１００枚コイン", -1, true);
}

void MR::appearPowerStarContinueCurrentDemo(const NameObj* pObj, const TVec3f& rParam2) {
    appearEventPowerStar(pObj->getName(), -1, &rParam2, true, false);
}

void MR::appearPowerStarWithoutDemo(const NameObj* pObj) {
    appearEventPowerStar(pObj->getName(), -1, nullptr, false, true);
}

void MR::requestAppearPowerStar(const NameObj* pObj) {
    appearEventPowerStar(pObj->getName(), -1, nullptr, false, false);
}

void MR::requestAppearPowerStar(const NameObj* pObj, const TVec3f& rParam2) {
    appearEventPowerStar(pObj->getName(), -1, &rParam2, false, false);
}

void MR::requestAppearPowerStar(const NameObj* pObj, s32 param2, const TVec3f& rParam3) {
    appearEventPowerStar(pObj->getName(), param2, &rParam3, false, false);
}

void MR::requestAppearPowerStar(const NameObj* pObj, const LiveActor* pActor, f32 flt) {
    MtxPtr baseMtx = pActor->getBaseMtx();
    TVec3f baseMtxY(baseMtx[1][0], baseMtx[1][1], baseMtx[1][2]);
    baseMtxY.scale(flt);
    baseMtxY.add(pActor->mPosition);
    appearEventPowerStar(pObj->getName(), -1, &baseMtxY, false, false);
}

void MR::requestAppearPowerStarCoin100() {
    appearEventPowerStar("１００枚コイン", -1, nullptr, false, false);
}

bool MR::isEndPowerStarAppearDemo(const NameObj* pObj) {
    return isEndEventPowerStarAppearDemo(pObj->getName());
}

ModelObj* MR::createPowerStarDemoModel(const NameObj* pObj, const char* pName, MtxPtr pMtx) {
    ModelObj* pPowerStarDemoModel = createModelObjNoSilhouettedMapObjStrongLight(pName, "PowerStar", pMtx);

    PowerStar::setupColor(pPowerStarDemoModel, pObj, -1);

    return pPowerStarDemoModel;
}

void MR::declareCoin(const NameObj* pObj, s32 param2) {
    if (param2 <= 0) {
        return;
    }

    MR::createCoinHolder();
    MR::getCoinHolder()->declare(pObj, param2);
}

s32 MR::getDeclareRemnantCoinCount(const NameObj* pObj) {
    return MR::getCoinHolder()->getDeclareRemnantCoinCount(pObj);
}

void MR::hopCoin(const NameObj* pObj, const TVec3f& rParam2, const TVec3f& rParam3) {
    MR::getCoinHolder()->hopCoin(pObj, rParam2, rParam3);
}

void MR::appearCoinFix(const NameObj* pObj, const TVec3f& rParam2, s32 param3) {
    MR::getCoinHolder()->appearCoinFix(pObj, rParam2, param3);
}

void MR::appearCoinPop(const NameObj* pObj, const TVec3f& rParam2, s32 param3) {
    MR::getCoinHolder()->appearCoinPop(pObj, rParam2, param3);
}

void MR::appearCoinPopToDirection(const NameObj* pObj, const TVec3f& rParam2, const TVec3f& rParam3, s32 param4) {
    MR::getCoinHolder()->appearCoinPopToDirection(pObj, rParam2, rParam3, param4);
}

void MR::appearCoinToVelocity(const NameObj* pObj, const TVec3f& rParam2, const TVec3f& rParam3, s32 param4) {
    MR::getCoinHolder()->appearCoinToVelocity(pObj, rParam2, rParam3, param4);
}

void MR::appearCoinCircle(const NameObj* pObj, const TVec3f& rParam2, s32 param3) {
    MR::getCoinHolder()->appearCoinCircle(pObj, rParam2, param3);
}

void MR::declareStarPiece(const NameObj* pObj, s32 num) {
    getStarPieceDirector()->declare(pObj, num);
}

s32 MR::getDeclareRemnantStarPieceCount(const NameObj* pObj) {
    return getStarPieceDirector()->getDeclareRemnantStarPieceCount(pObj);
}

s32 MR::getDeclareRemnantStarPieceCountNotExist(const NameObj* pObj) {
    return getStarPieceDirector()->getDeclareRemnantStarPieceCountNotExist(pObj);
}

void MR::declareStarPieceReceiver(const NameObj* pObj, s32 num) {
    getStarPieceDirector()->declareReceiveNum(pObj, num);
}

void MR::clearGotCountStarPieceReceiver(const NameObj* pObj) {
    getStarPieceDirector()->clearGotCountReceiver(pObj);
}

void MR::hopStarPiece(const NameObj* pObj, const TVec3f& rVec1, const TVec3f& rVec2) {
    getStarPieceDirector()->hopPiece(pObj, rVec1, rVec2);
}

bool MR::appearStarPiece(const NameObj* pObj, const TVec3f& rVec1, s32 a1, f32 f1, f32 f2, bool a2) {
    return getStarPieceDirector()->appearPiece(pObj, rVec1, a1, f1, f2, false, a2);
}

bool MR::appearStarPieceToDirection(const NameObj* pObj, const TVec3f& rVec1, const TVec3f& rVec2, s32 a1, f32 f1, f32 f2, bool a2) {
    return getStarPieceDirector()->appearPieceToDirection(pObj, rVec1, rVec2, a1, f1, f2, false, a2);
}

void MR::initStarPieceGetCSSound() {
    StarPieceDirector* pDirector = getStarPieceDirector();

    if (pDirector != nullptr) {
        pDirector->initCSSound();
    }
}

BenefitItemOneUp* MR::createKinokoOneUp() {
    BenefitItemOneUp* pKinokoOneUp;

    pKinokoOneUp = new BenefitItemOneUp("１ＵＰキノコ(create)");
    pKinokoOneUp->initWithoutIter();
    pKinokoOneUp->makeActorDead();

    return pKinokoOneUp;
}

void MR::appearKinokoOneUpPop(BenefitItemObj* pBenefitObj, MtxPtr pMtx, f32 speed) {
    TPos3f mtx(pMtx);
    TVec3f rotation;
    mtx.getEulerXYZ(rotation);

    TVec3f position;
    TVec3f up;
    TVec3f rotationDegrees = rotation;
    rotationDegrees.scale(_180_PI);
    pBenefitObj->mRotation.set(rotationDegrees);

    mtx.getTrans(position);
    mtx.getYDir(up);
    MR::normalize(&up);

    TVec3f velocity = up;
    velocity.scale(speed);
    pBenefitObj->shoot(position, velocity, true);
}

BenefitItemLifeUp* MR::createKinokoSuper() {
    BenefitItemLifeUp* pKinokoSuper;

    pKinokoSuper = new BenefitItemLifeUp("スーパーキノコ");
    pKinokoSuper->initWithoutIter();
    pKinokoSuper->makeActorDead();

    return pKinokoSuper;
}

void MR::appearKinokoSuper(BenefitItemLifeUp* pKinokoSuper, MtxPtr pMtx, f32 param3) {
    appearKinokoOneUpPop(pKinokoSuper, pMtx, param3);
}

void MR::stopScene(s32 frame) {
    MR::getSceneObj< StopSceneController >(SceneObj_StopSceneController)->requestStopScene(frame);
}

void MR::stopSceneForDefaultHit(s32 frame) {
    MR::getSceneObj< StopSceneController >(SceneObj_StopSceneController)->requestStopSceneDelay(frame, 2);
}

bool MR::tryRumblePad(const void* pParam1, const char* pPatternName, s32 channel) {
    return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, pPatternName);
}

bool MR::tryRumblePadVeryStrongLong(const void* pParam1, s32 channel) {
    return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "最強【長】");
}

bool MR::tryRumblePadVeryStrong(const void* pParam1, s32 channel) {
    return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "最強");
}

bool MR::tryRumblePadStrong(const void* pParam1, s32 channel) {
    return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "強");
}

bool MR::tryRumblePadMiddle(const void* pParam1, s32 channel) {
    return WPadFunction::getWPadRumble(channel)->getRumbleInstance()->vibratePatternIfNotExist(pParam1, "中");
}

bool MR::tryRumblePadWeak(const void* pParam1, s32 channel) {
    return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "弱");
}

bool MR::tryRumblePadVeryWeak(const void* pParam1, s32 channel) {
    return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "微弱");
}

bool MR::tryRumbleDefaultHit(const void* pParam1, s32 channel) {
    return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "強");
}

void MR::shakeCameraVeryStrong() {
    getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_VERY_STRONG);
}

void MR::shakeCameraStrong() {
    getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_STRONG);
}

void MR::shakeCameraNormalStrong() {
    getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_NORMAL_STRONG);
}

void MR::shakeCameraNormal() {
    getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_NORMAL);
}

void MR::shakeCameraNormalWeak() {
    getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_NORMAL_WEAK);
}

void MR::shakeCameraWeak() {
    getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_WEAK);
}

void MR::shakeCameraVeryWeak() {
    getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_VERY_WEAK);
}

void MR::shakeCameraInfinity(NameObj* pObj, f32 intensity, f32 speed) {
    getCameraDirector()->mShaker->shakeInfinity(pObj, intensity, 15.0f / speed);
}

void MR::stopShakingCamera(NameObj* pObj) {
    getCameraDirector()->mShaker->stopShakingInfinity(pObj);
}

bool MR::isName(const NameObj* pObj, const char* pName) {
    return strcmp(pObj->getName(), pName) == 0;
}

bool MR::isSame(const NameObj* pObj1, const NameObj* pObj2) {
    return pObj1 == pObj2;
}

bool MR::tryRegisterNamePosLinkObj(const NameObj* pObj, const JMapInfoIter& rIter) {
    return MR::getNamePosHolder()->tryRegisterLinkObj(pObj, rIter);
}

bool MR::findNamePos(const char* pName, MtxPtr pMtx) {
    return MR::tryFindLinkNamePos(nullptr, pName, pMtx);
}

bool MR::findNamePos(const char* pName, TVec3f* pPosition, TVec3f* pRotation) {
    return getNamePosHolder()->find(nullptr, pName, pPosition, pRotation);
}

void MR::findNamePosOnGround(const char* pName, MtxPtr pMtx) {
    Triangle triangle;
    TPos3f mtx;
    TVec3f hitPos;
    TVec3f position;
    TVec3f gravity;
    TVec3f front;
    findNamePos(pName, mtx.toMtxPtr());
    mtx.getTrans(position);
    mtx.getZDir(front);
    calcGravityVector(nullptr, position, &gravity, nullptr, 0);

    if (getFirstPolyOnLineToMap(&hitPos, &triangle, position - gravity * 100.0f, gravity * 1000.0f)) {
        makeMtxUpFrontPos(&mtx, -gravity, front, hitPos);
    }

    PSMTXCopy(mtx.toMtxPtr(), pMtx);
}

bool MR::tryFindNamePos(const char* pName, MtxPtr pMtx) {
    return MR::tryFindLinkNamePos(nullptr, pName, pMtx);
}

bool MR::tryFindNamePos(const char* pName, TVec3f* pParam2, TVec3f* pParam3) {
    return MR::tryFindLinkNamePos(nullptr, pName, pParam2, pParam3);
}

void MR::findLinkNamePos(const NameObj* pObj, const char* pName, MtxPtr pMtx) {
    MR::tryFindLinkNamePos(pObj, pName, pMtx);
}

bool MR::tryFindLinkNamePos(const NameObj* pObj, const char* pName, MtxPtr pMtx) {
    TVec3f position(0.0f, 0.0f, 0.0f);
    TVec3f rotation(0.0f, 0.0f, 0.0f);

    if (getNamePosHolder()->find(pObj, pName, &position, &rotation)) {
        makeMtxTR(pMtx, position, rotation);
        return true;
    }

    return false;
}

bool MR::tryFindLinkNamePos(const NameObj* pObj, const char* pName, TVec3f* pParam3, TVec3f* pParam4) {
    return getNamePosHolder()->find(pObj, pName, pParam3, pParam4);
}
