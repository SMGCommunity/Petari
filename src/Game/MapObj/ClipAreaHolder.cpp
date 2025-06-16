#include "Game/MapObj/ClipAreaHolder.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

namespace MR {
    void createClipAreaHolder() {
        createSceneObj(83);
    }

    bool isExistClipAreaHolder() {
        return isExistSceneObj(83);
    }

    void addClipArea(ClipArea * mArea) {
        SceneObjHolder* v1 = getSceneObjHolder();
        v1->getObj(83);
        LiveActorGroup* v2;
        v2->registerActor(mArea);
    }
}
