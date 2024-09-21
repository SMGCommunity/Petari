#include "Game/Scene/SceneFunction.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjListExecutor.hpp"
#include "Game/System/GameSystem.hpp"

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

void SceneFunction::createHioBasicNode(Scene*) {}