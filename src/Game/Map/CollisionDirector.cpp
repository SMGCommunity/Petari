#include "Game/Map/CollisionDirector.hpp"
#include "Game/Map/CollisionCategorizedKeeper.hpp"
#include "Game/Map/CollisionCode.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ObjUtil.hpp"

#define CATEGORY_KEEPER_NUM 4

CollisionDirector::CollisionDirector() : NameObj("地形コリジョン"), mCategoryKeeper(), mCode() {
    mCode = new CollisionCode();
    mCategoryKeeper = new CollisionCategorizedKeeper*[CATEGORY_KEEPER_NUM];

    for (s32 i = 0; i < CATEGORY_KEEPER_NUM; i++) {
        mCategoryKeeper[i] = new CollisionCategorizedKeeper(i);
    }

    MR::connectToScene(this, MR::MovementType_CollisionDirector, -1, -1, -1);
}

void CollisionDirector::init(const JMapInfoIter& rIter) {
}

void CollisionDirector::initAfterPlacement() {
}

void CollisionDirector::movement() {
    for (s32 i = 0; i < CATEGORY_KEEPER_NUM; i++) {
        mCategoryKeeper[i]->movement();
    }
}

CollisionDirector* MR::getCollisionDirector() {
    return MR::getSceneObj< CollisionDirector >(SceneObj_CollisionDirector);
}
