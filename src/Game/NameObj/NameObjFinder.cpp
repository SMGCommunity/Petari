#include "Game/NameObj/NameObjFinder.hpp"
#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/SingletonHolder.hpp"

NameObj* NameObjFinder::find(const char* pName) {
    return SingletonHolder< GameSystem >::get()->mSceneController->mObjHolder->find(pName);
}
