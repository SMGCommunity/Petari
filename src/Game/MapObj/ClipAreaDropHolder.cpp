#include "Game/MapObj/ClipAreaDropHolder.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/MapObj/ClipAreaDrop.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

ClipAreaDropHolder::ClipAreaDropHolder() : DeriveActorGroup< ClipAreaDrop >("クリップエリアのしずく管理", 32) {
    ClipAreaDrop* pClipArea = 0;
    for (int i = 0; i < 32; i++) {
        pClipArea = new ClipAreaDrop("クリップエリアのしずく");
        pClipArea->initWithoutIter();
        registerActor(pClipArea);
    }
}

namespace MR {
    NameObj* createClipAreaDropHolder() {
        return MR::createSceneObj(0x4F);
    }

    ClipAreaDrop* getDeadClipAreaDrop() {
        ClipAreaDropHolder* pObj = (ClipAreaDropHolder*)MR::getSceneObjHolder()->getObj(0x4F);
        if (!pObj->getDeadActor())
            return 0;
        else
            return (ClipAreaDrop*)pObj->getDeadActor();
    }

    bool appearClipAreaDrop(const TVec3f& rPos, f32 f) {
        ClipAreaDrop* pClipArea = getDeadClipAreaDrop();

        if (!pClipArea)
            return false;

        pClipArea->mPosition.set(rPos);
        pClipArea->setBaseSize(f);
        pClipArea->appear();
        return true;
    }
}  // namespace MR
ClipAreaDropHolder::~ClipAreaDropHolder() {}
