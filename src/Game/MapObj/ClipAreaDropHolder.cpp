#include "Game/MapObj/ClipAreaDropHolder.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ClipAreaDrop.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

namespace {
    static const s32 sMaxClipAreaDrop = 32;
};  // namespace

ClipAreaDropHolder::ClipAreaDropHolder() : DeriveActorGroup< ClipAreaDrop >("クリップエリアのしずく管理", ::sMaxClipAreaDrop) {
    ClipAreaDrop* pClipArea = nullptr;

    for (int i = 0; i < ::sMaxClipAreaDrop; i++) {
        pClipArea = new ClipAreaDrop("クリップエリアのしずく");
        pClipArea->initWithoutIter();
        registerActor(pClipArea);
    }
}

namespace MR {
    NameObj* createClipAreaDropHolder() {
        return MR::createSceneObj(SceneObj_ClipAreaDropHolder);
    }

    ClipAreaDrop* getDeadClipAreaDrop() {
        ClipAreaDropHolder* pHolder = MR::getSceneObj< ClipAreaDropHolder >(SceneObj_ClipAreaDropHolder);

        if (pHolder->getDeadActor() != nullptr) {
            return static_cast< ClipAreaDrop* >(pHolder->getDeadActor());
        }

        return nullptr;
    }

    bool appearClipAreaDrop(const TVec3f& rPos, f32 f) {
        ClipAreaDrop* pClipArea = getDeadClipAreaDrop();

        if (pClipArea == nullptr) {
            return false;
        }

        pClipArea->mPosition.set(rPos);
        pClipArea->setBaseSize(f);
        pClipArea->appear();

        return true;
    }
};  // namespace MR

ClipAreaDropHolder::~ClipAreaDropHolder() {
}
