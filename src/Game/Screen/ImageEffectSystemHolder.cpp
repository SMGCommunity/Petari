#include "Game/Screen/ImageEffectSystemHolder.hpp"
#include "Game/Screen/ImageEffectDirector.hpp"
#include "Game/Screen/ImageEffectResource.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ObjUtil.hpp"

ImageEffectSystemHolder::ImageEffectSystemHolder() : NameObj("画像効果管理") {
    mResource = new ImageEffectResource();
    mDirector = nullptr;
    mDirector = new ImageEffectDirector("全画面エフェクト管理");
}

void ImageEffectSystemHolder::pauseOff() {
    if (mDirector != nullptr) {
        MR::requestMovementOn(mDirector);
    }
}

namespace MR {
    void createImageEffectSystemHolder() {
        createSceneObj(SceneObj_ImageEffectSystemHolder);
    }

    ImageEffectSystemHolder* getImageEffectSystemHolder() {
        return getSceneObj<ImageEffectSystemHolder>(SceneObj_ImageEffectSystemHolder);
    }

    bool isExistImageEffectDirector() {
        if (isExistSceneObj(SceneObj_ImageEffectSystemHolder)) {
            return getImageEffectDirector() != nullptr;
        }

        return false;
    }

    ImageEffectDirector* getImageEffectDirector() {
        return getImageEffectSystemHolder()->mDirector;
    }

    ImageEffectResource* getImageEffectResource() {
        return getImageEffectSystemHolder()->mResource;
    }
};  // namespace MR