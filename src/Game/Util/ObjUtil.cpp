#include "Game/Util/ObjUtil.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraShaker.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/LiveActor/ClippingJudge.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Map/NamePosHolder.hpp"
#include "Game/Map/SwitchWatcherHolder.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/MapObj/BenefitItemLifeUp.hpp"
#include "Game/MapObj/BenefitItemObj.hpp"
#include "Game/MapObj/CoinHolder.hpp"
#include "Game/MapObj/PowerStar.hpp"
#include "Game/NPC/EventDirector.hpp"
#include "Game/NPC/StageStateKeeper.hpp"
#include "Game/NameObj/MovementOnOffGroupHolder.hpp"
#include "Game/NameObj/NameObjAdaptor.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjFinder.hpp"
#include "Game/NameObj/NameObjListExecutor.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Scene/StopSceneController.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/System/WPadRumble.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include <cstdio>
#include <va_list.h>

namespace {
    JMapInfo* tryCreateCsvParserLocal(const ResourceHolder* pHolder, const char* pArchive, va_list pFormat) NO_INLINE {
        char buf[0x100];
        vsnprintf(buf, sizeof(buf), pArchive, pFormat);

        if (!pHolder->mFileInfoTable->isExistRes(buf)) {
            return nullptr;
        }

        JMapInfo* inf = new JMapInfo();
        inf->attach(pHolder->mFileInfoTable->getRes(buf));

        return inf;
    }
};  // namespace

namespace MR {
    bool isJudgedToClipFrustum(const TVec3f& rParam1, f32 param2) { return MR::getClippingJudge()->isJudgedToClipFrustum(rParam1, param2); }

    bool isJudgedToClipFrustum100m(const TVec3f& rParam1, f32 param2) { return MR::getClippingJudge()->isJudgedToClipFrustum(rParam1, param2, 6); }

    bool isJudgedToClipFrustum300m(const TVec3f& rParam1, f32 param2) { return MR::getClippingJudge()->isJudgedToClipFrustum(rParam1, param2, 4); }

    bool isJudgedToNearClip(const TVec3f& rParam1, f32 param2) {
        TVec3f camPos = MR::getCamPos();

        return PSVECDistance(&rParam1, &camPos) < param2;
    }

    bool isInWater(const TVec3f& rParam1) {
        WaterInfo waterInfo = WaterInfo();

        return MR::getWaterAreaObj(&waterInfo, rParam1);
    }

    bool isInDeath(const TVec3f& rParam1) { return MR::isInAreaObj("DeathArea", rParam1); }

    bool isInDarkMatter(const TVec3f& rParam1) {
        return MR::isInAreaObj("DarkMatterCube", rParam1) || MR::isInAreaObj("DarkMatterCylinder", rParam1);
    }

    void connectToScene(LiveActor* pActor, int movementType, int calcAnimType, int drawBufferType, int drawType) {
        MR::registerNameObjToExecuteHolder(pActor, movementType, calcAnimType, drawBufferType, drawType);
    }

    void connectToScene(NameObj* pObj, int movementType, int calcAnimType, int drawBufferType, int drawType) {
        MR::registerNameObjToExecuteHolder(pObj, movementType, calcAnimType, drawBufferType, drawType);
        MR::connectToSceneTemporarily(pObj);
        MR::connectToDrawTemporarily(pObj);
    }

