#include "Game/Boss/BossAccessor.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

BossAccessor::BossAccessor()
    : NameObj("ボスへのアクセス"),
      mBoss(nullptr) {}

namespace BossAccess {
    void setBossAccessorKoopa(Koopa* pKoopa) {
        MR::createSceneObj(SceneObj_BossAccessor);
        MR::getSceneObj<BossAccessor>(SceneObj_BossAccessor)->setBoss(pKoopa);
    }

    Koopa* getBossAccessorKoopa() {
        if (MR::isExistSceneObj(SceneObj_BossAccessor)) {
            return static_cast<Koopa*>(
                MR::getSceneObj<BossAccessor>(SceneObj_BossAccessor)->getBoss());
        }

        return nullptr;
    }
}; // namespace BossAccess

BossAccessor::~BossAccessor() {
}
