﻿#include "Game/Map/CollisionCategorizedKeeper.h"
#include "Game/Map/CollisionCode.h"
#include "Game/Map/CollisionDirector.h"
#include "Game/Scene/SceneObjHolder.h"
#include "Game/Util/ObjUtil.h"

#define NR_CATEGORIZED_KEEPERS 4

CollisionDirector::CollisionDirector() : NameObj("地形コリジョン") {
    mKeepers = NULL;
    mCode = NULL;

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
