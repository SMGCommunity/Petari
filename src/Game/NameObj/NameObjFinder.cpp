#include "Game/NameObj/NameObjFinder.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/GameSystem.hpp"

NameObj* NameObjFinder::find(const char *pName) {
    return SingletonHolder<GameSystem>::sInstance->mSceneController->mObjHolder->find(pName);
}