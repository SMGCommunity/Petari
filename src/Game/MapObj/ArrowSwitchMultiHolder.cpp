#include "Game/MapObj/ArrowSwitchMultiHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

ArrowSwitchMultiHolder::ArrowSwitchMultiHolder() : DeriveActorGroup("複数方向矢印スイッチ管理", 0x10) {

}

ArrowSwitchMulti* ArrowSwitchMultiHolder::findSwitch(const JMapIdInfo *pInfo) {
    for (s32 i = 0; i < mObjectCount; i++) {
        ArrowSwitchMulti* sw = reinterpret_cast<ArrowSwitchMulti*>(getActor(i));
        JMapIdInfo* inf = sw->mIDInfo;

        bool isSame = false;
        if (inf->_0 == pInfo->_0 && inf->mZoneID == pInfo->mZoneID) {
            isSame = true;
        }

        if (isSame) {
            return sw;
        }
    }

    return nullptr;
}

namespace MR {
    void createArrowSwitchMultiHolder() {
        MR::createSceneObj(SceneObj_ArrowSwitchMultiHolder);
    }

    void registerArrowSwitchMulti(ArrowSwitchMulti *pSwitch) {
        MR::getSceneObj<ArrowSwitchMultiHolder*>(SceneObj_ArrowSwitchMultiHolder)->registerActor(pSwitch);
    }

    void registerArrowSwitchTarget(ArrowSwitchTarget *pTarget) {
        MR::getSceneObj<ArrowSwitchMultiHolder*>(SceneObj_ArrowSwitchMultiHolder)->findSwitch(pTarget->mJMapIDInfo)->registerTarget(pTarget);
    }
};