#include "Game/Demo/DemoCastGroupHolder.hpp"
#include "Game/Demo/DemoCastGroup.hpp"
#include "Game/Util/ObjUtil.hpp"

DemoCastGroupHolder::DemoCastGroupHolder() : NameObjGroup("デモ関係者グループ保持", 32) {
}

bool DemoCastGroupHolder::tryRegisterDemoActor(LiveActor* pActor, const JMapInfoIter& rIter, const JMapIdInfo& rInfo) const {
    for (s32 i = 0; i < mObjectCount; i++) {
        bool isRegistered = getCastGroup(i)->tryRegisterDemoActor(pActor, rIter, rInfo);

        if (isRegistered) {
            return true;
        }
    }

    return false;
}

bool DemoCastGroupHolder::tryRegisterDemoActor(LiveActor* pActor, const char* pName, const JMapInfoIter& rIter) const {
    for (s32 i = 0; i < mObjectCount; i++) {
        bool isRegistered = getCastGroup(i)->tryRegisterDemoActor(pActor, pName, rIter);

        if (isRegistered) {
            return true;
        }
    }

    return false;
}

DemoCastGroup* DemoCastGroupHolder::getCastGroup(int index) const {
    return static_cast< DemoCastGroup* >(mObjects[index]);
}

DemoCastGroup* DemoCastGroupHolder::findCastGroup(const char* pName) const {
    for (s32 i = 0; i < mObjectCount; i++) {
        DemoCastGroup* pCastGroup = getCastGroup(i);

        if (MR::isName(getCastGroup(i), pName)) {
            return pCastGroup;
        }
    }

    return nullptr;
}

DemoCastGroupHolder::~DemoCastGroupHolder() {
}
