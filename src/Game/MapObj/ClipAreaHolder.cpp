#include "Game/MapObj/ClipAreaHolder.hpp"
#include "Game/MapObj/ClipArea.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

namespace {
    ClipAreaHolder* getClipAreaHolder() { return MR::getSceneObj< ClipAreaHolder >(SceneObj_ClipAreaHolder); }
};  // namespace

ClipAreaHolder::ClipAreaHolder(const char* pName) : LiveActorGroup(pName, 64), mIsActive(true) {
    const MR::FunctorV0F func(*MR::setupShadowVolumeDraw);

    MR::registerPreDrawFunction(func, 42);
}

void ClipAreaHolder::init(const JMapInfoIter& rIter) {}

namespace MR {
    void createClipAreaHolder() { createSceneObj(SceneObj_ClipAreaHolder); }

    bool isExistClipAreaHolder() { return isExistSceneObj(SceneObj_ClipAreaHolder); }

    void addClipArea(ClipArea* pClipArea) { getClipAreaHolder()->registerActor(pClipArea); }

    bool isInClipArea(const TVec3f& rParam1, f32 param2) { return getClipAreaHolder()->isInArea(rParam1, param2); }

    bool isActiveClipArea() { return getClipAreaHolder()->mIsActive != false; }

    void activateClipArea() { getClipAreaHolder()->mIsActive = true; }

    void deactivateClipArea() { getClipAreaHolder()->mIsActive = false; }
}  // namespace MR
