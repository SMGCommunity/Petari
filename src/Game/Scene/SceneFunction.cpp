#include "Game/Effect/EffectSystem.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjListExecutor.hpp"
#include "Game/Scene/SceneDataInitializer.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/SingletonHolder.hpp"

void SceneFunction::startStageFileLoad() {
    MR::createSceneObj(SceneObj_SceneDataInitializer);
    MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer)->startStageFileLoad();
}

void SceneFunction::waitDoneStageFileLoad() {
    MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer)->waitDoneStageFileLoad();
}

void SceneFunction::startActorFileLoadCommon() {
    MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer)->startActorFileLoadCommon();
}

void SceneFunction::startActorFileLoadScenario() {
    MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer)->startActorFileLoadScenario();
}

void SceneFunction::startActorPlacement() {
    MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer)->startActorPlacement();
}

void SceneFunction::initAfterScenarioSelected() {
    MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer)->startStageFileLoadAfterScenarioSelected();
    MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer)->initAfterScenarioSelected();
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
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();
    exec->allocateDrawBufferActorList();
    MR::initConnectting();
}

void CategoryList::execute(MR::MovementType type) {
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();
    exec->executeMovement(type);
}

void CategoryList::execute(MR::CalcAnimType type) {
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();
    exec->executeCalcAnim(type);
}

void CategoryList::execute(MR::DrawType type) {
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();
    exec->executeDraw(type);
}

void CategoryList::entryDrawBuffer2D() {
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();
    exec->entryDrawBuffer2D();
}

void CategoryList::entryDrawBuffer3D() {
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();
    exec->entryDrawBuffer3D();
}

void CategoryList::entryDrawBufferMirror() {
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();
    exec->entryDrawBufferMirror();
}

void CategoryList::drawOpa(MR::DrawBufferType type) {
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();
    exec->drawOpa(type);
}

void CategoryList::drawXlu(MR::DrawBufferType type) {
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();
    exec->drawXlu(type);
}

void CategoryList::requestMovementOn(MR::MovementType type) {
    MR::requestMovementOnWithCategory(type);
}

void CategoryList::requestMovementOff(MR::MovementType type) {
    MR::requestMovementOffWithCategory(type);
}

void SceneFunction::createHioBasicNode(Scene*) {}