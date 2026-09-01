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
    void loadLight(s32 type) {
        if (type == 4) {
            MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->loadLightCoin();
        } else if (type == 0) {
            MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->loadLightPlayer();
        } else {
            AreaLightInfo* inf = MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mDefaultAreaLight;

            if (inf != nullptr) {
                if (type == 1) {
                    LightFunction::loadActorLightInfo(&inf->mStrongLight);
                } else if (type == 2) {
                    LightFunction::loadActorLightInfo(&inf->mWeakLight);
                } else if (type == 3) {
                    LightFunction::loadActorLightInfo(&inf->mPlanetLight);
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

    void requestPointLight(const LiveActor* pActor, TVec3f& rPos, Color8 color, f32 a4, s32 a5) {
        return MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mPointCtrl->requestPointLight(pActor, rPos, color, a4, a5);
    }

    // requestPointLight
};  // namespace MR
