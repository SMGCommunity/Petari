#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/ClippingDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

AllLiveActorGroup::AllLiveActorGroup() : LiveActorGroup("AllLiveActorGroup", 2560), mActorNum() {
}

void AllLiveActorGroup::startInitActorSystemInfo() {
    mActorNum = getObjNum();
}

void AllLiveActorGroup::initActorSystemInfo(const JMapInfoIter& rIter) {
    if (getObjNum() <= 0) {
        return;
    }

    s32 actorNum = mActorNum;

    if (actorNum == getObjNum()) {
        return;
    }

    for (actorNum; actorNum < getObjNum(); actorNum++) {
        LiveActor* pActor = getActor(mActorNum);
        MR::getClippingDirector()->initActorSystemInfo(pActor, rIter);
        mActorNum++;
    }
}

void AllLiveActorGroup::endInitActorSystemInfo() {
    MR::getClippingDirector()->endInitActorSystemInfo();
}

namespace MR {
    AllLiveActorGroup* getAllLiveActorGroup() {
        return MR::getSceneObj< AllLiveActorGroup >(SceneObj_AllLiveActorGroup);
    }

    void startInitLiveActorSystemInfo() {
        getAllLiveActorGroup()->startInitActorSystemInfo();
    }

    void initLiveActorSystemInfo(const JMapInfoIter& rIter) {
        getAllLiveActorGroup()->initActorSystemInfo(rIter);
    }

    void endInitLiveActorSystemInfo() {
        getAllLiveActorGroup()->endInitActorSystemInfo();
    }
};  // namespace MR
