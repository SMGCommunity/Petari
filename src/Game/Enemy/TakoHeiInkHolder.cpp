#include "Game/Enemy/TakoHeiInkHolder.hpp"
#include "Game/Enemy/TakoHeiInk.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

namespace {
    static const s32 sMaxTakoHeiInk = 16;
};  // namespace

TakoHeiInkHolder::TakoHeiInkHolder() : DeriveActorGroup< TakoHeiInk >("タコヘイ墨管理", ::sMaxTakoHeiInk) {
    for (int i = 0; i < ::sMaxTakoHeiInk; i++) {
        TakoHeiInk* newInk = new TakoHeiInk("タコヘイ墨");
        newInk->initWithoutIter();
        registerActor(newInk);
    }
}

namespace MR {
    void createTakoHeiInkHolder() {
        createSceneObj(SceneObj_TakoHeiInkHolder);
    }

    bool spurtTakoHeiInk(const TVec3f& position, const TVec3f& velocity) {
        TakoHeiInk* spurtInk = static_cast< TakoHeiInkHolder* >(getSceneObjHolder()->getObj(SceneObj_TakoHeiInkHolder))->getDeadMember();
        if (spurtInk == nullptr) {
            return false;
        }

        spurtInk->start(position, velocity, false);
        return true;
    }
};  // namespace MR
