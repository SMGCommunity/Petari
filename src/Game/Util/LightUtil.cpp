#include "Game/Util/LightUtil.hpp"
#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightDirector.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/LiveActor/LiveActor.hpp"

namespace MR {
    //loadLight

    void loadLightPlayer() {
        SceneObjHolder* holder = MR::getSceneObjHolder();
        LightDirector* lightDirector = (LightDirector*)holder->getObj(SceneObj_LightDirector);
        lightDirector->loadLightPlayer();
    }

    void initActorLightInfoLightType(LiveActor* pLiveActor, s32 a2) {
        pLiveActor->mActorLightCtrl->_4 = a2;
    }

    /*
    TODO: Add DrawBuffer class
    void initActorLightInfoDrawBuffer(LiveActor* pLiveActor, DrawBuffer pDrawBuffer) {
        pLiveActor->mActorLightCtrl->_8 = pDrawBuffer;
    }
    */
};
