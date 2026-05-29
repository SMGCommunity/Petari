#include "Game/Util/LightUtil.hpp"
#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightDirector.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/ResourceHolder.hpp"

namespace MR {
    // loadLight

    void loadLightPlayer() {
        MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->loadLightPlayer();
    }

    void initActorLightInfoLightType(LiveActor* pActor, s32 a2) {
        pActor->mActorLightCtrl->_4 = a2;
    }

    /*
    TODO: Add DrawBuffer class
    void initActorLightInfoDrawBuffer(LiveActor* pActor, DrawBuffer* pDrawBuffer) {
        pActor->mActorLightCtrl->_8 = pDrawBuffer;
    }
    */

    // requestPointLight
};
