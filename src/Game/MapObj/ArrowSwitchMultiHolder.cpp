#include "Game/MapObj/ArrowSwitchMultiHolder.hpp"
#include "Game/MapObj/ArrowSwitchMulti.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/JMapIdInfo.hpp"

namespace {
    static const s32 sMaxArrowSwitchMulti = 16;
};  // namespace

namespace {
    ArrowSwitchMultiHolder* getArrowSwitchMultiHolder() {
        return MR::getSceneObj< ArrowSwitchMultiHolder >(SceneObj_ArrowSwitchMultiHolder);
    }
};  // namespace

ArrowSwitchMultiHolder::ArrowSwitchMultiHolder() : DeriveActorGroup("複数方向矢印スイッチ管理", ::sMaxArrowSwitchMulti) {
}

ArrowSwitchMulti* ArrowSwitchMultiHolder::findSwitch(const JMapIdInfo* pIdInfo) {
    for (s32 i = 0; i < mObjectCount; i++) {
        ArrowSwitchMulti* sw = static_cast< ArrowSwitchMulti* >(getActor(i));

        if (*sw->mIdInfo == *pIdInfo) {
            return sw;
        }
    }

    return nullptr;
}

namespace MR {
    void createArrowSwitchMultiHolder() {
        MR::createSceneObj(SceneObj_ArrowSwitchMultiHolder);
    }

    void registerArrowSwitchMulti(ArrowSwitchMulti* pSwitch) {
        ::getArrowSwitchMultiHolder()->registerActor(pSwitch);
    }

    void registerArrowSwitchTarget(ArrowSwitchTarget* pTarget) {
        ::getArrowSwitchMultiHolder()->findSwitch(pTarget->mIdInfo)->registerTarget(pTarget);
    }
};  // namespace MR
