#include "Game/Enemy/KabokuriFireHolder.hpp"
#include "Game/Enemy/KabokuriFire.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

namespace {
    static const s32 sMaxKabokuriFire = 16;
};  // namespace

// "Kabokuri flame holder"
KabokuriFireHolder::KabokuriFireHolder() : DeriveActorGroup< KabokuriFire >("カボクリ炎管理", ::sMaxKabokuriFire) {
    for (int i = 0; i < ::sMaxKabokuriFire; i++) {
        // "Kabokuri flame"
        KabokuriFire* fire = new KabokuriFire("カボクリ炎");
        fire->initWithoutIter();
        registerActor(fire);
    }
}

void KabokuriFireHolderFunc::createHolder() {
    MR::createSceneObj(SceneObj_KabokuriFireHolder);
}

KabokuriFire* KabokuriFireHolderFunc::getDeadMember() {
    KabokuriFireHolder* holder = MR::getSceneObj< KabokuriFireHolder >(SceneObj_KabokuriFireHolder);

    return holder->getDeadMember();
}

bool KabokuriFireHolderFunc::generateFire(const TVec3f& rPosition, const TVec3f& rGravity, bool setGravity) {
    KabokuriFire* deadMember = KabokuriFireHolderFunc::getDeadMember();
    if (deadMember == nullptr) {
        return false;
    }

    if (setGravity) {
        deadMember->mGravity.set(rGravity);
    }

    deadMember->start(rPosition, setGravity);
    return true;
}
