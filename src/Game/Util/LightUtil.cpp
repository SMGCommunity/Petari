#include "Game/Util/LightUtil.hpp"
#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/LightDirector.hpp"
#include "Game/Map/LightFunction.hpp"
#include "Game/Map/LightPointCtrl.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

namespace MR {
    void loadLight(s32 type) {
        if (type == MR::LightType_Coin) {
            MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->loadLightCoin();
            return;
        }

        if (type == MR::LightType_Player) {
            MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->loadLightPlayer();
            return;
        }

        AreaLightInfo* inf = MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mDefaultAreaLight;

        if (inf != nullptr) {
            if (type == MR::LightType_Strong) {
                LightFunction::loadActorLightInfo(&inf->mStrongLight);
            } else if (type == MR::LightType_Weak) {
                LightFunction::loadActorLightInfo(&inf->mWeakLight);
            } else if (type == MR::LightType_Planet) {
                LightFunction::loadActorLightInfo(&inf->mPlanetLight);
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

    void requestPointLight(const LiveActor* pActor, TVec3f pos, Color8 color, f32 intensity, s32 duration) {
        return MR::getSceneObj< LightDirector >(SceneObj_LightDirector)->mPointCtrl->requestPointLight(pActor, pos, color, intensity, duration);
    }
};  // namespace MR
