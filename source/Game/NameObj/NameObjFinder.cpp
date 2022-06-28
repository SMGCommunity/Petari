#include "Game/NameObj/NameObjFinder.h"
#include "Game/SingletonHolder.h"
#include "Game/System/GameSystem.h"

NameObj* NameObjFinder::find(const char *pName) {
    return SingletonHolder<GameSystem>::sInstance->mSceneController->mObjHolder->find(pName);
}