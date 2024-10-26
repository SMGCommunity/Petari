#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/ClippingDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

AllLiveActorGroup::AllLiveActorGroup() : LiveActorGroup("AllLiveActorGroup", 0xA00) {
    _18 = 0;
}

void AllLiveActorGroup::initActorSystemInfo(const JMapInfoIter &rIter) {
    if (mObjectCount <= 0) {
        return;
    }
    
    s32 cnt = _18;

    if (cnt == mObjectCount) {
        return;
    }

    while (cnt != mObjectCount) {
        LiveActor* actor = getActor(_18);
        MR::getClippingDirector()->initActorSystemInfo(actor, rIter);
        cnt++;
        _18 = _18 + 1;
    }
}

namespace MR {
    AllLiveActorGroup* getAllLiveActorGroup() {
        return (AllLiveActorGroup*)MR::getSceneObjHolder()->getObj(SceneObj_AllLiveActorGroup);
    }

    void startInitLiveActorSystemInfo() {
        AllLiveActorGroup* allLiveActorGroup = (AllLiveActorGroup*)MR::getSceneObjHolder()->getObj(SceneObj_AllLiveActorGroup);
        allLiveActorGroup->_18 = allLiveActorGroup->mObjectCount;
    }

    void initLiveActorSystemInfo(const JMapInfoIter &rIter) {
        AllLiveActorGroup* group = (AllLiveActorGroup*)MR::getSceneObjHolder()->getObj(SceneObj_AllLiveActorGroup);
        group->initActorSystemInfo(rIter);
    }

    void endInitLiveActorSystemInfo() {
        MR::getSceneObjHolder()->getObj(SceneObj_AllLiveActorGroup);
        MR::getClippingDirector()->endInitActorSystemInfo();
    }
};

AllLiveActorGroup::~AllLiveActorGroup() {

}
