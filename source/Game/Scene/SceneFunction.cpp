#include "Game/Scene/SceneFunction.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/NameObj/NameObjListExecutor.hpp"
#include "Game/System/GameSystem.hpp"

void SceneFunction::allocateDrawBufferActorList() {
    NameObjListExecutor* exec = SingletonHolder<GameSystem>::get()->mSceneController->getNameObjListExecutor();
    exec->allocateDrawBufferActorList();
    MR::initConnectting();
}

void SceneFunction::createHioBasicNode(Scene*) {}