#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/ClippingDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

AllLiveActorGroup::AllLiveActorGroup()
    : LiveActorGroup("AllLiveActorGroup", 0xA00) {
    _18 = 0;
}

void AllLiveActorGroup::initActorSystemInfo(const JMapInfoIter& rIter) {
    if (mObjectCount <= 0) {
        return;
    }

    s32 cnt = _18;

    if (cnt == mObjectCount) {
        return;
    }

    while (cnt < mObjectCount) {
        LiveActor* actor = getActor(_18);
        MR::getClippingDirector()->initActorSystemInfo(actor, rIter);
        cnt++;
        _18 = _18 + 1;
    }
}

namespace MR {
    AllLiveActorGroup* getAllLiveActorGroup() {
        return MR::getSceneObj< AllLiveActorGroup >(SceneObj_AllLiveActorGroup);
    }

    void startInitLiveActorSystemInfo() {
        AllLiveActorGroup* pAllLiveActorGroup = getAllLiveActorGroup();

        pAllLiveActorGroup->_18 = pAllLiveActorGroup->mObjectCount;
    }

    void initLiveActorSystemInfo(const JMapInfoIter& rIter) {
        AllLiveActorGroup* pAllLiveActorGroup = getAllLiveActorGroup();

        pAllLiveActorGroup->initActorSystemInfo(rIter);
    }

    void endInitLiveActorSystemInfo() {
        AllLiveActorGroup* pAllLiveActorGroup = getAllLiveActorGroup();

        MR::getClippingDirector()->endInitActorSystemInfo();
    }
}; // namespace MR

AllLiveActorGroup::~AllLiveActorGroup() {
}
