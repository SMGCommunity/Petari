#include "Game/Demo/DemoCastGroupHolder.hpp"

DemoCastGroupHolder::DemoCastGroupHolder() : NameObjGroup("デモ関係者グループ保持", 0x20) {

}

bool DemoCastGroupHolder::tryRegisterDemoActor(LiveActor *pActor, const JMapInfoIter &rIter, const JMapIdInfo &rInfo) const {
    for (s32 i = 0; i < mObjectCount; i++) {
        bool ret = reinterpret_cast<DemoCastGroup*>(mObjects[i])->tryRegisterDemoActor(pActor, rIter, rInfo);

        if (ret) {
            return true;
        }
    }

    return false;
}

bool DemoCastGroupHolder::tryRegisterDemoActor(LiveActor *pActor, const char *pName, const JMapInfoIter &rIter) const {
    for (s32 i = 0; i < mObjectCount; i++) {
        bool ret = reinterpret_cast<DemoCastGroup*>(mObjects[i])->tryRegisterDemoActor(pActor, pName, rIter);

        if (ret) {
            return true;
        }
    }

    return false;
}

DemoCastGroup* DemoCastGroupHolder::getCastGroup(int index) const {
    return reinterpret_cast<DemoCastGroup*>(mObjects[index]);
}

DemoCastGroup* DemoCastGroupHolder::findCastGroup(const char *pName) const {
    for (s32 i = 0; i < mObjectCount; i++) {
        NameObj* obj = mObjects[i];
        if (MR::isName(mObjects[i], pName)) {
            return reinterpret_cast<DemoCastGroup*>(obj);
        }
    }

    return 0;
}

DemoCastGroupHolder::~DemoCastGroupHolder() {
    
}