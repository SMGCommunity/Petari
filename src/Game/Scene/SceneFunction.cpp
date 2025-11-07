#include "Game/Effect/EffectSystem.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjListExecutor.hpp"
#include "Game/Scene/SceneDataInitializer.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/SingletonHolder.hpp"

namespace {
    SceneDataInitializer* getSceneDataInitializer() {
        return MR::getSceneObj<SceneDataInitializer>(SceneObj_SceneDataInitializer);
    }
};

void SceneFunction::startStageFileLoad() {
    MR::createSceneObj(SceneObj_SceneDataInitializer);
    getSceneDataInitializer()->startStageFileLoad();
}

void SceneFunction::waitDoneStageFileLoad() {
    getSceneDataInitializer()->waitDoneStageFileLoad();
}

void SceneFunction::startActorFileLoadCommon() {
    getSceneDataInitializer()->startActorFileLoadCommon();
}

void SceneFunction::startActorFileLoadScenario() {
    getSceneDataInitializer()->startActorFileLoadScenario();
}

void SceneFunction::startActorPlacement() {
    getSceneDataInitializer()->startActorPlacement();
}

void SceneFunction::initAfterScenarioSelected() {
    getSceneDataInitializer()->startStageFileLoadAfterScenarioSelected();
    getSceneDataInitializer()->initAfterScenarioSelected();
}

void SceneFunction::initForNameObj() {
    MR::createSceneObj(SceneObj_NameObjExecuteHolder);
    MR::createSceneObj(SceneObj_StopSceneController);
    MR::createSceneObj(SceneObj_SceneNameObjMovementController);
}

void SceneFunction::initForLiveActor() {
    MR::createSceneObj(SceneObj_AllLiveActorGroup);
    MR::createSceneObj(SceneObj_ClippingDirector);
    MR::createSceneObj(SceneObj_DemoDirector);
    MR::createSceneObj(SceneObj_SensorHitChecker);
    MR::createSceneObj(SceneObj_CollisionDirector);
    MR::createSceneObj(SceneObj_MessageSensorHolder);
    MR::createSceneObj(SceneObj_LiveActorGroupArray);
    MR::createSceneObj(SceneObj_MovementOnOffGroupHolder);
    MR::createSceneObj(SceneObj_LightDirector);
    MR::createSceneObj(SceneObj_AreaObjContainer);
    MR::createSceneObj(SceneObj_CaptureScreenActor);
    MR::createSceneObj(SceneObj_StageSwitchContainer);
    MR::createSceneObj(SceneObj_SwitchWatcherHolder);
    MR::createSceneObj(SceneObj_SleepControllerHolder);
    MR::createSceneObj(SceneObj_TalkDirector);
    MR::createSceneObj(SceneObj_NPCDirector);
}

void SceneFunction::initEffectSystem(u32 a1, u32 a2) {
    MR::createSceneObj(SceneObj_EffectSystem);
    MR::getEffectSystem()->entry(MR::getParticleResourceHolder(), a1, a2);
}

void SceneFunction::allocateDrawBufferActorList() {
    NameObjListExecutor* pListExecutor = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();

    pListExecutor->allocateDrawBufferActorList();
    MR::initConnectting();
}

void CategoryList::execute(MR::MovementType type) {
    NameObjListExecutor* pListExecutor = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();

    pListExecutor->executeMovement(type);
}

void CategoryList::execute(MR::CalcAnimType type) {
    NameObjListExecutor* pListExecutor = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();

    pListExecutor->executeCalcAnim(type);
}

void CategoryList::execute(MR::DrawType type) {
    NameObjListExecutor* pListExecutor = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();

    pListExecutor->executeDraw(type);
}

void CategoryList::entryDrawBuffer2D() {
    NameObjListExecutor* pListExecutor = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();

    pListExecutor->entryDrawBuffer2D();
}

void CategoryList::entryDrawBuffer3D() {
    NameObjListExecutor* pListExecutor = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();

    pListExecutor->entryDrawBuffer3D();
}

void CategoryList::entryDrawBufferMirror() {
    NameObjListExecutor* pListExecutor = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();

    pListExecutor->entryDrawBufferMirror();
}

void CategoryList::drawOpa(MR::DrawBufferType type) {
    NameObjListExecutor* pListExecutor = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();

    pListExecutor->drawOpa(type);
}

void CategoryList::drawXlu(MR::DrawBufferType type) {
    NameObjListExecutor* pListExecutor = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();

    pListExecutor->drawXlu(type);
}

void CategoryList::requestMovementOn(MR::MovementType type) {
    MR::requestMovementOnWithCategory(type);
}

void CategoryList::requestMovementOff(MR::MovementType type) {
    MR::requestMovementOffWithCategory(type);
}

void SceneFunction::createHioBasicNode(Scene*) {
    
}
