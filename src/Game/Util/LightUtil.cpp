#include "Game/Util/LightUtil.hpp"
#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightDirector.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Map/LightPointCtrl.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/ResourceHolder.hpp"

namespace MR {

    // TODO define 'somewhere' the types (player:0,strong:1,weak:2,planet:3,coin:4)
    void loadLight(s32 type) {
        if (type == 4) {
            MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->loadLightCoin();
        } else if (type == 0) {
            MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->loadLightPlayer();
        } else {
            AreaLightInfo* lightInfo = reinterpret_cast< AreaLightInfo* >(MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->_18);

            if (lightInfo != nullptr) {
                if (type == 1) {
                    LightFunction::loadActorLightInfo(&lightInfo->mStrongLight);
                } else if (type == 2) {
                    LightFunction::loadActorLightInfo(&lightInfo->mWeakLight);
                } else if (type == 3) {
                    LightFunction::loadActorLightInfo(&lightInfo->mPlanetLight);
                }
            }
        }
    }

    void loadLightPlayer() {
        MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->loadLightPlayer();
    }

    void initActorLightInfoLightType(LiveActor* pActor, s32 a2) {
        pActor->mActorLightCtrl->_4 = a2;
    }

    void initActorLightInfoDrawBuffer(LiveActor* pActor, DrawBuffer* pDrawBuffer) {
        pActor->mActorLightCtrl->_8 = pDrawBuffer;
    }

    void requestPointLight(const LiveActor* pActor, TVec3f pos, Color8 color, f32 brightness, s32 type) {
        MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mPointCtrl->requestPointLight(pActor, pos, color, brightness, type);
    }
};  // namespace MR
