#include "Game/Map/CollisionCategorizedKeeper.hpp"
#include "Game/Map/CollisionCode.hpp"
#include "Game/Map/CollisionDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ObjUtil.hpp"

#define NR_CATEGORIZED_KEEPERS 4

CollisionDirector::CollisionDirector() : NameObj("地形コリジョン") {
    mKeepers = nullptr;
    mCode = nullptr;

    mCode = new CollisionCode();
    mKeepers = new CollisionCategorizedKeeper *[NR_CATEGORIZED_KEEPERS];

    for (s32 i = 0; i < NR_CATEGORIZED_KEEPERS; i++) {
        mKeepers[i] = new CollisionCategorizedKeeper(i);
    }

    MR::connectToScene(this, 0x20, -1, -1, -1);
}

CollisionDirector::~CollisionDirector() {

}

void CollisionDirector::init(const JMapInfoIter &rIter) {

}

void CollisionDirector::initAfterPlacement() {

}

void CollisionDirector::movement() {
    for (s32 i = 0; i < NR_CATEGORIZED_KEEPERS; i++) {
        mKeepers[i]->movement();
    }
}

CollisionDirector *MR::getCollisionDirector() {
    NameObj *nameObj = MR::getSceneObjHolder()->getObj(SceneObj_CollisionDirector);

    return reinterpret_cast<CollisionDirector *>(nameObj);
}
