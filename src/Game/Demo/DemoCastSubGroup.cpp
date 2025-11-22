#include "Game/Demo/DemoCastSubGroup.hpp"
#include "Game/Demo/DemoCastGroupHolder.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/System/MessageHolder.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/JMapInfo.hpp"

DemoCastSubGroup::DemoCastSubGroup(const char* pName) : DemoCastGroup(pName) {
    mExecutor = nullptr;
}

void DemoCastSubGroup::init(const JMapInfoIter& rIter) {
    DemoCastGroup::init(rIter);
    DemoFunction::getDemoCastSubGroupHolder()->registerObj(this);
}

bool DemoCastSubGroup::tryRegisterDemoActor(LiveActor* pActor, const JMapInfoIter& rIter, const JMapIdInfo& rInfo) {
    if (!DemoCastGroup::tryRegisterDemoActor(pActor, rIter, rInfo)) {
        return false;
    }

    if (mExecutor == nullptr) {
        mExecutor = DemoFunction::findDemoExecutor(mName);
        if (mExecutor == nullptr) {
            return false;
        }
    }

    mExecutor->registerDemoActor(pActor, rIter);
    return true;
}

DemoCastSubGroup::~DemoCastSubGroup() {}
