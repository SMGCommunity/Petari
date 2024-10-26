#include "Game/Scene/SceneFunction.hpp"
#include "Game/Effect/EffectSystem.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjListExecutor.hpp"
#include "Game/Scene/SceneDataInitializer.hpp"
#include "Game/System/GameSystem.hpp"

void SceneFunction::startStageFileLoad() {
    MR::createSceneObj(SceneObj_SceneDataInitializer);
    SceneDataInitializer* init = MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer);
    init->startStageFileLoad();
}

void SceneFunction::waitDoneStageFileLoad() {
    SceneDataInitializer* init = MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer);
    init->waitDoneStageFileLoad();
}

void SceneFunction::startActorFileLoadCommon() {
    SceneDataInitializer* init = MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer);
    init->startActorFileLoadCommon();
}

void SceneFunction::startActorFileLoadScenario() {
    SceneDataInitializer* init = MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer);
    init->startActorFileLoadScenario();
}

void SceneFunction::startActorPlacement() {
    SceneDataInitializer* init = MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer);
    init->startActorPlacement();
}

void SceneFunction::initAfterScenarioSelected() {
    SceneDataInitializer* init = MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer);
    init->startStageFileLoadAfterScenarioSelected();
    SceneDataInitializer* init_again = MR::getSceneObj<SceneDataInitializer*>(SceneObj_SceneDataInitializer);
    init_again->initAfterScenarioSelected();
}

void SceneFunction::initForNameObj() {
    MR::createSceneObj(SceneObj_NameObjExecuteHolder);
    MR::createSceneObj(SceneObj_StopSceneController);
    MR::createSceneObj(SceneObj_SceneObjMovementCtrl);
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