    void connectToSceneCollisionMapObj(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_CollisionMapObj, MR::CalcAnimType_CollisionMapObj, MR::DrawBufferType_MapObj, -1);
    }

    void connectToSceneCollisionMapObjMovementCalcAnim(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_CollisionMapObj, MR::CalcAnimType_CollisionMapObj, -1, -1);
    }

    void connectToSceneCollisionMapObjWeakLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_CollisionMapObj, MR::CalcAnimType_CollisionMapObj, MR::DrawBufferType_MapObjWeakLight, -1);
    }

    void connectToSceneCollisionMapObjStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_CollisionMapObj, MR::CalcAnimType_CollisionMapObj, MR::DrawBufferType_MapObjStrongLight, -1);
    }

    void connectToSceneCollisionEnemy(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_CollisionEnemy, MR::CalcAnimType_CollisionEnemy, MR::DrawBufferType_Enemy, -1);
    }

    void connectToSceneCollisionEnemyMovement(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_CollisionEnemy, -1, -1, -1); }

    void connectToSceneCollisionEnemyStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_CollisionEnemy, MR::CalcAnimType_CollisionEnemy, MR::DrawBufferType_MapObjStrongLight, -1);
    }

    void connectToSceneCollisionEnemyNoShadowedMapObjStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_CollisionEnemy, MR::CalcAnimType_CollisionEnemy, MR::DrawBufferType_NoShadowedMapObjStrongLight,
                           -1);
    }

    void connectToSceneNpc(LiveActor* pActor) { MR::connectToScene(pActor, MR::MovementType_NPC, MR::CalcAnimType_NPC, MR::DrawBufferType_NPC, -1); }

    void connectToSceneNpcMovement(LiveActor* pActor) { MR::connectToScene(pActor, MR::MovementType_NPC, -1, -1, -1); }

    void connectToSceneRide(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Ride, MR::CalcAnimType_Ride, MR::DrawBufferType_Ride, -1);
    }

    void connectToSceneEnemy(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Enemy, MR::CalcAnimType_Enemy, MR::DrawBufferType_Enemy, -1);
    }

    void connectToSceneEnemyMovement(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_Enemy, -1, -1, -1); }

    void connectToSceneMapObj(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_MapObj, -1);
    }

    void connectToSceneMapObjMovement(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_MapObj, -1, -1, -1); }

    void connectToSceneMapObjMovementCalcAnim(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, -1, -1); }

    void connectToSceneMapObjNoMovement(LiveActor* pActor) { MR::connectToScene(pActor, -1, MR::CalcAnimType_MapObj, MR::DrawBufferType_MapObj, -1); }

    void connectToSceneMapObjNoCalcAnim(LiveActor* pActor) { MR::connectToScene(pActor, MR::MovementType_MapObj, -1, MR::DrawBufferType_MapObj, -1); }

    void connectToSceneMapObjNoCalcAnimStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, -1, MR::DrawBufferType_MapObjStrongLight, -1);
    }

    void connectToSceneMapObjDecoration(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObjDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_MapObj, -1);
    }

    void connectToSceneMapObjDecorationStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObjDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_MapObjStrongLight, -1);
    }

    void connectToSceneMapObjDecorationMovement(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_MapObjDecoration, -1, -1, -1); }

    void connectToSceneMapObjStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_MapObjStrongLight, -1);
    }

    void connectToSceneMapParts(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_ClippedMapParts, MR::CalcAnimType_ClippedMapParts, MR::DrawBufferType_MapObj, -1);
    }

    void connectToScenePlanet(LiveActor* pActor) {
        if (MR::isExistIndirectTexture(pActor)) {
            MR::connectToScene(pActor, MR::MovementType_Planet, MR::CalcAnimType_Planet, MR::DrawBufferType_IndirectPlanet, -1);
        } else {
            MR::connectToScene(pActor, MR::MovementType_Planet, MR::CalcAnimType_Planet, MR::DrawBufferType_Planet, -1);
        }
    }

    void connectToSceneEnvironment(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Environment, MR::CalcAnimType_Environment, MR::DrawBufferType_Environment, -1);
    }

    void connectToSceneEnvironmentStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Environment, MR::CalcAnimType_Environment, MR::DrawBufferType_EnvironmentStrongLight, -1);
    }

    void connectToClippedMapParts(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_ClippedMapParts, MR::CalcAnimType_ClippedMapParts, MR::DrawBufferType_ClippedMapParts, -1);
    }

    void connectToSceneEnemyDecoration(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_EnemyDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_EnemyDecoration, -1);
    }

    void connectToSceneEnemyDecorationMovement(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_EnemyDecoration, -1, -1, -1); }

    void connectToSceneEnemyDecorationMovementCalcAnim(NameObj* pObj) {
        MR::connectToScene(pObj, MR::MovementType_EnemyDecoration, MR::CalcAnimType_MapObjDecoration, -1, -1);
    }

    void connectToSceneItem(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Item, MR::CalcAnimType_Item, MR::DrawBufferType_NoSilhouettedMapObj, -1);
    }

    void connectToSceneItemStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Item, MR::CalcAnimType_Item, MR::DrawBufferType_NoSilhouettedMapObjStrongLight, -1);
    }

    void connectToSceneIndirectEnemy(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Enemy, MR::CalcAnimType_Enemy, MR::DrawBufferType_IndirectEnemy, -1);
    }

    void connectToSceneIndirectNpc(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_NPC, MR::CalcAnimType_NPC, MR::DrawBufferType_IndirectNpc, -1);
    }

    void connectToSceneIndirectMapObj(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_IndirectMapObj, -1);
    }

    void connectToSceneIndirectMapObjStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_IndirectMapObjStrongLight, -1);
    }

    void connectToSceneScreenEffectMovement(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_ScreenEffect, -1, -1, -1); }

    void connectToSceneAreaObj(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_AreaObj, -1, -1, -1); }

    void connectToScene3DModelFor2D(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Layout, MR::CalcAnimType_Layout, MR::DrawBufferType_Model3DFor2D, -1);
    }

    void connectToSceneLayout(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_Layout, MR::CalcAnimType_Layout, -1, MR::DrawType_Layout); }

    void connectToSceneLayoutMovementCalcAnim(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_Layout, MR::CalcAnimType_Layout, -1, -1); }

    void connectToSceneLayoutDecoration(NameObj* pObj) {
        MR::connectToScene(pObj, MR::MovementType_LayoutDecoration, MR::CalcAnimType_LayoutDecoration, -1, MR::DrawType_LayoutDecoration);
    }

    void connectToSceneTalkLayout(NameObj* pObj) {
        MR::connectToScene(pObj, MR::MovementType_Layout, MR::CalcAnimType_Layout, -1, MR::DrawType_TalkLayout);
    }

    void connectToSceneTalkLayoutNoMovement(NameObj* pObj) { MR::connectToScene(pObj, -1, MR::CalcAnimType_Layout, -1, MR::DrawType_TalkLayout); }

    void connectToSceneWipeLayout(NameObj* pObj) {
        MR::connectToScene(pObj, MR::MovementType_WipeLayout, MR::CalcAnimType_Layout, -1, MR::DrawType_WipeLayout);
    }

    void connectToSceneLayoutOnPause(NameObj* pObj) {
        MR::connectToScene(pObj, MR::MovementType_LayoutOnPause, MR::CalcAnimType_Layout, -1, MR::DrawType_LayoutOnPause);
    }

    void connectToSceneLayoutOnPauseNoMovement(NameObj* pObj) {
        MR::connectToScene(pObj, -1, MR::CalcAnimType_Layout, -1, MR::DrawType_LayoutOnPause);
    }

    void connectToSceneLayoutOnPauseMovementCalcAnim(NameObj* pObj) {
        MR::connectToScene(pObj, MR::MovementType_LayoutOnPause, MR::CalcAnimType_Layout, -1, -1);
    }

    void connectToSceneLayoutMovement(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_Layout, -1, -1, -1); }

    void connectToSceneMovie(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_Movie, -1, -1, MR::DrawType_Movie); }

    void connectToSceneMirrorMapObj(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MirrorMapObj, MR::DrawBufferType_MirrorMapObj, -1);
    }

    void connectToSceneMirrorMapObjDecoration(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObjDecoration, MR::CalcAnimType_MirrorMapObj, MR::DrawBufferType_MirrorMapObj, -1);
    }

    void connectToSceneMirrorMapObjNoMovement(LiveActor* pActor) {
        MR::connectToScene(pActor, -1, MR::CalcAnimType_MirrorMapObj, MR::DrawBufferType_MirrorMapObj, -1);
    }

    void connectToSceneCamera(NameObj* pObj) { MR::connectToScene(pObj, MR::MovementType_Camera, -1, -1, -1); }

    void connectToSceneNoShadowedMapObj(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoShadowedMapObj, -1);
    }

    void connectToSceneNoShadowedMapObjStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoShadowedMapObjStrongLight, -1);
    }

    void connectToSceneNoSilhouettedMapObj(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoSilhouettedMapObj, -1);
    }

    void connectToSceneNoSilhouettedMapObjStrongLight(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoSilhouettedMapObjStrongLight, -1);
    }

    void connectToSceneNoSilhouettedMapObjWeakLightNoMovement(LiveActor* pActor) {
        MR::connectToScene(pActor, -1, MR::CalcAnimType_MapObj, MR::DrawBufferType_NoSilhouettedMapObjWeakLight, -1);
    }

    void connectToSceneSky(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Sky, MR::CalcAnimType_MapObj, MR::DrawBufferType_Sky, -1);
    }

    void connectToSceneAir(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Sky, MR::CalcAnimType_MapObj, MR::DrawBufferType_Air, -1);
    }

    void connectToSceneSun(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_Sky, MR::CalcAnimType_MapObj, MR::DrawBufferType_Sun, -1);
    }

    void connectToSceneCrystal(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_Crystal, -1);
    }

    void connectToSceneNormalMapObj(LiveActor* pActor) {
        MR::connectToScene(pActor, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, -1, 0x18);  // ??
    }

    void requestMovementOn(NameObj* pObj) { NameObjFunction::requestMovementOn(pObj); }

    void requestMovementOn(LiveActor* pActor) {
        NameObjFunction::requestMovementOn(pActor);

        if (MR::isExistEffectKeeper(pActor)) {
            MR::Effect::requestMovementOn(pActor->mEffectKeeper);
        }
    }

    void requestMovementOn(LayoutActor* pActor) { NameObjFunction::requestMovementOn(pActor); }

    void requestMovementOff(NameObj* pObj) { NameObjFunction::requestMovementOff(pObj); }

    NameObjGroup* joinToNameObjGroup(NameObj* pObj, const char* pGroupName) {
        NameObjGroup* pObjGroup = static_cast< NameObjGroup* >(NameObjFinder::find(pGroupName));

        pObjGroup->registerObj(pObj);

        return pObjGroup;
    }

    NameObjGroup* joinToMovementOnOffGroup(const char* pName, NameObj* pObj, u32 a3) {
        MovementOnOffGroupHolder* pGroupHolder = MR::getSceneObj< MovementOnOffGroupHolder >(SceneObj_MovementOnOffGroupHolder);

        return pGroupHolder->joinToGroup(pName, pObj, a3);
    }

    void onMovementOnOffGroup(const char* pGroupName) {
        MR::getSceneObj< MovementOnOffGroupHolder >(SceneObj_MovementOnOffGroupHolder)->onMovementGroup(pGroupName);
    }

    void registerPreDrawFunction(const MR::FunctorBase& rFunc, int drawType) {
        SingletonHolder< GameSystem >::get()->mSceneController->getNameObjListExecutor()->registerPreDrawFunction(rFunc, drawType);
    }

    NameObjAdaptor* createDrawAdaptor(const char* pName, const MR::FunctorBase& rFunctor) {
        NameObjAdaptor* pAdaptor = new NameObjAdaptor(pName);

        pAdaptor->connectToDraw(rFunctor);

        return pAdaptor;
    }

    NameObjAdaptor* createAdaptorAndConnectToDrawBloomModel(const char* pName, const MR::FunctorBase& rFunctor) {
        NameObjAdaptor* pAdaptor = new NameObjAdaptor(pName);
        pAdaptor->connectToDraw(rFunctor);

        MR::connectToScene(pAdaptor, -1, -1, -1, MR::DrawType_BloomModel);

        return pAdaptor;
    }

    void listenNameObjStageSwitchOnAppear(const NameObj* pObj, const StageSwitchCtrl* pCtrl, const MR::FunctorBase& rOnFunctor) {
        SwitchEventFunctorListener* pListener = new SwitchEventFunctorListener();
        pListener->setOnFunctor(rOnFunctor);

        MR::getSwitchWatcherHolder()->joinSwitchEventListenerAppear(pCtrl, pListener);
    }

    void listenNameObjStageSwitchOnOffAppear(const NameObj* pObj, const StageSwitchCtrl* pCtrl, const MR::FunctorBase& rOnFunctor,
                                             const MR::FunctorBase& rOffFunctor) {
        SwitchEventFunctorListener* pListener = new SwitchEventFunctorListener();
        pListener->setOnFunctor(rOnFunctor);
        pListener->setOffFunctor(rOffFunctor);

        MR::getSwitchWatcherHolder()->joinSwitchEventListenerAppear(pCtrl, pListener);
    }

    void listenNameObjStageSwitchOnA(const NameObj* pObj, const StageSwitchCtrl* pCtrl, const MR::FunctorBase& rOnFunctor) {
        SwitchEventFunctorListener* pListener = new SwitchEventFunctorListener();
        pListener->setOnFunctor(rOnFunctor);

        MR::getSwitchWatcherHolder()->joinSwitchEventListenerA(pCtrl, pListener);
    }

    void listenNameObjStageSwitchOnOffA(const NameObj* pObj, const StageSwitchCtrl* pCtrl, const MR::FunctorBase& rOnFunctor,
                                        const MR::FunctorBase& rOffFunctor) {
        SwitchEventFunctorListener* pListener = new SwitchEventFunctorListener();
        pListener->setOnFunctor(rOnFunctor);
        pListener->setOffFunctor(rOffFunctor);

        MR::getSwitchWatcherHolder()->joinSwitchEventListenerA(pCtrl, pListener);
    }

    void listenNameObjStageSwitchOnB(const NameObj* pObj, const StageSwitchCtrl* pCtrl, const MR::FunctorBase& rOnFunctor) {
        SwitchEventFunctorListener* pListener = new SwitchEventFunctorListener();
        pListener->setOnFunctor(rOnFunctor);

        MR::getSwitchWatcherHolder()->joinSwitchEventListenerB(pCtrl, pListener);
    }

    void listenNameObjStageSwitchOffB(const NameObj* pObj, const StageSwitchCtrl* pCtrl, const MR::FunctorBase& rOffFunctor) {
        SwitchEventFunctorListener* pListener = new SwitchEventFunctorListener();
        pListener->setOffFunctor(rOffFunctor);

        MR::getSwitchWatcherHolder()->joinSwitchEventListenerB(pCtrl, pListener);
    }

    void listenNameObjStageSwitchOnOffB(const NameObj* pObj, const StageSwitchCtrl* pCtrl, const MR::FunctorBase& rOnFunctor,
                                        const MR::FunctorBase& rOffFunctor) {
        SwitchEventFunctorListener* pListener = new SwitchEventFunctorListener();
        pListener->setOnFunctor(rOnFunctor);
        pListener->setOffFunctor(rOffFunctor);

        MR::getSwitchWatcherHolder()->joinSwitchEventListenerB(pCtrl, pListener);
    }

    ResourceHolder* createAndAddResourceHolder(const char* pResource) {
        return SingletonHolder< ResourceHolderManager >::get()->createAndAdd(pResource, nullptr);
    }

    void* loadResourceFromArc(const char* pArchive, const char* pFile) {
        ResourceHolder* pResourceHolder = createAndAddResourceHolder(pArchive);

        return pResourceHolder->mFileInfoTable->getRes(pFile);
    }

    bool isExistResourceInArc(const char* pArchive, const char* pFile) {
        ResourceHolder* pResourceHolder = createAndAddResourceHolder(pArchive);

        return pResourceHolder->mFileInfoTable->isExistRes(pFile);
    }

    const ResTIMG* loadTexFromArc(const char* pArchive, const char* pFile) {
        return static_cast< const ResTIMG* >(loadResourceFromArc(pArchive, pFile));
    }

    const ResTIMG* loadTexFromArc(const char* pArchive) {
        char arcBuf[256];
        snprintf(arcBuf, sizeof(arcBuf), "%s.arc", pArchive);

        char texBuf[256];
        snprintf(texBuf, sizeof(texBuf), "%s.bti", pArchive);

        return loadTexFromArc(arcBuf, texBuf);
    }

    bool isExistFileInArc(const ResourceHolder*, const char*, ...);

    JMapInfo* createCsvParser(const ResourceHolder* pHolder, const char* pFormat, ...) {
        va_list list;
        va_start(list, pFormat);

        return ::tryCreateCsvParserLocal(pHolder, pFormat, list);
    }

    JMapInfo* createCsvParser(const char* pArchive, const char* pFormat, ...) {
        ResourceHolder* pResourceHolder = SingletonHolder< ResourceHolderManager >::get()->createAndAdd(pArchive, nullptr);

        return MR::createCsvParser(pResourceHolder, pFormat);
    }

    // tryCreateCsvParser
    // tryCreateCsvParser

    s32 getCsvDataElementNum(const JMapInfo* pMapInfo) {
        if (pMapInfo->mData != nullptr) {
            return pMapInfo->mData->mNumEntries;
        }

        return 0;
    }

    void getCsvDataStr(const char** pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
        int v7 = pMapInfo->searchItemInfo(pKey);

        if (v7 >= 0) {
            pMapInfo->getValueFast(idx, v7, pOut);
        }
    }

    void getCsvDataStrOrNULL(const char** pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
        getCsvDataStr(pOut, pMapInfo, pKey, idx);

        if (*pOut[0] == 0) {
            *pOut = nullptr;
        }
    }

    void getCsvDataS32(s32* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) { pMapInfo->getValue< s32 >(idx, pKey, pOut); }

    void getCsvDataU8(u8* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
        s32 val = 0;
        pMapInfo->getValue< s32 >(idx, pKey, &val);
        *pOut = val;
    }

    /*
    void getCsvDataF32(f32* pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
        int v7 = pMapInfo->searchItemInfo(pKey);

        if (v7 >= 0) {
            *pOut = *((f32*)(pMapInfo->mData->mNumEntries + pMapInfo->mData->mDataOffset +
                             idx * (pMapInfo->mData->mEntrySize + static_cast< const JMapItem* >(&pMapInfo->mData->mItems)[v7].mOffsData)));
        }
    }
    */

    // MR::getCsvDataBool

    void getCsvDataVec(Vec*, const JMapInfo*, const char*, s32);
    void getCsvDataColor(GXColor*, const JMapInfo*, const char*, s32);

    bool isStageStateScenarioOpeningCamera() { return GameSceneFunction::isExecScenarioOpeningCamera(); }

    bool isStageStatePowerStarAppeared() { return EventFunction::getStageStateKeeper()->mIsPowerStarAppeared; }

    void declarePowerStar(const NameObj* pObj) { declareEventPowerStar(pObj, -1, true); }

    void declarePowerStar(const NameObj* pObj, s32 param2) { declareEventPowerStar(pObj, param2, true); }

    void declarePowerStarCoin100() { declareEventPowerStar("１００枚コイン", -1, true); }

    void appearPowerStarContinueCurrentDemo(const NameObj* pObj, const TVec3f& rParam2) {
        appearEventPowerStar(pObj->mName, -1, &rParam2, true, false);
    }

    void appearPowerStarWithoutDemo(const NameObj* pObj) { appearEventPowerStar(pObj->mName, -1, nullptr, false, true); }

    void requestAppearPowerStar(const NameObj* pObj) { appearEventPowerStar(pObj->mName, -1, nullptr, false, false); }

    void requestAppearPowerStar(const NameObj* pObj, const TVec3f& rParam2) { appearEventPowerStar(pObj->mName, -1, &rParam2, false, false); }

    void requestAppearPowerStar(const NameObj* pObj, s32 param2, const TVec3f& rParam3) {
        appearEventPowerStar(pObj->mName, param2, &rParam3, false, false);
    }

    // requestAppearPowerStar

    void requestAppearPowerStarCoin100() { appearEventPowerStar("１００枚コイン", -1, nullptr, false, false); }

    bool isEndPowerStarAppearDemo(const NameObj* pObj) { return isEndEventPowerStarAppearDemo(pObj->mName); }

    ModelObj* createPowerStarDemoModel(const NameObj* pObj, const char* pName, MtxPtr pMtx) {
        ModelObj* pPowerStarDemoModel = createModelObjNoSilhouettedMapObjStrongLight(pName, "PowerStar", pMtx);

        PowerStar::setupColor(pPowerStarDemoModel, pObj, -1);

        return pPowerStarDemoModel;
    }

    void declareCoin(const NameObj* pObj, s32 param2) {
        if (param2 <= 0) {
            return;
        }

        MR::createCoinHolder();
        MR::getCoinHolder()->declare(pObj, param2);
    }

    s32 getDeclareRemnantCoinCount(const NameObj* pObj) { return MR::getCoinHolder()->getDeclareRemnantCoinCount(pObj); }

    void hopCoin(const NameObj* pObj, const TVec3f& rParam2, const TVec3f& rParam3) { MR::getCoinHolder()->hopCoin(pObj, rParam2, rParam3); }

    void appearCoinFix(const NameObj* pObj, const TVec3f& rParam2, s32 param3) { MR::getCoinHolder()->appearCoinFix(pObj, rParam2, param3); }

    void appearCoinPop(const NameObj* pObj, const TVec3f& rParam2, s32 param3) { MR::getCoinHolder()->appearCoinPop(pObj, rParam2, param3); }

    void appearCoinPopToDirection(const NameObj* pObj, const TVec3f& rParam2, const TVec3f& rParam3, s32 param4) {
        MR::getCoinHolder()->appearCoinPopToDirection(pObj, rParam2, rParam3, param4);
    }

    void appearCoinToVelocity(const NameObj* pObj, const TVec3f& rParam2, const TVec3f& rParam3, s32 param4) {
        MR::getCoinHolder()->appearCoinToVelocity(pObj, rParam2, rParam3, param4);
    }

    void appearCoinCircle(const NameObj* pObj, const TVec3f& rParam2, s32 param3) { MR::getCoinHolder()->appearCoinCircle(pObj, rParam2, param3); }

    // declareStarPiece
    // getDeclareRemnantStarPieceCount
    // getDeclareRemnantStarPieceCountNotExist
    // declareStarPieceReceiver
    // clearGotCountStarPieceReceiver
    // hopStarPiece
    // appearStarPiece
    // appearStarPieceToDirection
    // initStarPieceGetCSSound

    BenefitItemOneUp* createKinokoOneUp() {
        BenefitItemOneUp* pKinokoOneUp = new BenefitItemOneUp("１ＵＰキノコ(create)");
        pKinokoOneUp->initWithoutIter();
        pKinokoOneUp->makeActorDead();

        return pKinokoOneUp;
    }

    void appearKinokoOneUpPop(BenefitItemObj*, MtxPtr, f32);

    BenefitItemLifeUp* createKinokoSuper() {
        BenefitItemLifeUp* pKinokoSuper = new BenefitItemLifeUp("スーパーキノコ");
        pKinokoSuper->initWithoutIter();
        pKinokoSuper->makeActorDead();

        return pKinokoSuper;
    }

    void appearKinokoSuper(BenefitItemLifeUp* pKinokoSuper, MtxPtr pMtx, f32 param3) { appearKinokoOneUpPop(pKinokoSuper, pMtx, param3); }

    void stopScene(s32 param1) { MR::getSceneObj< StopSceneController >(SceneObj_StopSceneController)->requestStopScene(param1); }

    void stopSceneForDefaultHit(s32 param1) {
        MR::getSceneObj< StopSceneController >(SceneObj_StopSceneController)->requestStopSceneDelay(param1, 2);
    }

    bool tryRumblePad(const void* pParam1, const char* pPatternName, s32 channel) {
        return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, pPatternName);
    }

    bool tryRumblePadVeryStrongLong(const void* pParam1, s32 channel) {
        return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "最強【長】");
    }

    bool tryRumblePadVeryStrong(const void* pParam1, s32 channel) {
        return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "最強【長】");
    }

    bool tryRumblePadStrong(const void* pParam1, s32 channel) {
        return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "最強");
    }

    bool tryRumblePadMiddle(const void* pParam1, s32 channel) {
        return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "最強");
    }

    bool tryRumblePadWeak(const void* pParam1, s32 channel) {
        return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "微弱");
    }

    bool tryRumblePadVeryWeak(const void* pParam1, s32 channel) {
        return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "微弱");
    }

    bool tryRumbleDefaultHit(const void* pParam1, s32 channel) {
        return WPadFunction::getWPadRumble(channel)->vibratePatternIfNotExist(pParam1, "強");
    }

    void shakeCameraVeryStrong() { getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_VERY_STRONG); }

    void shakeCameraStrong() { getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_STRONG); }

    void shakeCameraNormalStrong() { getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_NORMAL_STRONG); }

    void shakeCameraNormal() { getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_NORMAL); }

    void shakeCameraNormalWeak() { getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_NORMAL_WEAK); }

    void shakeCameraWeak() { getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_WEAK); }

    void shakeCameraVeryWeak() { getCameraDirector()->mShaker->shakeVertical(CameraShaker::SINGLY_VERTICAL_POWER_VERY_WEAK); }

    void shakeCameraInfinity(NameObj* pObj, f32 param2, f32 param3) { getCameraDirector()->mShaker->shakeInfinity(pObj, param2, 15.0f / param3); }

    void stopShakingCamera(NameObj* pObj) { getCameraDirector()->mShaker->stopShakingInfinity(pObj); }

    bool isName(const NameObj* pObj, const char* pName) { return strcmp(pObj->mName, pName) == 0; }

    bool isSame(const NameObj* pObj1, const NameObj* pObj2) { return pObj1 == pObj2; }

    bool tryRegisterNamePosLinkObj(const NameObj* pObj, const JMapInfoIter& rIter) { return MR::getNamePosHolder()->tryRegisterLinkObj(pObj, rIter); }

    void findNamePos(const char* pName, MtxPtr pMtx) { MR::tryFindLinkNamePos(nullptr, pName, pMtx); }

    void findNamePos(const char* pName, TVec3f* a2, TVec3f* a3) { getNamePosHolder()->find(nullptr, pName, a2, a3); }

    // findNamePosOnGround

    bool tryFindNamePos(const char* pName, MtxPtr pMtx) { return tryFindLinkNamePos(nullptr, pName, pMtx); }

    bool tryFindNamePos(const char* pName, TVec3f* pParam2, TVec3f* pParam3) { return tryFindLinkNamePos(nullptr, pName, pParam2, pParam3); }

    void findLinkNamePos(const NameObj* pObj, const char* pName, MtxPtr pMtx) { tryFindLinkNamePos(pObj, pName, pMtx); }

    // tryFindLinkNamePos

    bool tryFindLinkNamePos(const NameObj* pObj, const char* pName, TVec3f* pParam3, TVec3f* pParam4) {
        return getNamePosHolder()->find(pObj, pName, pParam3, pParam4);
    }
};  // namespace MR
