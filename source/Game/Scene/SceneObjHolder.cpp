#include "Game/Scene/SceneObjHolder.h"
#include "Game/AreaObj/AreaObjContainer.h"
#include "Game/Map/PlanetMapCreator.h"
#include "Game/NameObj/NameObj.h"
#include "Game/NameObj/NameObjExecuteHolder.h"
#include "Game/NameObj/NameObjGroup.h"
#include "Game/NameObj/MovementOnOffGroupHolder.h"
#include "Game/LiveActor/AllLiveActorGroup.h"
#include "Game/Scene/StageDataHolder.h"
#include "Game/Util.h"
#include <revolution.h>

SceneObjHolder::SceneObjHolder() {
    for (s32 i = 0; i < 0x7B; i++) {
        mSceneObjs[i] = 0;
    }
}

NameObj* SceneObjHolder::create(int objID) {
    NameObj* obj = mSceneObjs[objID];
    if (obj) {
        return obj;
    }
    
    NameObj* newObj = newEachObj(objID);
    newObj->initWithoutIter();
    mSceneObjs[objID] = newObj;
    return newObj;
}

NameObj* SceneObjHolder::getObj(int objID) const {
    return mSceneObjs[objID];
}

bool SceneObjHolder::isExist(int objID) const {
    return mSceneObjs[objID] != 0;
}

namespace MR {
    NameObj* createSceneObj(int objID) {
        return MR::getSceneObjHolder()->create(objID);
    }
};

/*
// fill me in as we go
NameObj* SceneObjHolder::newEachObj(int objID) {
    switch(objID) {
        case 8:
            return new StageDataHolder(MR::getCurrentStageName(), 0, true);

        case 13:
            return new AreaObjContainer("エリアオブジェクトコンテナ管理");

        case 15:
            return new MovementOnOffGroupHolder("Movementグループ管理");

        case 24:
            return new NameObjGroup("IgnorePauseNameObj", 0x10);

        case 48:
            return new AllLiveActorGroup();

        case 88:
            return new NameObjExecuteHolder(0x1000);

        case 116:
            return new PlanetMapCreator("惑星クリエイタ");
    }

    return NULL;
}
*/
