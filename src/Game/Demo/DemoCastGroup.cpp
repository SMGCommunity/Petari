#include "Game/Demo/DemoCastGroup.hpp"

DemoCastGroup::DemoCastGroup(const char *pName) : NameObj(pName) {
    mInfo = 0;
    mGroup = 0;
}

bool DemoCastGroup::tryRegisterDemoActor(LiveActor *pActor, const JMapInfoIter &rIter, const JMapIdInfo &rInfo) {
    bool isNotSame = !(*mInfo == rInfo);

    if (isNotSame) {
        return false;
    }

    registerDemoActor(pActor, rIter);
    return true;
}

bool DemoCastGroup::tryRegisterDemoActor(LiveActor *pActor, const char *pName, const JMapInfoIter &rIter) {
    if (!MR::isEqualString(mName, pName)) {
        return false;
    }

    registerDemoActor(pActor, rIter);
    return true;
}

void DemoCastGroup::registerDemoActor(LiveActor *pActor, const JMapInfoIter & /* unused */) {
    mGroup->registerActor(pActor);
}

DemoCastGroup::~DemoCastGroup() {

}

void DemoCastGroup::init(const JMapInfoIter &rIter) {
    setName(MR::getDemoName(rIter));
    mGroup = new LiveActorGroup("関連者保持", 0xC0);
    mGroup->initWithoutIter();
    mInfo = new JMapIdInfo(MR::getDemoGroupLinkID(rIter), rIter);
}